import type { ProjectChat } from "../../context/ChatContext";

const TOKEN_RE = /(@\w+|`[^`]+`|https?:\/\/\S+)/g;

export function getCurrentTime() {
  return new Date().toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" });
}

export function createMessageId() {
  return crypto.randomUUID?.() ?? `msg-${Date.now()}-${Math.random()}`;
}

export function getLastMessage(chat: ProjectChat, currentUser: { id: string; name: string }) {
  if (chat.id === "ai-placeholder" || chat.id.startsWith("ai-")) {
    return "🐾 Hi! I am Meowlinette, your AI assistant.";
  }
  const last = chat.messages[chat.messages.length - 1];
  if (!last) return "No messages yet";
  if (last.senderId === currentUser.id) return `You: ${last.content}`;
  return last.content;
}

export function getLastMessageTime(chat: ProjectChat) {
  return chat.messages[chat.messages.length - 1]?.createdAt ?? "";
}

export function renderMessageContent(content: string, isMine = false) {
  return content.split(TOKEN_RE).map((part, i) => {
    if (part.startsWith("@"))
      return <span key={i} className="text-indigo-500 font-semibold">{part}</span>;
    if (part.startsWith("`") && part.endsWith("`"))
      return <code key={i} className={`rounded px-1 text-[11px] font-mono ${isMine ? "bg-white/20 text-white" : "bg-gray-100 text-indigo-600"}`}>{part.slice(1, -1)}</code>;
    if (part.startsWith("http://") || part.startsWith("https://"))
      return <a key={i} href={part} target="_blank" rel="noreferrer" className="text-indigo-400 underline break-all">{part}</a>;
    return <span key={i}>{part}</span>;
  });
}

export function renderWithHighlight(content: string, query: string, isMine = false) {
  if (!query.trim()) return renderMessageContent(content, isMine);
  const escaped = query.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  const parts = content.split(new RegExp(`(${escaped})`, 'gi'));
  return parts.map((part, i) =>
    part.toLowerCase() === query.toLowerCase()
      ? <mark key={i} className="bg-yellow-300 text-gray-900 rounded-sm not-italic">{part}</mark>
      : <span key={i}>{part}</span>
  );
}

export function getDateLabel(raw: string): string | null {
  if (!raw) return null;
  const d = new Date(raw);
  if (isNaN(d.getTime())) return null;
  const today     = new Date();
  const yesterday = new Date();
  yesterday.setDate(today.getDate() - 1);
  if (d.toDateString() === today.toDateString())     return "Today";
  if (d.toDateString() === yesterday.toDateString()) return "Yesterday";
  return d.toLocaleDateString('en-US', { month: 'short', day: 'numeric', year: 'numeric' });
}
