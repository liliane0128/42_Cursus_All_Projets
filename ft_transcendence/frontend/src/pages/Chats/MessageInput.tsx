import React, { useRef, useState, useEffect, type ChangeEvent, type KeyboardEvent } from "react";
import { Send, ImageIcon, Smile } from "lucide-react";
import { flushSync } from "react-dom";
import { useAuth } from "../../context/AuthContext";
import { useChat, MOCK_USERS, type ChatMessage } from "../../context/ChatContext";
import { getCurrentTime, createMessageId } from "./utils";
import Avatar from "./Avatar";

const BANNED_WORDS = [
  // English - most common profanity
  "fuck", "fucking", "fucked", "fucker",
  "shit", "shitty", "bullshit",
  "asshole", "ass",
  "bitch", "bitchy",
  "cunt", "dick", "cock", "pussy",
  "bastard", "motherfucker", "jackass",
  "damn", "hell",
  // English - hate speech & slurs
  "nigger", "nigga", "faggot", "fag",
  "retard", "retarded", "tranny",
  "dyke", "spic", "chink", "gook", "kike",
  "whore", "slut", "skank", "hoe", "thot",
  "bimbo", "nazi", "kys", "kill yourself",
  // French - most common profanity
  "putain", "merde", "connard", "connasse",
  "salope", "enculé", "enculer", "va te faire foutre",
  "bordel", "chier", "bite", "couille",
  "pute", "nique", "niquer", "batard", "bâtard",
  "ferme ta gueule", "ta gueule", "casse toi",
  // French - hate speech & slurs
  "pd", "gonzesse", "tapette", "bamboula",
  "nazi", "gros con",
];

const isPrideMonth = new Date().getMonth() === 5; // June = 5

const EMOJIS = [
  // Pride
  "❤️","🧡","💛","💚","💙","💜","🏳️‍🌈","🏳️‍⚧️","🥰","🫶",
  // Faces & Emotions
  "😀","😁","😂","🤣","😊","😍","😘","😎","🤩","🥳",
  "😢","😭","😡","😱","😴","🤔","😅","🙄","😇",
  "😜","🤯","😬","🤤","😏","🤗","🙃","🤐","🥺","😔",
  "😒","😌","😋","🤪","😝","🤑","🤠","🥸","😤","😠",
  "🤬","😈","👿","💀","☠️","🤡","👹","👺","👻","👽",
  // Gestures & People
  "👍","👎","👏","🙏","💪","🤝","✌️","🤞","👋","🙌",
  "🤜","🤛","👊","✊","🤲","👐","🫰","🤙","👈",
  "👉","👆","👇","☝️","🫵","🖐️","✋","🤚","👌","🤌",
  // Hearts & Love
  "🖤","🤍","🤎","💔",
  "❤️‍🔥","💕","💞","💓","💗","💖","💝","💘","💟","☮️",
  // Animals
  "🐱","🐶","🐼","🦊","🐨","🐯","🦁","🐮","🐷","🐸",
  "🐙","🦋","🐝","🦄","🐲","🦖","🐬","🦈","🦅","🐧",
  // Food & Drink
  "🍕","🍔","🌮","🍜","🍣","🍩","🎂","🍪","🧁","🍫",
  "☕","🧋","🍵","🥤","🍺","🥂","🍷","🧃","🍹","🥛",
  // Activities & Objects
  "⚽","🏀","🎮","🎸","🎵","🎉","🎊","🏆","🥇","🎯",
  "🚀","✈️","🚗","🏠","💻","📱","📷","🎬","📚","🔑",
  // Nature & Weather
  "🌟","⭐","🌈","☀️","🌙","⚡","🌊","🔥","❄️","🌸",
  "🌺","🌻","🍀","🌿","🍁","🍄","🌵","🌴","🌍","🌏",
  // Symbols
  "💯","✅","❌","⚠️","💡","🔔","📢","💬","💭","🗯️",
  "🎁","💰","💎","🔮","🧿","⚙️","🔧","🔨","🧲","🪄",
];

type Props = {
  activeChatId: string | null;
  messagesEndRef: React.RefObject<HTMLDivElement | null>;
};

export default function MessageInput({ activeChatId, messagesEndRef }: Props) {
  const { user, token } = useAuth();
  const {
    projectChats, setProjectChats, sendMessage, sendTyping, typingUsers, onlineUsers, friends,
  } = useChat();

  const currentUser   = user ? { id: String(user.id), name: user.displayName || user.login } : MOCK_USERS[0];
  const currentUserId = user ? String(user.id) : "";
  const activeChat    = projectChats.find(c => c.id === activeChatId);

  const mentionable = [
    { id: "meow", name: "Meow" },
    { id: "all",  name: "all"  },
    ...(onlineUsers ?? []).filter(u =>
      u.id !== String(user?.id) &&
      !u.id.includes("notif") &&
      !friends.find(f => String(f.id) === u.id)?.is_deleted
    ),
  ];

  const [inputValue,          setInputValue]          = useState("");
  const [showMentionDropdown, setShowMentionDropdown] = useState(false);
  const [mentionQuery,        setMentionQuery]        = useState("");
  const [showEmojiPicker,     setShowEmojiPicker]     = useState(false);
  const [isUploading,         setIsUploading]         = useState(false);
  const [aiModel,             setAiModel]             = useState("llama-3.3-70b-versatile");
  const [isAiStreaming,       setIsAiStreaming]        = useState(false);
  const [streamingContent,    setStreamingContent]    = useState("");
  const [moderationError,     setModerationError]     = useState<string | null>(null);
  const [sendBlockedMsg,      setSendBlockedMsg]      = useState(false);
  const [dmBlockedMsg,        setDmBlockedMsg]        = useState<string | null>(null);

  const textareaRef    = useRef<HTMLTextAreaElement | null>(null);
  const fileInputRef   = useRef<HTMLInputElement | null>(null);
  const typingTimerRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  const filteredMentions = mentionable.filter(u =>
    u.name.toLowerCase().startsWith(mentionQuery.toLowerCase())
  );

  useEffect(() => {
    const handler = (e: Event) => {
      const msg = (e as CustomEvent<{ message: string }>).detail.message;
      setDmBlockedMsg(msg);
      setTimeout(() => setDmBlockedMsg(null), 3000);
    };
    window.addEventListener("dm:blocked", handler);
    return () => window.removeEventListener("dm:blocked", handler);
  }, []);

  function handleInputChange(e: ChangeEvent<HTMLTextAreaElement>) {
    const v = e.target.value;
    setInputValue(v);
    const el = textareaRef.current;
    if (el) { el.style.height = "auto"; el.style.height = `${Math.min(el.scrollHeight, 96)}px`; }

    if (currentUserId) {
      sendTyping(activeChatId, currentUserId, user?.displayName ?? "", true);
      if (typingTimerRef.current) clearTimeout(typingTimerRef.current);
      typingTimerRef.current = setTimeout(() => {
        sendTyping(activeChatId, currentUserId, user?.displayName ?? "", false);
      }, 1000);
    }

    if (activeChat?.id.startsWith("dm-")) {
      setShowMentionDropdown(false);
      return;
    }

    const at = v.lastIndexOf("@");
    if (at !== -1) {
      const after = v.slice(at + 1);
      const spaceIdx = after.indexOf(" ");
      if (spaceIdx !== -1) {
        setShowMentionDropdown(false);
      } else {
        setMentionQuery(after);
        setShowMentionDropdown(true);
      }
    } else {
      setShowMentionDropdown(false);
    }
  }

  function handleMentionSelect(name: string) {
    const at = inputValue.lastIndexOf("@");
    setInputValue(inputValue.slice(0, at) + `@${name} `);
    setShowMentionDropdown(false);
  }

  function handleKeyDown(e: KeyboardEvent<HTMLTextAreaElement>) {
    if (e.key === "Enter" && !e.shiftKey) { e.preventDefault(); handleSendMessage(); }
    if (e.key === "Escape") setShowMentionDropdown(false);
  }

  async function sendToAI(trimmed: string) {
    const userMsg = { id: createMessageId(), senderId: currentUser.id, senderName: currentUser.name, content: trimmed, createdAt: getCurrentTime(), type: "user" as const };
    setProjectChats(prev => prev.map(c => c.id === activeChat!.id ? { ...c, messages: [...c.messages, userMsg] } : c));

    fetch("/api/messages", {
      method: "POST",
      headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
      body: JSON.stringify({
        id: userMsg.id,
        chat_id: activeChat!.id,
        sender_id: String(currentUser.id),
        sender_name: currentUser.name,
        content: trimmed,
        type: "user",
        created_at: new Date().toISOString(),
      }),
    }).catch(() => {});

    setInputValue("");
    setIsAiStreaming(true);
    setStreamingContent("");

    const aiMsgId = createMessageId();
    const aiMsg = { id: aiMsgId, senderId: "ai", senderName: "Meowlinette", content: "", createdAt: new Date().toISOString(), type: "ai" as const };
    setProjectChats(prev => prev.map(c => c.id === activeChat!.id ? { ...c, messages: [...c.messages, aiMsg] } : c));

    try {
      const res = await fetch("/api/ai/stream", {
        method: "POST",
        headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
        body: JSON.stringify({ user_id: String(currentUser.id), message: trimmed, chat_id: activeChat!.id, user_name: currentUser.name, user_message_id: userMsg.id, model: aiModel }),
      });
      const reader = res.body!.getReader();
      const decoder = new TextDecoder();
      let fullContent = "";
      let buffer = "";

      while (true) {
        const { done, value } = await reader.read();
        if (done) break;

        buffer += decoder.decode(value, { stream: true });
        const lines = buffer.split("\n");
        buffer = lines.pop() ?? "";

        for (const line of lines) {
          const trimmedLine = line.trim();
          if (!trimmedLine.startsWith("data: ")) continue;
          try {
            const data = JSON.parse(trimmedLine.slice(6));
            if (data.type === "chunk") {
              fullContent += data.content;
              const currentContent = fullContent;
              flushSync(() => {
                setProjectChats(prev => prev.map(c =>
                  c.id === activeChat!.id
                    ? { ...c, messages: c.messages.map(m =>
                        m.id === aiMsgId ? { ...m, content: currentContent } : m
                      )}
                    : c
                ));
              });
            } else if (data.type === "done") {
              const saveRes = await fetch("/api/messages", {
                method: "POST",
                headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
                body: JSON.stringify({
                  id: aiMsgId,
                  chat_id: activeChat!.id,
                  sender_id: "ai",
                  sender_name: "Meowlinette",
                  content: data.content,
                  type: "ai",
                  created_at: aiMsg.createdAt,
                }),
              });
              if (!saveRes.ok) {
                const errText = await saveRes.text();
                console.error("[stream] save AI msg 400:", errText);
              }
            } else if (data.type === "error") {
              setProjectChats(prev => prev.map(c =>
                c.id === activeChat!.id
                  ? { ...c, messages: c.messages.map(m =>
                      m.id === aiMsgId ? { ...m, content: data.content } : m
                    )}
                  : c
              ));
            }
          } catch (e) {
            continue;
          }
        }
      }
    } catch {
      setProjectChats(prev => prev.map(c => c.id === activeChat!.id ? { ...c, messages: c.messages.map(m => m.id === aiMsgId ? { ...m, content: "Sorry, something went wrong." } : m) } : c));
    } finally {
      setIsAiStreaming(false);
    }
  }

  function sendToUser(trimmed: string) {
    const msg = {
      id: createMessageId(),
      senderId: currentUser.id,
      senderName: currentUser.name,
      content: trimmed,
      createdAt: getCurrentTime(),
      createdAtRaw: new Date().toISOString(),
      type: "user" as const,
    };

    sendMessage(activeChat!.id, msg as ChatMessage);
    setInputValue("");
    setShowMentionDropdown(false);
    if (textareaRef.current) textareaRef.current.style.height = "auto";

    setTimeout(() => {
      messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
    }, 50);
  }

  async function handleSendMessage() {
    if (activeChat?.is_deleted) {
      setSendBlockedMsg(true);
      setTimeout(() => setSendBlockedMsg(false), 2000);
      return;
    }
    const trimmed = inputValue.trim();
    if (!trimmed || !activeChat) return;

    const checkBannedWords = (text: string) => {
      const lower = text.toLowerCase();
      return BANNED_WORDS.some(w =>
        new RegExp(`\\b${w.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')}\\b`).test(lower)
      );
    };
    if (checkBannedWords(trimmed)) {
      setModerationError("⚠️ Your message contains inappropriate content and was not sent.");
      setTimeout(() => setModerationError(null), 3000);
      return;
    }
    setModerationError(null);

    if (activeChat.id.startsWith("ai-")) {
      await sendToAI(trimmed);
    } else {
      sendToUser(trimmed);
    }
  }

  async function handleImageUpload(file: File) {
    if (!file.type.startsWith("image/")) return;
    if (file.size > 5 * 1024 * 1024) { alert("File exceeds 5 MB limit"); return; }
    if (!activeChat) return;
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

  if (!activeChat) return null;

  return (
    <div className="px-5 py-4 border-t border-gray-100">
      {dmBlockedMsg && (
        <div className="mx-4 mb-2 px-3 py-2 rounded-lg bg-red-50 border border-red-200 text-[11px] text-red-500 text-center">
          {dmBlockedMsg}
        </div>
      )}
      {moderationError && (
        <div className="px-4 py-2 text-red-500 text-xs mb-1">{moderationError}</div>
      )}
      {sendBlockedMsg && (
        <p className="text-[11px] text-red-400 mb-1 px-1">Message not sent — this account has been deactivated</p>
      )}
      {(() => {
        const names = (typingUsers.get(activeChatId) ?? [])
          .filter(n => n !== user?.displayName);
        if (!names.length) return null;

        const firstName = names[0];
        const typer = friends.find(f => f.displayName === firstName)
          ?? activeChat?.members?.find(m => m.displayName === firstName);

        return (
          <div className="flex items-center gap-2 mb-1.5 pl-1">
            <Avatar src={typer?.avatar} name={firstName} size="sm" />
            <div className="flex items-center gap-1 px-3 py-2 bg-gray-100 rounded-xl">
              <span className={`w-1.5 h-1.5 rounded-full animate-bounce ${isPrideMonth ? "bg-pink-400" : "bg-gray-400"}`} style={{ animationDelay: "0ms" }} />
              <span className={`w-1.5 h-1.5 rounded-full animate-bounce ${isPrideMonth ? "bg-purple-400" : "bg-gray-400"}`} style={{ animationDelay: "150ms" }} />
              <span className={`w-1.5 h-1.5 rounded-full animate-bounce ${isPrideMonth ? "bg-blue-400" : "bg-gray-400"}`} style={{ animationDelay: "300ms" }} />
            </div>
            {names.length > 1 && (
              <span className="text-[10px] text-gray-400">+{names.length - 1} more</span>
            )}
          </div>
        );
      })()}
      <input
        ref={fileInputRef}
        type="file"
        accept="image/jpeg,image/png,image/webp,image/gif"
        className="hidden"
        onChange={e => { const f = e.target.files?.[0]; if (f) handleImageUpload(f); e.target.value = ""; }}
      />

      <div className="relative flex items-center gap-2">
        {showMentionDropdown && filteredMentions.length > 0 && !activeChat.id.startsWith("dm-") && (
          <div className="absolute bottom-12 left-0 w-44 bg-white border border-gray-100 rounded-xl overflow-hidden shadow-lg z-10">
            {filteredMentions.map(u => (
              <button
                key={u.id}
                onClick={() => handleMentionSelect(u.name)}
                className="w-full text-left px-3 py-2 text-[12px] text-gray-700 hover:bg-gray-50 transition-colors flex items-center gap-2"
              >
                <span className="text-indigo-500 font-medium">@</span>
                {u.name}
              </button>
            ))}
          </div>
        )}

        <button
          onClick={() => fileInputRef.current?.click()}
          disabled={isUploading}
          className="w-9 h-9 rounded-lg border border-gray-200 flex items-center justify-center text-gray-400 hover:text-gray-700 hover:bg-gray-50 disabled:opacity-40 disabled:cursor-not-allowed transition-colors flex-shrink-0"
          title="Upload image"
        >
          {isUploading
            ? <span className="w-3.5 h-3.5 border-2 border-gray-300 border-t-gray-600 rounded-full animate-spin" />
            : <ImageIcon className="w-4 h-4" />
          }
        </button>

        <div className="flex-shrink-0">
          <button
            onClick={() => setShowEmojiPicker(o => !o)}
            className="w-9 h-9 rounded-lg border border-gray-200 flex items-center justify-center text-gray-400 hover:text-gray-700 hover:bg-gray-50 transition-colors"
            title="Emoji"
          >
            <Smile className="w-4 h-4" />
          </button>
        </div>

        {activeChat.id.startsWith("ai-") && (
          <select
            value={aiModel}
            onChange={e => setAiModel(e.target.value)}
            className="bg-gray-50 border border-gray-200 rounded-lg px-2 py-2 text-[11px] text-gray-600 outline-none cursor-pointer flex-shrink-0"
          >
            <option value="llama-3.3-70b-versatile">Smart</option>
            <option value="llama-3.1-8b-instant">Fast</option>
          </select>
        )}

        <textarea
          ref={textareaRef}
          value={inputValue}
          onChange={handleInputChange}
          onKeyDown={handleKeyDown}
          rows={1}
          disabled={isAiStreaming}
          className="flex-1 bg-gray-50 border border-gray-200 rounded-lg px-4 py-2.5 text-[12px] text-gray-800 outline-none focus:ring-1 focus:ring-indigo-300 focus:border-indigo-300 placeholder:text-gray-300 transition-all resize-none overflow-hidden disabled:opacity-50 disabled:cursor-not-allowed"
          placeholder={`Message ${activeChat.title}…`}
        />

        {isAiStreaming && (
          <span className="text-xs text-gray-400">Meowlinette is thinking...</span>
        )}
        <button
          onClick={handleSendMessage}
          disabled={!inputValue.trim() || isAiStreaming}
          className="w-9 h-9 rounded-lg bg-indigo-500 flex items-center justify-center text-white hover:bg-indigo-600 active:scale-90 transition-transform disabled:opacity-40 disabled:cursor-not-allowed"
        >
          <Send className="w-4 h-4" />
        </button>
      </div>
      {showEmojiPicker && (
        <div className="mt-2 p-3 bg-gray-50 border border-gray-100 rounded-xl max-h-40 overflow-y-auto">
          <div className="grid grid-cols-10 gap-1">
            {EMOJIS.map((e, i) => (
              <button key={i} type="button" onClick={() => {
                setInputValue(prev => prev + e);
                textareaRef.current?.focus();
              }} className="text-xl hover:bg-white hover:scale-110 transition-transform rounded-lg p-1.5">
                {e}
              </button>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}
