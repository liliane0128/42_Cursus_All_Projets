import React, { useState, useRef } from "react";
import { Pencil, Trash2, ChevronDown } from "lucide-react";
import { useAuth } from "../../context/AuthContext";
import { useChat, type ChatMessage, type ProjectChat } from "../../context/ChatContext";
import { getDateLabel, renderWithHighlight, getCurrentTime, createMessageId } from "./utils";
import RobotCatAvatar from "./RobotCatAvatar";
import Avatar from "./Avatar";

type MockUser = { id: string; name: string };

type Props = {
  activeChat: ProjectChat;
  currentUser: MockUser;
  avatarCache: Record<string, string | null>;
  deletedUserIds: Set<string>;
  msgSearchValue: string;
  highlightedMessageId: string | null;
  onSelectUser: (userId: string, displayName: string, avatar?: string) => void;
  onSelectAI: () => void;
  onImageClick: (src: string) => void;
  messagesEndRef: React.RefObject<HTMLDivElement | null>;
};

export default function MessageList({
  activeChat,
  currentUser,
  avatarCache,
  deletedUserIds,
  msgSearchValue,
  highlightedMessageId,
  onSelectUser,
  onSelectAI,
  onImageClick,
  messagesEndRef,
}: Props) {
  const { user, token } = useAuth();
  const { friends, setProjectChats, sendEditMessage, sendMessage } = useChat();

  const [isDragging,        setIsDragging]        = useState(false);
  const [isUploading,       setIsUploading]       = useState(false);
  const [editingMessageId,  setEditingMessageId]  = useState<string | null>(null);
  const [editingContent,    setEditingContent]    = useState<string>("");
  const [showScrollButton,  setShowScrollButton]  = useState(false);

  const containerRef = useRef<HTMLDivElement>(null);

  function handleDragOver(e: React.DragEvent) {
    e.preventDefault();
    setIsDragging(true);
  }

  function handleDragLeave(e: React.DragEvent) {
    if (!e.currentTarget.contains(e.relatedTarget as Node)) setIsDragging(false);
  }

  function handleDrop(e: React.DragEvent) {
    e.preventDefault();
    setIsDragging(false);
    const file = e.dataTransfer.files[0];
    if (file) handleImageUpload(file);
  }

  async function handleImageUpload(file: File) {
    if (!file.type.startsWith("image/")) return;
    if (file.size > 5 * 1024 * 1024) { alert("File exceeds 5 MB limit"); return; }
    setIsUploading(true);
    try {
      const form = new FormData();
      form.append("file", file);
      const res = await fetch("/api/files/upload", {
        method: "POST",
        headers: { Authorization: `Bearer ${token}` },
        body: form,
      });
      const body = await res.json();
      if (!res.ok) throw new Error("Upload failed");
      const { url } = body as { url: string };
      const msg: ChatMessage = {
        id: createMessageId(),
        senderId: currentUser.id,
        senderName: currentUser.name,
        content: url,
        createdAt: getCurrentTime(),
        type: "user",
      };
      sendMessage(activeChat.id, msg);
      setTimeout(() => messagesEndRef.current?.scrollIntoView({ behavior: "smooth" }), 50);
    } catch {
      alert("Image upload failed. Please try again.");
    } finally {
      setIsUploading(false);
    }
  }

  async function handleEditMessage(messageId: string, newContent: string) {
    const res = await fetch(`/api/messages/${messageId}`, {
      method: "PATCH",
      headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
      body: JSON.stringify({ content: newContent }),
    });
    if (res.ok) {
      sendEditMessage(activeChat.id, messageId, newContent);
    }
    setProjectChats(prev => prev.map(c => c.id === activeChat.id
      ? { ...c, messages: c.messages.map(m => m.id === messageId ? { ...m, content: newContent, edited: true } : m) }
      : c
    ));
    setEditingMessageId(null);
  }

  async function handleDeleteMessage(messageId: string) {
    await fetch(`/api/messages/${messageId}`, {
      method: "DELETE",
      headers: { Authorization: `Bearer ${token}` },
    });
    setProjectChats(prev => prev.map(c => c.id === activeChat.id
      ? { ...c, messages: c.messages.filter(m => m.id !== messageId) }
      : c
    ));
  }

  return (
    <div
      ref={containerRef}
      className="flex-1 overflow-y-auto px-5 py-4 space-y-4 relative"
      onDragOver={handleDragOver}
      onDragLeave={handleDragLeave}
      onDrop={handleDrop}
      onScroll={() => {
        const el = containerRef.current;
        if (!el) return;
        const distanceFromBottom = el.scrollHeight - el.scrollTop - el.clientHeight;
        setShowScrollButton(distanceFromBottom > 200);
      }}
    >
      {isDragging && (
        <div className="absolute inset-0 z-20 flex items-center justify-center bg-indigo-50/80 border-2 border-dashed border-indigo-300 rounded-xl pointer-events-none">
          <p className="text-indigo-500 text-[13px] font-medium">Drop image to upload</p>
        </div>
      )}

      {(() => {
        const welcome = activeChat.messages.find(m => m.id === "meow-welcome");
        const rest    = activeChat.messages.filter(m => m.id !== "meow-welcome");

        type Item =
          | { kind: 'separator'; label: string; key: string }
          | { kind: 'message';   message: ChatMessage };

        const items: Item[] = [];
        if (welcome) items.push({ kind: 'message', message: welcome });

        let lastDateStr = '';
        for (const msg of rest) {
          const rawDate = msg.createdAtRaw || msg.createdAt;
          const d = new Date(rawDate);
          const dateStr = isNaN(d.getTime()) ? "unknown" : d.toDateString();
          if (dateStr === "unknown") {
            items.push({ kind: 'message', message: msg });
            continue;
          }
          const label = getDateLabel(msg.createdAtRaw);
          if (label && dateStr !== lastDateStr) {
            items.push({ kind: 'separator', label, key: `sep-${dateStr}` });
            lastDateStr = dateStr;
          }
          items.push({ kind: 'message', message: msg });
        }

        if (items.length === 0) {
          return (
            <div className="h-full flex flex-col items-center justify-center text-center px-6 py-12">
              <p className="text-3xl mb-2">👋</p>
              <p className="text-[13px] text-gray-400">Say hi to start the conversation</p>
            </div>
          );
        }

        return items.map(item => {
          if (item.kind === 'separator') {
            return (
              <div key={item.key} className="flex items-center gap-3 my-4 px-2">
                <div className="flex-1 h-px bg-gray-200" />
                <span className="text-[10px] text-gray-400 font-medium">{item.label}</span>
                <div className="flex-1 h-px bg-gray-200" />
              </div>
            );
          }
          const { message } = item;
          const isMine        = message.senderId === currentUser.id;
          const isAI          = message.type === "ai";
          const isHighlighted = highlightedMessageId === message.id;
          const isImage       = /^https?:\/\/.+\.(png|jpe?g|webp|gif)(\?.*)?$/i.test(message.content)
                             || message.content.includes("/uploads/");
          const senderAvatar = (() => {
            if (message.senderId === String(user?.id)) return user?.avatar ?? null;
            const friend = friends.find(f => String(f.id) === message.senderId);
            if (friend) return friend.avatar ?? null;
            const member = activeChat.members?.find(m => String(m.id) === message.senderId);
            if (member?.avatar) return member.avatar;
            return avatarCache[message.senderId] ?? null;
          })();
          return (
            <div
              key={message.id}
              id={`msg-${message.id}`}
              className={`group flex gap-2.5 animate-message-in ${isMine ? "justify-end" : "justify-start"}`}
            >
              {!isMine && (
                isAI
                  ? <RobotCatAvatar onClick={onSelectAI} />
                  : <Avatar
                      src={senderAvatar}
                      name={message.senderName}
                      size="md"
                      variant="dark"
                      onClick={() => onSelectUser(message.senderId, message.senderName, senderAvatar ?? undefined)}
                    />
              )}

              <div className="relative max-w-[70%]">
                <div className={`flex items-center gap-2 mb-1 ${isMine ? "justify-end" : "justify-start"}`}>
                  <span className="text-[11px] font-medium text-gray-700">
                    {message.senderName}
                    {(deletedUserIds.has(message.senderId) || friends.find(f => String(f.id) === message.senderId)?.is_deleted) && (
                      <span className="text-gray-400 ml-1">(deactivated)</span>
                    )}
                  </span>
                  <span className="text-[10px] text-gray-400">{message.createdAt}</span>
                  {message.edited && (
                    <span className="text-[9px] text-gray-300 italic">edited</span>
                  )}
                </div>

                {isImage ? (
                  <img
                    src={message.content}
                    alt="uploaded"
                    onClick={() => onImageClick(message.content)}
                    className="max-w-[200px] rounded-xl border border-gray-100 object-cover cursor-zoom-in"
                  />
                ) : editingMessageId === message.id ? (
                  <div>
                    <textarea
                      value={editingContent}
                      onChange={e => setEditingContent(e.target.value)}
                      className="bg-white border border-indigo-200 rounded-lg px-3 py-2 text-[12px] text-gray-800 resize-none w-full focus:outline-none focus:ring-1 focus:ring-indigo-300"
                      rows={2}
                      onKeyDown={e => {
                        if (e.key === "Enter" && !e.shiftKey) { e.preventDefault(); handleEditMessage(message.id, editingContent); }
                        if (e.key === "Escape") setEditingMessageId(null);
                      }}
                    />
                    <div className="flex gap-1 mt-1 justify-end">
                      <button onClick={() => setEditingMessageId(null)} className="text-[10px] text-gray-400 hover:text-gray-600">Cancel</button>
                      <button onClick={() => handleEditMessage(message.id, editingContent)} className="text-[10px] text-indigo-500 hover:text-indigo-700 font-medium">Save</button>
                    </div>
                  </div>
                ) : (
                  <div className={`px-3.5 py-2.5 rounded-xl text-[12px] leading-relaxed border transition-colors duration-500 ${
                    isHighlighted
                      ? "bg-amber-50 border-amber-200 text-amber-900"
                      : isMine
                        ? "bg-indigo-50 text-gray-800 border-indigo-100"
                        : isAI
                          ? "bg-indigo-50 text-indigo-900 border-indigo-100"
                          : "bg-gray-100 text-gray-800 border-gray-100"
                  }`}>
                    {renderWithHighlight(message.content, msgSearchValue.trim(), isMine)}
                  </div>
                )}

                {isMine && !isImage && (
                  <div className="absolute -left-16 top-1/2 -translate-y-1/2 flex gap-1 opacity-0 group-hover:opacity-100 transition-opacity">
                    <button
                      onClick={() => { setEditingMessageId(message.id); setEditingContent(message.content); }}
                      className="p-1 rounded text-gray-400 hover:text-gray-600 hover:bg-gray-100"
                    >
                      <Pencil className="w-3 h-3" />
                    </button>
                    <button
                      onClick={() => handleDeleteMessage(message.id)}
                      className="p-1 rounded text-gray-400 hover:text-red-400 hover:bg-red-50"
                    >
                      <Trash2 className="w-3 h-3" />
                    </button>
                  </div>
                )}
              </div>
            </div>
          );
        });
      })()}
      {showScrollButton && (
        <button
          onClick={() => messagesEndRef.current?.scrollIntoView({ behavior: "smooth" })}
          className="absolute bottom-4 right-4 w-9 h-9 rounded-full bg-white border border-gray-200 shadow-md flex items-center justify-center text-gray-500 hover:bg-gray-50 transition-colors z-10"
        >
          <ChevronDown className="w-4 h-4" />
        </button>
      )}
      <div ref={messagesEndRef} />
    </div>
  );
}
