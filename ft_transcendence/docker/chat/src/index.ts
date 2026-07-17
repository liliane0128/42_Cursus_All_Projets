import express from "express";
import { createServer } from "node:http";
import { WebSocket, WebSocketServer } from "ws";
import * as dotenv from "dotenv";

dotenv.config();

type ChatMessage = {
  id: string;
  senderId: string;
  senderName: string;
  content: string;
  createdAt: string;
  createdAtRaw?: string;
  type: "user" | "ai";
  mentionAll?: boolean;
};

type TypingPayload = {
  chatId: string;
  userId: string;
  userName: string;
};

type ClientEvent =
  | {
      type: "chat:send";
      payload: {
        chatId: string;
        message: ChatMessage;
      };
    }
  | { type: "typing:start"; payload: TypingPayload }
  | { type: "typing:stop";  payload: TypingPayload }
  | { type: "chat:edit_message"; payload: { chatId: string; messageId: string; content: string } };

type ServerEvent =
  | {
      type: "chat:new_message";
      payload: {
        chatId: string;
        message: ChatMessage;
      };
    }
  | {
      type: "presence:update";
      payload: {
        onlineUsers: { id: string; name: string }[];
      };
    }
  | {
      type: "mention:all";
      payload: {
        chatId: string;
        message: ChatMessage;
      };
    }
  | { type: "typing:start"; payload: TypingPayload }
  | { type: "typing:stop";  payload: TypingPayload }
  | { type: "dm:blocked";   payload: { message: string } }
  | { type: "chat:message_edited"; payload: { chatId: string; messageId: string; content: string } }
  | { type: "workspace:notification"; payload: { notif_type: string; message: string; workspace_id: number | null } };

const app = express();
app.use(express.json());
const server = createServer(app);
const wss = new WebSocketServer({ server });

const clientsByUserId = new Map<string, Set<WebSocket>>();
const userNamesById = new Map<string, string>();

app.get("/health", (_req, res) => {
  res.json({ ok: true });
});

function getCurrentTime() {
  return new Date().toLocaleTimeString([], {
    hour: "2-digit",
    minute: "2-digit",
  });
}

function createMessageId(prefix: string) {
  return `${prefix}-${Date.now()}-${Math.random().toString(16).slice(2)}`;
}

function getOnlineUsers() {
  return Array.from(clientsByUserId.keys())
    .filter(id => !id.includes("notif"))
    .map(id => ({
      id,
      name: userNamesById.get(id) ?? id,
    }));
}

function sendToSocket(socket: WebSocket, event: ServerEvent) {
  if (socket.readyState === WebSocket.OPEN) {
    socket.send(JSON.stringify(event));
  }
}

function sendToUser(userId: string, event: ServerEvent) {
  const sockets = clientsByUserId.get(`notif-${userId}`);
  if (!sockets) return;
  for (const socket of sockets) {
    sendToSocket(socket, event);
  }
}

app.post("/internal/push", (req, res) => {
  const { userId, event } = req.body ?? {};
  if (!userId || !event) { res.status(400).json({ error: "missing userId or event" }); return; }
  sendToUser(String(userId), event as ServerEvent);
  res.json({ ok: true });
});

function broadcast(event: ServerEvent) {
  for (const client of wss.clients) {
    sendToSocket(client, event);
  }
}

function broadcastExcept(senderSocket: WebSocket, event: ServerEvent) {
  for (const client of wss.clients) {
    if (client !== senderSocket) sendToSocket(client, event);
  }
}

function broadcastPresence() {
  broadcast({
    type: "presence:update",
    payload: {
      onlineUsers: getOnlineUsers(),
    },
  });
}

function addClient(userId: string, userName: string, socket: WebSocket) {
  const existingSockets = clientsByUserId.get(userId) ?? new Set<WebSocket>();

  existingSockets.add(socket);
  clientsByUserId.set(userId, existingSockets);
  userNamesById.set(userId, userName);
}

function removeClient(userId: string, socket: WebSocket) {
  const existingSockets = clientsByUserId.get(userId);

  if (!existingSockets) {
    return;
  }

  existingSockets.delete(socket);

  if (existingSockets.size === 0) {
    clientsByUserId.delete(userId);
    userNamesById.delete(userId);
  }
}

function removeAiMention(content: string) {
  return content.replace(/^@(AI|Meowlinette|Meow)\b/i, "").trim();
}

function isAiMention(content: string) {
  return /^@(AI|Meowlinette|Meow)\b/i.test(content.trim());
}

function isAllMention(content: string) {
  return /@all\b/i.test(content);
}

async function createAiReply(userId: string, userQuestion: string, chatId: string = "ai", userName: string = "", userMessageId: string = ""): Promise<ChatMessage> {
  const question = removeAiMention(userQuestion);

  let content: string;

  try {
    const response = await fetch("http://localhost:8080/api/ai/chat", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        user_id: userId,
        message: question || "Hello",
        chat_id: chatId,
        user_name: userName,
        user_message_id: userMessageId,
      }),
    });

    const data = await response.json() as { reply: string };
    content = data.reply;
  } catch (error) {
    console.error("FastAPI AI error:", error);
    content = "Meow. I could not reach the AI API right now.";
  }

  return {
    id: createMessageId("meowlinette"),
    senderId: "ai",
    senderName: "Meowlinette",
    content,
    createdAt: getCurrentTime(),
    type: "ai",
  };
}

async function checkDmAllowed(chatId: string, senderId: string): Promise<boolean> {
  try {
    const res = await fetch(`http://localhost:8080/api/dm/check/${chatId}`, {
      headers: { "x-sender-id": senderId },
    });
    return res.ok;
  } catch {
    return true;
  }
}

async function handleClientEvent(event: ClientEvent, senderSocket: WebSocket) {
  if (event.type === "typing:start" || event.type === "typing:stop") {
    broadcastExcept(senderSocket, { type: event.type, payload: event.payload });
    return;
  }

  if (event.type === "chat:edit_message") {
    broadcast({ type: "chat:message_edited", payload: event.payload });
    return;
  }


  if (event.type !== "chat:send") {
    return;
  }

  const { chatId, message } = event.payload;
  console.log('chat:send chatId:', chatId, 'isAiChat:', chatId === 'ai');

  const outMessage = isAllMention(message.content)
    ? { ...message, mentionAll: true }
    : message;

  if (chatId.startsWith("ai-")) {
    sendToSocket(senderSocket, {
      type: "chat:new_message",
      payload: { chatId, message: outMessage },
    });
  } else if (chatId.startsWith("dm-")) {
    const allowed = await checkDmAllowed(chatId, message.senderId);
    if (!allowed) {
      sendToSocket(senderSocket, {
        type: "dm:blocked",
        payload: { message: "You are no longer friends. Add them back to send messages." },
      });
    } else {
      broadcast({
        type: "chat:new_message",
        payload: { chatId, message: outMessage },
      });
    }
  } else {
    broadcast({
      type: "chat:new_message",
      payload: { chatId, message: outMessage },
    });
  }

  if (isAllMention(message.content)) {
    broadcast({
      type: "mention:all",
      payload: { chatId, message: outMessage },
    });
  }

  if (chatId.startsWith("ai-") || isAiMention(message.content)) {
    const socket = senderSocket;
    setTimeout(async () => {
      console.log('senderName:', message.senderName, 'senderId:', message.senderId);
      const aiReply = await createAiReply(message.senderId, message.content, chatId, message.senderName, message.id);
      const replyEvent = {
        type: "chat:new_message" as const,
        payload: { chatId, message: aiReply },
      };
      if (chatId.startsWith("ai-")) {
        sendToSocket(socket, replyEvent);
      } else {
        broadcast(replyEvent);
      }
    }, 700);
  }
}

function windowSetTimeoutForNode(callback: () => void, delay: number) {
  setTimeout(callback, delay);
}

wss.on("connection", (socket, request) => {
  const url = new URL(request.url ?? "/", "http://localhost:3001");
  const userId = url.searchParams.get("userId") ?? createMessageId("guest");
  const userName = url.searchParams.get("userName") ?? "Guest";

  addClient(userId, userName, socket);
  broadcastPresence();

  socket.on("message", (rawData) => {
    try {
      const event = JSON.parse(rawData.toString()) as ClientEvent;
      handleClientEvent(event, socket);
    } catch (error) {
      console.error("Invalid WebSocket message:", error);
    }
  });

  socket.on("close", () => {
    removeClient(userId, socket);
    broadcastPresence();
  });

  socket.on("error", (error) => {
    console.error("WebSocket error:", error);
  });
});

server.listen(3001, () => {
  console.log("WebSocket server running on http://localhost:3001");
});