import { useRef, useState, useEffect } from "react";
import type { Dispatch, SetStateAction } from "react";
import { ArrowLeft, Cat, Search, Link, Video, Users, Trash2 } from "lucide-react";
import type { ProjectChat } from "../../context/ChatContext";
import { useChat } from "../../context/ChatContext";
import { useAuth } from "../../context/AuthContext";
import Avatar from "./Avatar";

type Props = {
  activeChat: ProjectChat;
  avatarCache: Record<string, string | null>;
  onSelectUser: (userId: string, displayName: string, avatar?: string) => void;
  onSelectAI: () => void;
  isMemberPanelOpen: boolean;
  setIsMemberPanelOpen: Dispatch<SetStateAction<boolean>>;
  showMsgSearch: boolean;
  setShowMsgSearch: Dispatch<SetStateAction<boolean>>;
  msgSearchValue: string;
  setMsgSearchValue: Dispatch<SetStateAction<string>>;
  onBack: () => void;
  activeMentionId: string | undefined;
  onJumpToMention: (messageId: string) => void;
};

export default function ChatHeader({
  activeChat, avatarCache,
  onSelectUser, onSelectAI,
  isMemberPanelOpen, setIsMemberPanelOpen,
  showMsgSearch, setShowMsgSearch,
  msgSearchValue, setMsgSearchValue,
  onBack, activeMentionId, onJumpToMention,
}: Props) {
  const { onlineUsers, friends } = useChat();
  const { token, user } = useAuth();

  const [links, setLinks] = useState<{ id: string; name: string; url: string }[]>([]);
  const [linksLoading, setLinksLoading] = useState(false);
  const [showLinksPopup, setShowLinksPopup] = useState(false);
  const [showLinkForm, setShowLinkForm] = useState(false);
  const [linkNameInput, setLinkNameInput] = useState("");
  const [linkUrlInput, setLinkUrlInput] = useState("");

  const msgSearchInputRef = useRef<HTMLInputElement | null>(null);

  useEffect(() => {
    if (showMsgSearch) msgSearchInputRef.current?.focus();
  }, [showMsgSearch]);

  async function fetchLinks(chatId = activeChat.id) {
    if (!token) return;
    setLinksLoading(true);
    try {
      const res = await fetch(`/api/links/${chatId}`, {
        headers: { Authorization: `Bearer ${token}` },
      });
      if (res.ok) setLinks(await res.json());
    } catch { /* ignore */ } finally {
      setLinksLoading(false);
    }
  }

  const dmOtherUserId = activeChat.id.startsWith("dm-")
    ? activeChat.participantIds.find(pid => pid !== String(user?.id)) ?? null
    : null;
  const dmOtherAvatar = dmOtherUserId
    ? (friends.find(f => String(f.id) === dmOtherUserId)?.avatar
      ?? activeChat.members?.find(m => String(m.id) === dmOtherUserId)?.avatar
      ?? avatarCache[dmOtherUserId]
      ?? null)
    : null;
  return (
    <>
      <div className="h-[52px] px-5 border-b border-gray-100 flex items-center gap-3 flex-shrink-0">
        <button
          onClick={onBack}
          className="md:hidden p-1.5 rounded-lg border border-gray-100 text-gray-400"
        >
          <ArrowLeft className="w-4 h-4" />
        </button>

        <div className="relative">
          {dmOtherUserId ? (
            <Avatar
              src={dmOtherAvatar}
              name={activeChat.title}
              size="md"
              onClick={() => onSelectUser(dmOtherUserId, activeChat.title, dmOtherAvatar ?? undefined)}
            />
          ) : (
            <div
              className="relative w-8 h-8 rounded-full bg-indigo-50 border border-indigo-100 flex items-center justify-center text-[11px] font-semibold text-indigo-600 cursor-pointer"
              onClick={() => activeChat.id.startsWith("ai-") && onSelectAI()}
            >
              {activeChat.id.startsWith("ai-") ? (
                <Cat className="w-4 h-4 text-indigo-400" />
              ) : <span className="text-indigo-400 font-bold">#</span>}
              {activeChat.id.startsWith("ai-") && (
                <span className="absolute bottom-0 right-0 w-2.5 h-2.5 rounded-full border-2 border-white bg-green-400" />
              )}
            </div>
          )}
          {dmOtherUserId && !activeChat.is_deleted && (
            <span className={`absolute bottom-0 right-0 w-2.5 h-2.5 rounded-full border-2 border-white ${(onlineUsers ?? []).some(u => u.id === dmOtherUserId) ? "bg-green-400" : "bg-gray-300"}`} />
          )}
        </div>

        <div>
          <p className="text-[13px] font-semibold text-gray-900">{activeChat.title}{activeChat.is_deleted && <span className="text-gray-400 text-[10px] ml-1 font-normal">(deactivated)</span>}</p>
        </div>

        <div className="ml-auto relative">
          <button
            onClick={() => { const next = !showLinksPopup; setShowLinksPopup(next); setShowLinkForm(false); setLinkNameInput(""); setLinkUrlInput(""); if (next) fetchLinks(); }}
            className={`w-[30px] h-[30px] flex items-center justify-center rounded-lg transition-colors ${showLinksPopup ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-900 hover:bg-gray-100"}`}
          >
            <Link size={15} />
          </button>

          {showLinksPopup && (
            <div className="absolute right-0 top-9 z-40 w-52 max-w-[80vw] bg-white border border-gray-200 rounded-xl shadow-lg p-3">
              <div className="flex items-center justify-between mb-2">
                <p className="text-[12px] font-semibold text-gray-800">Channel Links</p>
                <button
                  onClick={() => { setShowLinkForm(o => !o); setLinkNameInput(""); setLinkUrlInput(""); }}
                  className={`w-5 h-5 flex items-center justify-center rounded text-[14px] leading-none transition-colors ${showLinkForm ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-900 hover:bg-gray-100"}`}
                  title="Add link"
                >
                  +
                </button>
              </div>

              {linksLoading ? (
                <p className="text-[11px] text-gray-400">Loading…</p>
              ) : links.length > 0 ? (
                <ul className="space-y-1.5 max-h-40 overflow-y-auto">
                  {links.map(link => (
                    <li key={link.id} className="flex items-center gap-2">
                      <a
                        href={link.url}
                        target="_blank"
                        rel="noopener noreferrer"
                        className="flex-1 min-w-0 text-[12px] font-medium text-indigo-600 hover:underline truncate"
                      >
                        {link.name}
                      </a>
                      <button
                        onClick={async () => {
                          await fetch(`/api/links/${link.id}`, {
                            method: "DELETE",
                            headers: { Authorization: `Bearer ${token}` },
                          });
                          fetchLinks();
                        }}
                        className="shrink-0 text-gray-300 hover:text-red-400 transition-colors"
                      >
                        <Trash2 size={13} />
                      </button>
                    </li>
                  ))}
                </ul>
              ) : (
                <p className="text-[11px] text-gray-400">No links saved yet.</p>
              )}

              {showLinkForm && (
                <div className="border-t border-gray-100 mt-2 pt-2 space-y-2">
                  <input
                    value={linkNameInput}
                    onChange={e => setLinkNameInput(e.target.value)}
                    placeholder="Name"
                    className="w-full bg-gray-50 border border-gray-200 rounded-lg px-2 py-1.5 text-[12px] text-gray-800 outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300"
                  />
                  <input
                    value={linkUrlInput}
                    onChange={e => setLinkUrlInput(e.target.value)}
                    placeholder="URL"
                    className="w-full bg-gray-50 border border-gray-200 rounded-lg px-2 py-1.5 text-[12px] text-gray-800 outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300"
                  />
                  <button
                    onClick={async () => {
                      if (!linkNameInput.trim() || !linkUrlInput.trim()) return;
                      await fetch(`/api/links/${activeChat.id}`, {
                        method: "POST",
                        headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
                        body: JSON.stringify({ name: linkNameInput.trim(), url: linkUrlInput.trim() }),
                      });
                      setLinkNameInput("");
                      setLinkUrlInput("");
                      setShowLinkForm(false);
                      fetchLinks();
                    }}
                    disabled={!linkNameInput.trim() || !linkUrlInput.trim()}
                    className="w-full text-[12px] font-medium bg-gray-900 text-white rounded-lg py-1.5 hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors"
                  >
                    Add
                  </button>
                </div>
              )}
            </div>
          )}
        </div>

        <button
          onClick={() => window.open("https://meet.google.com/new", "_blank")}
          className="w-[30px] h-[30px] flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors"
        >
          <Video size={15} />
        </button>

        {activeChat.id.startsWith("workspace_") && (
          <button
            onClick={() => setIsMemberPanelOpen(o => !o)}
            className={`w-[30px] h-[30px] flex items-center justify-center rounded-lg transition-colors ${isMemberPanelOpen ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-900 hover:bg-gray-100"}`}
          >
            <Users size={15} />
          </button>
        )}

        <button
          onClick={() => { setShowMsgSearch(o => !o); setMsgSearchValue(""); }}
          className={`w-[30px] h-[30px] flex items-center justify-center rounded-lg transition-colors ${showMsgSearch ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-900 hover:bg-gray-100"}`}
          title="Search messages"
        >
          <Search size={15} />
        </button>
      </div>

      {showMsgSearch && (
        <div className="px-5 py-2 border-b border-gray-100 flex items-center gap-2">
          <Search className="w-3.5 h-3.5 text-gray-300 shrink-0" />
          <input
            ref={msgSearchInputRef}
            value={msgSearchValue}
            onChange={e => setMsgSearchValue(e.target.value)}
            placeholder="Search in messages…"
            className="flex-1 bg-transparent text-[12px] text-gray-700 outline-none placeholder:text-gray-300"
          />
          {msgSearchValue && (
            <span className="text-[11px] text-gray-400 shrink-0">
              {activeChat.messages.filter(m => m.content.toLowerCase().includes(msgSearchValue.trim().toLowerCase())).length} result(s)
            </span>
          )}
          <button
            onClick={() => { setShowMsgSearch(false); setMsgSearchValue(""); }}
            className="text-gray-300 hover:text-gray-600 text-[16px] leading-none"
          >
            ×
          </button>
        </div>
      )}

      {activeMentionId && (
        <div className="px-5 pt-3">
          <button
            onClick={() => onJumpToMention(activeMentionId)}
            className="w-full flex items-center justify-between px-4 py-2 bg-red-500 text-white text-[11px] font-medium rounded-lg hover:bg-red-600 transition-colors"
          >
            <span>@ Someone mentioned you</span>
            <span>Jump ↓</span>
          </button>
        </div>
      )}
    </>
  );
}
