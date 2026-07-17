import { createContext, useContext, useState, useEffect, useRef } from "react";
import { useNotifications } from "./NotificationContext";
import { useAuth } from "./AuthContext";

export type ChatMessage = {
  id: string;
  senderId: string;
  senderName: string;
  content: string;
  createdAt: string;
  createdAtRaw: string;
  type: "user" | "ai";
  edited?: boolean;
};

export type ProjectChat = {
  id: string;
  title: string;
  description: string;
  avatarLabel: string;
  participantIds: string[];
  members?: { id: number; displayName: string; avatar?: string; role?: string }[];
  unreadCount: number;
  messages: ChatMessage[];
  is_deleted?: boolean;
};

export type Friend = {
  id: number;
  login: string;
  displayName: string;
  avatar?: string;
  status: string;
  is_deleted?: boolean;
};

export type FriendRequest = {
  id: number;
  login: string;
  displayName: string;
  avatar?: string;
  is_deleted?: boolean;
};

type TypingPayload = { chatId: string; userId: string; userName: string };

type ServerEvent =
  | { type: "chat:new_message"; payload: { chatId: string; message: ChatMessage } }
  | { type: "presence:update";  payload: { onlineUsers: { id: string; name: string }[] } }
  | { type: "mention:all";      payload: { chatId: string; message: ChatMessage } }
  | { type: "typing:start";        payload: TypingPayload }
  | { type: "typing:stop";         payload: TypingPayload }
  | { type: "dm:blocked";          payload: { message: string } }
  | { type: "chat:message_edited"; payload: { chatId: string; messageId: string; content: string } };

type ChatContextType = {
  projectChats: ProjectChat[];
  onlineUsers: { id: string; name: string }[];
  isSocketConnected: boolean;
  mentionedChats: Map<string, string>;
  typingUsers: Map<string, string[]>;
  sendMessage: (chatId: string, message: ChatMessage) => void;
  sendTyping: (chatId: string, userId: string, userName: string, isTyping: boolean) => void;
  openChat: (chatId: string) => Promise<void>;
  setMentionedChats: React.Dispatch<React.SetStateAction<Map<string, string>>>;
  friends: Friend[];
  friendRequests: FriendRequest[];
  sendFriendRequest: (userId: number) => Promise<void>;
  acceptFriendRequest: (userId: number) => Promise<void>;
  declineFriendRequest: (userId: number) => Promise<void>;
  removeFriend: (userId: number) => Promise<void>;
  fetchFriends: () => Promise<void>;
  refreshChannels: () => Promise<void>;
  setProjectChats: React.Dispatch<React.SetStateAction<ProjectChat[]>>;
  sendEditMessage: (chatId: string, messageId: string, content: string) => void;
};

const ChatContext = createContext<ChatContextType | null>(null);

const MOCK_USERS = [
  { id: "u1", name: "Huilin" },
  { id: "u2", name: "Alice"  },
  { id: "u3", name: "Noa"    },
  { id: "u4", name: "Mika"   },
];

const MEOW_WELCOME_MSG: ChatMessage = {
  id: "meow-welcome", senderId: "meow", senderName: "Meowlinette",
  content: "🐾 Hi! I am Meowlinette, your AI assistant. Mention me with @Meow, @AI, or @Meowlinette to ask me anything — project questions, task help, or just a chat!",
  createdAt: "00:00", type: "ai",
};

const initialProjectChats: ProjectChat[] = [
  {
    id: "ai-placeholder", title: "Meowlinette", description: "Ask questions with @Meow",
    avatarLabel: "🐾", participantIds: [], unreadCount: 0,
    messages: [MEOW_WELCOME_MSG],
  },
];

export function ChatProvider({ currentUserId, currentUserName }: { currentUserId: string; currentUserName: string }) {
  return null; // placeholder - see below
}

export function ChatContextProvider({ children, currentUserId, currentUserName }: {
  children: React.ReactNode;
  currentUserId: string;
  currentUserName: string;
}) {
  const { upsertChatUnread, markChatAsRead, setActiveChatId: setContextActiveChatId, addFriendRequestNotif } = useNotifications();
  const { token } = useAuth();

  const [projectChats,     setProjectChats]     = useState<ProjectChat[]>(initialProjectChats);
  const [onlineUsers,      setOnlineUsers]       = useState<{ id: string; name: string }[]>([]);
  const [isSocketConnected,setIsSocketConnected] = useState(false);
  const [mentionedChats,   setMentionedChats]   = useState<Map<string, string>>(() => {
    try {
      const raw = localStorage.getItem("mentioned_chats_cache");
      const obj: Record<string, string> = raw ? JSON.parse(raw) : {};
      return new Map(Object.entries(obj));
    } catch {
      return new Map();
    }
  });
  const [typingUsers,      setTypingUsers]       = useState<Map<string, string[]>>(new Map());
  const [activeChatId,     setActiveChatId]      = useState<string | null>(null);
  const [friends,          setFriends]           = useState<Friend[]>([]);
  const [friendRequests,   setFriendRequests]    = useState<FriendRequest[]>([]);

  const activeChatIdRef             = useRef<string | null>(null);
  const currentUserIdRef            = useRef(currentUserId);
  const currentUserNameRef          = useRef(currentUserName);
  const projectChatsRef             = useRef<ProjectChat[]>([]);
  const tokenRef                    = useRef(token);
  const reconnectTimeoutRef         = useRef<ReturnType<typeof setTimeout> | null>(null);
  const socketRef                   = useRef<WebSocket | null>(null);
  const hasInitializedRequestsRef   = useRef(false);
  const friendRequestsRef           = useRef<FriendRequest[]>([]);

  useEffect(() => {
    try {
      const existing = JSON.parse(localStorage.getItem("chat_unread_cache") ?? "{}");
      const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = { ...existing };
      projectChats.forEach(c => {
        const lastMsg = c.messages.length > 0
          ? c.messages[c.messages.length - 1]
          : existing[c.id]?.last_message;
        cache[c.id] = { unreadCount: c.unreadCount, last_message: lastMsg };
      });
      localStorage.setItem("chat_unread_cache", JSON.stringify(cache));
    } catch { /* ignore */ }
  }, [projectChats]);

  useEffect(() => {
    if (!token || token.length < 10) return;
    const sortKey = (t: string) => {
      const d = new Date(t); if (!isNaN(d.getTime())) return d.getTime();
      const iso = new Date(t.replace(' ', 'T')); if (!isNaN(iso.getTime())) return iso.getTime();
      const m = t.match(/^(\d{1,2}):(\d{2})/); return m ? Number(m[1]) * 60 + Number(m[2]) : 0;
    };
    projectChats.forEach(async (chat) => {
      if (chat.id === "ai-placeholder" || chat.id.startsWith("ai-")) return;
      try {
        const res = await fetch(`/api/messages/${chat.id}`, {
          headers: { Authorization: `Bearer ${token}` },
        });
        if (!res.ok) return;
        const rows = await res.json() as Array<{
          id: string; chat_id: string; sender_id: string; sender_name: string;
          content: string; type: string; created_at: string; edited?: boolean;
        }>;
        const fetched: ChatMessage[] = rows.map(r => ({
          id: r.id, senderId: r.sender_id, senderName: r.sender_name,
          content: r.content, type: r.type as "user" | "ai",
          createdAt: (() => { const d = new Date(r.created_at); return isNaN(d.getTime()) ? r.created_at : d.toLocaleTimeString('fr-FR', { hour: '2-digit', minute: '2-digit' }); })(),
          createdAtRaw: r.created_at,
          edited: r.edited ?? false,
        }));
        if (fetched.length === 0) {
          setProjectChats(prev => prev.map(c => c.id === chat.id ? { ...c, messages: [] } : c));
          try {
            const raw = localStorage.getItem("chat_unread_cache");
            const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
            if (cache[chat.id]) { delete cache[chat.id].last_message; localStorage.setItem("chat_unread_cache", JSON.stringify(cache)); }
          } catch { /* ignore */ }
        } else {
          fetched.sort((a, b) => new Date(a.createdAtRaw).getTime() - new Date(b.createdAtRaw).getTime());
          setProjectChats(prev => prev.map(c => c.id === chat.id ? { ...c, messages: fetched } : c));
          try {
            const raw = localStorage.getItem("chat_unread_cache");
            const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
            cache[chat.id] = { ...(cache[chat.id] ?? {}), last_message: fetched[fetched.length - 1] };
            localStorage.setItem("chat_unread_cache", JSON.stringify(cache));
          } catch { /* ignore */ }
        }
      } catch { /* keep existing on network error */ }
    });
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  useEffect(() => {
    try {
      const obj: Record<string, string> = {};
      mentionedChats.forEach((msgId, chatId) => { obj[chatId] = msgId; });
      localStorage.setItem("mentioned_chats_cache", JSON.stringify(obj));
    } catch { /* ignore */ }
  }, [mentionedChats]);

  useEffect(() => { currentUserIdRef.current   = currentUserId;   }, [currentUserId]);
  useEffect(() => { currentUserNameRef.current  = currentUserName; }, [currentUserName]);

  useEffect(() => {
    if (!token || token.length < 10 || !currentUserId) return;
    refreshChannels();
    fetchFriends();
    fetchFriendRequests();
  }, [token, currentUserId]);

  useEffect(() => {
    if (!token || token.length < 10) return;
    const interval = setInterval(() => {
      fetchFriends();
      fetchFriendRequests();
    }, 5000);
    return () => clearInterval(interval);
  }, [token]);

  useEffect(() => { projectChatsRef.current     = projectChats;    }, [projectChats]);
  useEffect(() => { tokenRef.current            = token;           }, [token]);

  useEffect(() => {
    if (!currentUserId) return;
    const url    = `${window.location.protocol === 'https:' ? 'wss' : 'ws'}://${window.location.host}/ws?userId=${currentUserId}&userName=${encodeURIComponent(currentUserName)}`;
    const socket = new WebSocket(url);
    socketRef.current = socket;

    socket.onopen  = () => setIsSocketConnected(true);
    socket.onclose = () => {
      if (socketRef.current === socket) {
        setIsSocketConnected(false);
        reconnectTimeoutRef.current = setTimeout(() => {
          if (socketRef.current === socket) {
            const newUrl = `${window.location.protocol === 'https:' ? 'wss' : 'ws'}://${window.location.host}/ws?userId=${currentUserId}&userName=${encodeURIComponent(currentUserName)}`;
            const newSocket = new WebSocket(newUrl);
            socketRef.current = newSocket;
            newSocket.onopen    = () => setIsSocketConnected(true);
            newSocket.onclose   = socket.onclose;
            newSocket.onerror   = () => setIsSocketConnected(false);
            newSocket.onmessage = socket.onmessage;
          }
        }, 3000);
      }
    };
    socket.onerror = () => setIsSocketConnected(false);

    socket.onmessage = async (event) => {
      try {
        const e = JSON.parse(event.data) as ServerEvent;

        if (e.type === "presence:update") {
          setOnlineUsers(e.payload.onlineUsers);
          return;
        }

        if (e.type === "mention:all") {
          const { chatId, message } = e.payload;
          if (String(message.senderId) !== String(currentUserIdRef.current)) {
            setMentionedChats(prev => new Map(prev).set(chatId, message.id));
          }
          return;
        }

        if (e.type === "typing:start") {
          const { chatId, userName } = e.payload;
          setTypingUsers(prev => {
            const next = new Map(prev);
            const names = next.get(chatId) ?? [];
            if (!names.includes(userName)) next.set(chatId, [...names, userName]);
            return next;
          });
          return;
        }

        if (e.type === "typing:stop") {
          const { chatId, userName } = e.payload;
          setTypingUsers(prev => {
            const next = new Map(prev);
            const names = (next.get(chatId) ?? []).filter(n => n !== userName);
            names.length ? next.set(chatId, names) : next.delete(chatId);
            return next;
          });
          return;
        }

        if (e.type === "chat:new_message") {
          const { chatId, message } = e.payload;
          const isFromMe     = message.senderId === currentUserIdRef.current;

          if (!message.createdAtRaw) {
            message.createdAtRaw = new Date().toISOString();
          }

          if (chatId.startsWith("dm-") && message.senderId !== currentUserIdRef.current) {
            try {
              const res = await fetch(`/api/dm/check/${chatId}`, {
                headers: { "x-sender-id": message.senderId },
              });
              const data = await res.json();
              if (!data.allowed) return;
            } catch { /* allow by default */ }
          }
          const isActiveChat = chatId === activeChatIdRef.current;
          let chatBefore     = projectChatsRef.current.find(c => c.id === chatId);

          if (!chatBefore) {
            await refreshChannels();
            chatBefore = projectChatsRef.current.find(c => c.id === chatId);
          }

          if (!isFromMe && !isActiveChat && chatBefore) {
            upsertChatUnread({ chatId, chatTitle: chatBefore.title, unreadCount: chatBefore.unreadCount + 1 });
          }

          if (!isFromMe && message.content.toLowerCase().includes(`@${currentUserNameRef.current.toLowerCase()}`)) {
            setMentionedChats(prev => new Map(prev).set(chatId, message.id));
          }

          setProjectChats(prev =>
            prev.map(c => c.id === chatId
              ? { ...c, unreadCount: isFromMe || isActiveChat ? c.unreadCount : c.unreadCount + 1, messages: [...c.messages, message].filter((m, i, arr) => arr.findIndex(x => x.id === m.id) === i) }
              : c
            )
          );

          if (message.type === "ai") {
            window.dispatchEvent(new CustomEvent('ai-task-updated'));
          }

          if ((message.type === "user" || message.type === "ai") && token && token !== "null" && token.length > 10 && (message.type === "ai" || message.senderId === currentUserIdRef.current)) {
            (async () => {
              try {
                const res = await fetch("/api/messages", {
                  method: "POST",
                  headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
                  body: JSON.stringify({
                    id: message.id,
                    chat_id: chatId,
                    sender_id: message.senderId,
                    sender_name: message.senderName,
                    content: message.content,
                    type: message.type,
                    created_at: new Date().toISOString(),
                  }),
                });
                if (res.status === 403) {
                  setProjectChats(prev =>
                    prev.map(c => c.id === chatId
                      ? { ...c, messages: c.messages.filter(m => m.id !== message.id) }
                      : c
                    )
                  );
                  window.dispatchEvent(new CustomEvent("dm:blocked", {
                    detail: { message: "You are no longer friends. Add them back to send messages." },
                  }));
                }
              } catch { /* ignore network errors */ }
            })();
          }
        }

        if (e.type === "dm:blocked") {
          window.dispatchEvent(new CustomEvent("dm:blocked", { detail: { message: e.payload.message } }));
          return;
        }

        if (e.type === "chat:message_edited") {
          const { chatId, messageId, content } = e.payload;
          setProjectChats(prev => prev.map(c => c.id === chatId
            ? { ...c, messages: c.messages.map(m => m.id === messageId ? { ...m, content, edited: true } : m) }
            : c
          ));
          return;
        }
      } catch { /* ignore */ }
    };

    return () => {
      if (reconnectTimeoutRef.current) clearTimeout(reconnectTimeoutRef.current);
      socket.close();
    };
  }, [currentUserId, currentUserName]);

  function sendTyping(chatId: string, userId: string, userName: string, isTyping: boolean) {
    if (socketRef.current?.readyState === WebSocket.OPEN) {
      socketRef.current.send(JSON.stringify({
        type: isTyping ? "typing:start" : "typing:stop",
        payload: { chatId, userId, userName },
      }));
    }
  }

  function sendEditMessage(chatId: string, messageId: string, content: string) {
    if (socketRef.current?.readyState === WebSocket.OPEN) {
      socketRef.current.send(JSON.stringify({ type: "chat:edit_message", payload: { chatId, messageId, content } }));
    }
  }

  function sendMessage(chatId: string, message: ChatMessage) {
    if (socketRef.current?.readyState === WebSocket.OPEN) {
      socketRef.current.send(JSON.stringify({ type: "chat:send", payload: { chatId, message } }));
    } else {
      setProjectChats(prev =>
        prev.map(c => c.id === chatId ? { ...c, messages: [...c.messages, message] } : c)
      );
    }
  }

  async function fetchMessages(chatId: string) {
    if (!token) return;
    try {
      const res = await fetch(`/api/messages/${chatId}`, {
        headers: { Authorization: `Bearer ${token}` },
      });
      if (res.ok) {
        const rows = await res.json() as Array<{
          id: string; chat_id: string; sender_id: string; sender_name: string;
          content: string; type: string; created_at: string; edited?: boolean;
        }>;
        const fetched: ChatMessage[] = rows.map(r => ({
          id: r.id,
          senderId: r.sender_id,
          senderName: r.sender_name,
          content: r.content,
          type: r.type as "user" | "ai",
          createdAt: (() => { const d = new Date(r.created_at); return isNaN(d.getTime()) ? r.created_at : d.toLocaleTimeString('fr-FR', { hour: '2-digit', minute: '2-digit' }); })(),
          createdAtRaw: r.created_at,
          edited: r.edited ?? false,
        }));
        const sortKey = (t: string) => {
          const d = new Date(t);
          if (!isNaN(d.getTime())) return d.getTime();
          const iso = new Date(t.replace(' ', 'T'));
          if (!isNaN(iso.getTime())) return iso.getTime();
          const match = t.match(/^(\d{1,2}):(\d{2})/);
          if (match) return Number(match[1]) * 60 + Number(match[2]);
          return 0;
        };

        if (fetched.length === 0) {
          const finalMessages = chatId.startsWith("ai-") ? [MEOW_WELCOME_MSG] : [];
          setProjectChats(prev => prev.map(c => c.id === chatId ? { ...c, messages: finalMessages } : c));
          try {
            const raw = localStorage.getItem("chat_unread_cache");
            const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
            if (cache[chatId]) { delete cache[chatId].last_message; localStorage.setItem("chat_unread_cache", JSON.stringify(cache)); }
          } catch { /* ignore */ }
        } else {
          fetched.sort((a, b) => new Date(a.createdAtRaw).getTime() - new Date(b.createdAtRaw).getTime());
          const finalMessages = chatId.startsWith("ai-")
            ? [MEOW_WELCOME_MSG, ...fetched.filter(m => m.id !== "meow-welcome")]
            : fetched;
          setProjectChats(prev => prev.map(c => c.id === chatId ? { ...c, messages: finalMessages } : c));
          try {
            const raw = localStorage.getItem("chat_unread_cache");
            const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
            cache[chatId] = { ...(cache[chatId] ?? {}), last_message: finalMessages[finalMessages.length - 1] };
            localStorage.setItem("chat_unread_cache", JSON.stringify(cache));
          } catch { /* ignore */ }
        }
      }
    } catch { /* keep existing messages on network error */ }
  }

  useEffect(() => {
    if (!token || token.length < 10 || !activeChatId) return;
    fetchMessages(activeChatId);
  }, [token, activeChatId]);

  async function openChat(chatId: string) {
    activeChatIdRef.current = chatId;
    setActiveChatId(chatId);
    setContextActiveChatId(chatId);
    setProjectChats(prev =>
      prev.map(c => c.id === chatId ? { ...c, unreadCount: 0 } : c)
    );
    markChatAsRead(chatId);
    setMentionedChats(prev => { const n = new Map(prev); n.delete(chatId); return n; });
    try {
      const rawM = localStorage.getItem("mentioned_chats_cache");
      const objM: Record<string, string> = rawM ? JSON.parse(rawM) : {};
      delete objM[chatId];
      localStorage.setItem("mentioned_chats_cache", JSON.stringify(objM));
    } catch { /* ignore */ }
    try {
      const raw = localStorage.getItem("chat_unread_cache");
      const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
      cache[chatId] = { ...cache[chatId], unreadCount: 0 };
      localStorage.setItem("chat_unread_cache", JSON.stringify(cache));
    } catch { /* ignore */ }
  }

  async function refreshChannels() {
    if (!token || token.length < 10 || !currentUserId) return;
    try {
      const headers = { Authorization: `Bearer ${token}` };
      const [channelsRes, dmsRes] = await Promise.all([
        fetch("/api/channels", { headers }),
        fetch("/api/dm",       { headers }),
      ]);
      if (!channelsRes.ok) return;
      const channels = await channelsRes.json() as Array<{
        id: string; name: string; description: string; color: string; member_ids: number[];
        members?: { id: number; display_name: string; avatar?: string; role?: string }[];
      }>;
      const dms = dmsRes.ok
        ? await dmsRes.json() as Array<{ chat_id: string; user_id: number; display_name: string; is_deleted?: boolean }>
        : [];
      const raw = localStorage.getItem("chat_unread_cache");
      const cache: Record<string, { unreadCount: number; last_message?: ChatMessage }> = raw ? JSON.parse(raw) : {};
      const workspaceChats: ProjectChat[] = channels.map(ch => ({
        id: ch.id,
        title: ch.name,
        description: ch.description,
        avatarLabel: ch.name[0]?.toUpperCase() ?? "?",
        participantIds: ch.member_ids.map(String),
        members: (ch.members ?? []).map((m: { id: number; display_name: string; avatar?: string; role?: string }) => ({ id: m.id, displayName: m.display_name, avatar: m.avatar ?? undefined, role: m.role })),
        unreadCount: cache[ch.id]?.unreadCount ?? 0,
        messages: cache[ch.id]?.last_message ? [cache[ch.id].last_message] : [],
      }));
      const dmChats: ProjectChat[] = dms.map(dm => ({
        id: dm.chat_id,
        title: dm.display_name,
        description: "Direct Message",
        avatarLabel: dm.display_name[0]?.toUpperCase() ?? "?",
        participantIds: [currentUserId, String(dm.user_id)],
        unreadCount: cache[dm.chat_id]?.unreadCount ?? 0,
        messages: cache[dm.chat_id]?.last_message ? [cache[dm.chat_id].last_message] : [],
        is_deleted: dm.is_deleted,
      }));
      const meowlinetteChat: ProjectChat = {
        id: `ai-${currentUserId}`,
        title: "Meowlinette", description: "Ask questions with @Meow",
        avatarLabel: "🐾", participantIds: [], unreadCount: 0,
        messages: [MEOW_WELCOME_MSG],
      };
      setProjectChats([...workspaceChats, ...dmChats, meowlinetteChat]);
      const activeId = activeChatIdRef.current;
      if (activeId && !activeId.startsWith('ai-')) {
        fetchMessages(activeId);
      }
    } catch { /* keep existing on network error */ }
  }

  async function fetchFriends() {
    if (!token) return;
    try {
      const res = await fetch("/api/friends", { headers: { Authorization: `Bearer ${token}` } });
      if (res.ok) setFriends(await res.json());
    } catch { /* ignore */ }
  }

  async function fetchFriendRequests() {
    if (!token) return;
    try {
      const res = await fetch("/api/friends/requests", {
        headers: { Authorization: `Bearer ${token}` },
      });
      if (res.ok) {
        const data: FriendRequest[] = await res.json();
        const prevRequests = friendRequestsRef.current;
        if (hasInitializedRequestsRef.current) {
          const newOnes = data.filter(r =>
            !prevRequests.some(p => p.id === r.id));
          newOnes.forEach(r =>
            addFriendRequestNotif(r.displayName, r.id));
        } else {
          hasInitializedRequestsRef.current = true;
        }
        friendRequestsRef.current = data;
        setFriendRequests(data);
      }
    } catch { /* ignore */ }
  }

  async function sendFriendRequest(userId: number) {
    if (!token) return;
    await fetch(`/api/friends/request/${userId}`, {
      method: "POST",
      headers: { Authorization: `Bearer ${token}` },
    });
  }

  async function acceptFriendRequest(userId: number) {
    if (!token) return;
    await fetch(`/api/friends/accept/${userId}`, {
      method: "POST",
      headers: { Authorization: `Bearer ${token}` },
    });
    await fetchFriends();
    await fetchFriendRequests();
  }

  async function declineFriendRequest(userId: number) {
    if (!token) return;
    await fetch(`/api/friends/${userId}`, {
      method: "DELETE",
      headers: { Authorization: `Bearer ${token}` },
    });
    await fetchFriendRequests();
    await fetchFriends();
  }

  async function removeFriend(userId: number) {
    if (!token) return;
    await fetch(`/api/friends/${userId}`, {
      method: "DELETE",
      headers: { Authorization: `Bearer ${token}` },
    });
    await fetchFriends();
  }

  return (
    <ChatContext.Provider value={{
      projectChats, onlineUsers, isSocketConnected,
      mentionedChats, setMentionedChats, typingUsers, sendMessage, sendTyping, openChat,
      friends, friendRequests, sendFriendRequest, acceptFriendRequest, declineFriendRequest, removeFriend, fetchFriends, refreshChannels, setProjectChats, sendEditMessage,
    }}>
      {children}
    </ChatContext.Provider>
  );
}

export function useChat() {
  const ctx = useContext(ChatContext);
  if (!ctx) throw new Error("useChat must be used inside ChatContextProvider");
  return ctx;
}

export { MOCK_USERS, initialProjectChats };