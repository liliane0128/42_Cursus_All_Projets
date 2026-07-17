import { useState, useEffect, type Dispatch, type SetStateAction } from "react";
import { Cat, Eye, EyeOff, Search } from "lucide-react";
import { useChat, type ProjectChat } from "../../context/ChatContext";
import { useAuth } from "../../context/AuthContext";
import { getLastMessage, getLastMessageTime } from "./utils";
import Avatar from "./Avatar";

type Props = {
  activeChatId: string | null;
  sidebarTab: "channels" | "friends";
  setSidebarTab: (tab: "channels" | "friends") => void;
  avatarCache: Record<string, string | null>;
  sentRequests: Set<number>;
  setSentRequests: Dispatch<SetStateAction<Set<number>>>;
  onSelectChat: (chatId: string) => void;
  onSelectUser: (id: string, name: string, avatar?: string) => void;
  onRemoveFriend: (target: { id: number; name: string }) => void;
  onMessageFriend: (chatId: string) => void;
  isChatOpenOnMobile: boolean;
};

export default function ChatListPanel({
  activeChatId, sidebarTab, setSidebarTab,
  avatarCache, sentRequests, setSentRequests,
  onSelectChat, onSelectUser, onRemoveFriend, onMessageFriend,
  isChatOpenOnMobile,
}: Props) {
  const {
    projectChats, friends, friendRequests, onlineUsers,
    sendFriendRequest, acceptFriendRequest, declineFriendRequest,
    refreshChannels, mentionedChats,
  } = useChat();
  const { user, token } = useAuth();

  const currentUserId = user ? String(user.id) : "";
  const currentUser   = user ? { id: String(user.id), name: user.displayName || user.login } : { id: "", name: "" };

  const [searchValue,         setSearchValue]         = useState("");
  const [friendSearch,        setFriendSearch]        = useState("");
  const [friendSearchResults, setFriendSearchResults] = useState<{ id: number; login: string; displayName: string; avatar?: string; friendshipStatus: "friend" | "pending" | "none" }[]>([]);
  const [ignoredRequestIds,   setIgnoredRequestIds]   = useState<Set<number>>(new Set());
  const [hiddenChatIds,       setHiddenChatIds]       = useState<Set<string>>(() => {
    try {
      const raw = localStorage.getItem("hidden_chat_ids");
      return raw ? new Set(JSON.parse(raw)) : new Set();
    } catch { return new Set(); }
  });
  const [showHidden, setShowHidden] = useState(false);

  useEffect(() => {
    localStorage.setItem("hidden_chat_ids", JSON.stringify([...hiddenChatIds]));
  }, [hiddenChatIds]);

  useEffect(() => {
    if (!friendSearch.trim() || !token) { setFriendSearchResults([]); return; }
    const t = setTimeout(async () => {
      try {
        const res = await fetch(`/api/users/search?q=${encodeURIComponent(friendSearch)}`, {
          headers: { Authorization: `Bearer ${token}` },
        });
        if (res.ok) setFriendSearchResults(await res.json());
      } catch { /* ignore */ }
    }, 300);
    return () => clearTimeout(t);
  }, [friendSearch, token]);

  const filteredChats = projectChats.filter(c => c.title.toLowerCase().includes(searchValue.toLowerCase()));
  const visibleChats  = filteredChats.filter(c => !hiddenChatIds.has(c.id));
  const hiddenChats   = filteredChats.filter(c => hiddenChatIds.has(c.id));

  async function ignoreFriendRequest(userId: number) {
    await declineFriendRequest(userId);
    setIgnoredRequestIds(prev => new Set(prev).add(userId));
  }

  function renderChatItem(chat: ProjectChat, opts: { hidden: boolean; showUnread?: boolean; showOnlineDot?: boolean }) {
    const isActive   = chat.id === activeChatId;
    const hasMention = mentionedChats.has(chat.id);
    const avatarSize = opts.hidden ? "w-8 h-8" : "w-9 h-9";
    const iconSize   = opts.hidden ? "w-3.5 h-3.5" : "w-4 h-4";
    const rowPadding = opts.hidden ? "py-2" : "py-2.5";
    const rowOpacity = opts.hidden ? "opacity-50 hover:opacity-100" : "";

    return (
      <div key={chat.id} className="group relative">
        <button
          onClick={() => onSelectChat(chat.id)}
          className={`w-full flex items-center gap-3 px-3 ${rowPadding} text-left transition-colors hover:scale-[1.01] transition-transform group-hover:pr-7 ${rowOpacity} ${isActive ? "border-l-2 border-indigo-400 bg-gray-50 pl-[10px]" : "hover:bg-gray-50 border-l-2 border-transparent pl-[10px]"}`}
        >
          <div className="relative shrink-0">
            {chat.id.startsWith("dm-") ? (() => {
              const otherId = chat.participantIds.find(pid => pid !== currentUserId);
              const other   = friends.find(f => String(f.id) === otherId);
              const avatar  = other?.avatar ?? (otherId ? avatarCache[otherId] : null) ?? null;
              return (
                <Avatar
                  src={avatar}
                  name={chat.title}
                  fallback={chat.avatarLabel}
                  size="md"
                  className="border border-indigo-100"
                />
              );
            })() : (
              <div className={`${avatarSize} rounded-full overflow-hidden bg-indigo-50 border border-indigo-100 flex items-center justify-center text-[11px] font-semibold text-indigo-600`}>
                {chat.id.startsWith("ai-")
                  ? <Cat className={`${iconSize} text-indigo-400`} />
                  : <span className="text-indigo-400 font-bold">#</span>
                }
              </div>
            )}
            {opts.showOnlineDot && chat.id.startsWith("dm-") && !chat.is_deleted && (() => {
              const otherId = chat.participantIds.find(pid => pid !== currentUserId);
              const online  = otherId && (onlineUsers ?? []).some(u => u.id === otherId);
              return <span className={`absolute bottom-0 right-0 w-2.5 h-2.5 rounded-full border-2 border-white ${online ? "bg-green-400" : "bg-gray-300"}`} />;
            })()}
            {opts.showOnlineDot && chat.id.startsWith("ai-") && (
              <span className="absolute bottom-0 right-0 w-2.5 h-2.5 rounded-full border-2 border-white bg-green-400" />
            )}
          </div>

          <div className="min-w-0 flex-1">
            {opts.hidden ? (
              <p className="text-[12px] text-gray-600 truncate">{chat.title}</p>
            ) : (
              <>
                <div className="flex items-center justify-between gap-2">
                  <p className="text-[12px] font-medium text-gray-900 truncate">
                    {chat.title}{chat.is_deleted && <span className="text-gray-400 text-[10px] ml-1">(deactivated)</span>}
                  </p>
                  <span className="text-[10px] text-gray-400 shrink-0">{getLastMessageTime(chat)}</span>
                </div>
                <div className="flex items-center justify-between gap-2 mt-0.5">
                  {hasMention ? (
                    <span className="text-[11px] text-red-400 font-medium">@ Someone mentioned you</span>
                  ) : (
                    <p className="text-[11px] text-gray-400 truncate">{getLastMessage(chat, currentUser)}</p>
                  )}
                  {opts.showUnread && chat.unreadCount > 0 && (
                    <span className="min-w-[18px] h-[18px] px-1 rounded-full bg-gray-900 text-white text-[9px] font-semibold flex items-center justify-center shrink-0">
                      {chat.unreadCount}
                    </span>
                  )}
                </div>
              </>
            )}
          </div>
        </button>
        <button
          onClick={e => {
            e.stopPropagation();
            if (opts.hidden) {
              setHiddenChatIds(prev => { const next = new Set(prev); next.delete(chat.id); return next; });
            } else {
              setHiddenChatIds(prev => new Set(prev).add(chat.id));
            }
          }}
          className="absolute right-2 top-1/2 -translate-y-1/2 opacity-0 group-hover:opacity-100 transition-opacity p-1 rounded hover:bg-gray-200"
          title={opts.hidden ? "Unhide chat" : "Hide chat"}
        >
          {opts.hidden
            ? <Eye className="w-3.5 h-3.5 text-gray-400" />
            : <EyeOff className="w-3.5 h-3.5 text-gray-400" />
          }
        </button>
      </div>
    );
  }

  return (
    <div className={`w-full md:w-72 md:flex-shrink-0 flex flex-col bg-white border border-gray-100 rounded-xl overflow-hidden ${isChatOpenOnMobile ? "hidden md:flex" : "flex"}`}>

      {/* ── Tab switcher ── */}
      <div className="flex flex-shrink-0 border-b border-gray-100">
        <button
          onClick={() => setSidebarTab("channels")}
          className={`flex-1 py-2.5 text-[12px] font-medium transition-colors ${sidebarTab === "channels" ? "text-gray-900 border-b-2 border-gray-900 -mb-px" : "text-gray-400 hover:text-gray-600"}`}
        >
          Channels
        </button>
        <button
          onClick={() => setSidebarTab("friends")}
          className={`flex-1 py-2.5 text-[12px] font-medium transition-colors inline-flex items-center justify-center gap-1 ${sidebarTab === "friends" ? "text-gray-900 border-b-2 border-gray-900 -mb-px" : "text-gray-400 hover:text-gray-600"}`}
        >
          Friends
          {friendRequests.filter(r => !ignoredRequestIds.has(r.id)).length > 0 && (
            <span className="inline-flex items-center justify-center min-w-[14px] h-[14px] px-0.5 rounded-full bg-red-500 text-white text-[8px] font-semibold">
              {friendRequests.filter(r => !ignoredRequestIds.has(r.id)).length}
            </span>
          )}
        </button>
      </div>

      {/* ── Channels tab ── */}
      {sidebarTab === "channels" && (
        <>
          <div className="p-3 border-b border-gray-100 flex-shrink-0">
            <div className="relative">
              <Search className="w-3.5 h-3.5 text-gray-300 absolute left-3 top-1/2 -translate-y-1/2" />
              <input
                value={searchValue}
                onChange={e => setSearchValue(e.target.value)}
                className="w-full bg-gray-50 border border-gray-100 rounded-lg pl-8 pr-3 py-2 text-[12px] text-gray-700 outline-none placeholder:text-gray-300"
                placeholder="Search chats"
              />
            </div>
          </div>

          <div className="flex-1 overflow-y-auto py-1">
            {visibleChats.map(chat => renderChatItem(chat, { hidden: false, showUnread: true, showOnlineDot: true }))}

            {hiddenChats.length > 0 && (
              <div className="border-t border-gray-100 mt-1">
                <button
                  onClick={() => setShowHidden(prev => !prev)}
                  className="w-full flex items-center gap-1.5 px-3 py-2 text-left text-[11px] text-gray-400 hover:text-gray-600 hover:bg-gray-50 transition-colors"
                >
                  <Eye className="w-3 h-3" />
                  Hidden ({hiddenChats.length})
                </button>
                {showHidden && hiddenChats.map(chat => renderChatItem(chat, { hidden: true, showUnread: false, showOnlineDot: false }))}
              </div>
            )}
          </div>
        </>
      )}

      {/* ── Friends tab ── */}
      {sidebarTab === "friends" && (
        <div className="flex-1 overflow-y-auto flex flex-col">

          {/* User search */}
          <div className="p-3 border-b border-gray-100 flex-shrink-0">
            <div className="relative">
              <Search className="w-3.5 h-3.5 text-gray-300 absolute left-3 top-1/2 -translate-y-1/2" />
              <input
                value={friendSearch}
                onChange={e => setFriendSearch(e.target.value)}
                className="w-full bg-gray-50 border border-gray-100 rounded-lg pl-8 pr-3 py-2 text-[12px] text-gray-700 outline-none placeholder:text-gray-300"
                placeholder="Search or add friends"
              />
            </div>
            {friendSearchResults.length > 0 && (
              <div className="mt-2 space-y-0.5">
                {friendSearchResults.map(u => (
                  <div key={u.id} className="flex items-center gap-2 px-2 py-1.5 rounded-lg hover:bg-gray-50">
                    <div className="w-7 h-7 rounded-full bg-indigo-50 border border-indigo-100 flex items-center justify-center text-[11px] font-semibold text-indigo-600 shrink-0">
                      {u.displayName[0]?.toUpperCase() ?? "?"}
                    </div>
                    <p className="flex-1 text-[12px] text-gray-700 truncate">{u.displayName}</p>
                    <button
                      disabled={u.friendshipStatus !== "none"}
                      onClick={async () => {
                        if (u.friendshipStatus !== "none") return;
                        try { await sendFriendRequest(u.id); } catch { /* ignore 409 */ }
                        setSentRequests(prev => new Set(prev).add(u.id));
                      }}
                      className={`text-[11px] font-medium shrink-0 transition-colors ${u.friendshipStatus !== "none" || sentRequests.has(u.id) ? "text-gray-300 cursor-not-allowed" : "text-indigo-600 hover:text-indigo-800"}`}
                    >
                      {u.friendshipStatus === "friend" ? "Friends" : u.friendshipStatus === "pending" || sentRequests.has(u.id) ? "Sent" : "Add"}
                    </button>
                  </div>
                ))}
              </div>
            )}
          </div>

          {/* Pending requests */}
          {friendRequests.filter(r => !ignoredRequestIds.has(r.id)).length > 0 && (
            <div className="px-3 pt-3 flex-shrink-0">
              <p className="text-[10px] font-semibold text-gray-400 uppercase tracking-wider mb-1.5 px-2">Friend Requests</p>
              <div className="space-y-0.5">
                {friendRequests.filter(r => !ignoredRequestIds.has(r.id)).map(r => (
                  <div key={r.id} className="flex items-center gap-2 px-2 py-1.5 rounded-lg hover:bg-gray-50">
                    <div className="w-7 h-7 rounded-full bg-indigo-50 border border-indigo-100 flex items-center justify-center text-[11px] font-semibold text-indigo-600 shrink-0">
                      {r.displayName[0]?.toUpperCase() ?? "?"}
                    </div>
                    <p className="flex-1 text-[12px] text-gray-700 truncate">{r.displayName}</p>
                    <button
                      onClick={() => acceptFriendRequest(r.id)}
                      className="text-[11px] text-green-600 hover:text-green-800 font-medium shrink-0 transition-colors"
                    >
                      Accept
                    </button>
                    <button
                      onClick={() => ignoreFriendRequest(r.id)}
                      className="text-[11px] text-gray-400 hover:text-red-400 font-medium shrink-0 transition-colors"
                    >
                      Ignore
                    </button>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* Friends list */}
          <div className="px-3 pt-3 pb-3">
            {friends.length === 0 ? (
              <p className="text-[11px] text-gray-400 px-2">No friends yet</p>
            ) : (
              <div className="space-y-0.5">
                {(() => {
                  const sortedFriends = [...friends].sort((a, b) => {
                    const aOnline = (onlineUsers ?? []).some(u => u.id === String(a.id));
                    const bOnline = (onlineUsers ?? []).some(u => u.id === String(b.id));
                    if (aOnline !== bOnline) return aOnline ? -1 : 1;
                    return (a.displayName ?? "").localeCompare(b.displayName ?? "");
                  });
                  const onlineFriendsCount = sortedFriends.filter(f => !f.is_deleted && (onlineUsers ?? []).some(u => u.id === String(f.id))).length;
                  return (<>
                  <p className="text-[10px] font-semibold text-gray-400 uppercase tracking-wider mb-2 px-2">
                    Friends · <span className="text-green-500">{onlineFriendsCount} online</span>
                  </p>
                  {sortedFriends.map(f => (
                    <div key={f.id} className="group flex items-center gap-2 px-2 py-1.5 rounded-lg hover:bg-gray-50 hover:scale-[1.01] transition-transform">
                      <div
                        className="relative shrink-0 cursor-pointer"
                        onClick={() => onSelectUser(String(f.id), f.displayName, f.avatar ?? undefined)}
                      >
                        <Avatar src={f.avatar} name={f.displayName} size="sm" className="border border-indigo-100" />
                        {!f.is_deleted && (
                          <span className={`absolute -bottom-0.5 -right-0.5 w-2.5 h-2.5 rounded-full border-2 border-white ${(onlineUsers ?? []).some(u => u.id === String(f.id)) ? "bg-green-400" : "bg-gray-300"}`} />
                        )}
                      </div>
                      <p
                        className="flex-1 text-[12px] text-gray-700 truncate cursor-pointer hover:text-gray-900"
                        onClick={() => onSelectUser(String(f.id), f.displayName, f.avatar ?? undefined)}
                      >
                        {f.displayName}{f.is_deleted && <span className="text-gray-400 text-[10px] ml-1 font-normal">(deactivated)</span>}
                      </p>
                      <button
                        onClick={async () => {
                          try {
                            const res = await fetch(`/api/dm/${f.id}`, {
                              method: "POST",
                              headers: { Authorization: `Bearer ${token}` },
                            });
                            if (!res.ok) return;
                            const { chat_id } = await res.json() as { chat_id: string };
                            await refreshChannels();
                            onMessageFriend(chat_id);
                          } catch { /* ignore */ }
                        }}
                        className="text-[11px] text-indigo-500 hover:text-indigo-700 font-medium shrink-0 opacity-0 group-hover:opacity-100 transition-opacity mr-1"
                      >
                        Message
                      </button>
                      <button
                        onClick={() => onRemoveFriend({ id: f.id, name: f.displayName })}
                        className="text-[11px] text-gray-300 hover:text-red-400 font-medium shrink-0 opacity-0 group-hover:opacity-100 transition-opacity"
                      >
                        Remove
                      </button>
                    </div>
                  ))}
                  </>);
                })()}
              </div>
            )}
          </div>

        </div>
      )}

    </div>
  );
}
