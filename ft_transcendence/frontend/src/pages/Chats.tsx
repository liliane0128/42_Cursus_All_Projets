import Logo42 from "../components/Logo42";
import UserMenu from "../components/UserMenu";
import {
  LayoutDashboard, FolderKanban, CheckSquare, MessageSquare,
} from "lucide-react";
import { useRef, useState, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import NotificationPanel from '../components/NotificationPanel';
import { useChat, MOCK_USERS, type ProjectChat, type Friend } from '../context/ChatContext';
import { useAuth } from '../context/AuthContext';
import NavItem from './Chats/NavItem';
import ImageLightbox from './Chats/ImageLightbox';
import RemoveFriendDialog from './Chats/RemoveFriendDialog';
import MemberPanel from './Chats/MemberPanel';
import ChatListPanel from './Chats/ChatListPanel';
import ChatHeader from './Chats/ChatHeader';
import MessageList from './Chats/MessageList';
import MessageInput from './Chats/MessageInput';
import UserCardModal, { type UserCardData } from './Chats/UserCardModal';

// ── Types ─────────────────────────────────────────────────────────────────────

type MockUser = {
  id: string;
  name: string;
};

export default function Chats() {
  const {
    projectChats, onlineUsers, isSocketConnected,
    mentionedChats, setMentionedChats, openChat,
    friends, friendRequests, sendFriendRequest, acceptFriendRequest, removeFriend,
    setProjectChats, refreshChannels, sendEditMessage,
  } = useChat();

  const { user, token } = useAuth();
  const navigate = useNavigate();
  const location = useLocation();

  const [activeChatId,        setActiveChatId]        = useState<string | null>(() => {
    return sessionStorage.getItem("activeChatId");
  });
  const setActiveChat = (id: string | null) => {
    if (id) sessionStorage.setItem("activeChatId", id);
    else sessionStorage.removeItem("activeChatId");
    setActiveChatId(id);
  };
  useEffect(() => {
    const savedId = activeChatId;
    const savedExists = projectChats.some(c => c.id === savedId);
    if (savedId !== null && savedExists) return;
    const firstChannel = projectChats.find(c => !c.id.startsWith("ai-"));
    const fallback = projectChats.find(c => c.id.startsWith("ai-"));
    const target = firstChannel ?? fallback;
    if (target) setActiveChat(target.id);
  }, [projectChats]);
  const [isChatOpenOnMobile,  setIsChatOpenOnMobile]  = useState(false);
  const [highlightedMessageId,setHighlightedMessageId]= useState<string | null>(null);
  const [isMemberPanelOpen,   setIsMemberPanelOpen]   = useState(false);
  const [sidebarTab,          setSidebarTab]          = useState<"channels" | "friends">("channels");
  const [sentRequests,        setSentRequests]        = useState<Set<number>>(new Set());
  const [selectedUser,        setSelectedUser]        = useState<UserCardData | null>(null);

  const [lightboxSrc,         setLightboxSrc]         = useState<string | null>(null);
  const [showMsgSearch,       setShowMsgSearch]       = useState(false);
  const [msgSearchValue,      setMsgSearchValue]      = useState("");
  const [avatarCache,         setAvatarCache]         = useState<Record<string, string | null>>({});
  const [deletedUserIds,      setDeletedUserIds]      = useState<Set<string>>(new Set());

  const [removeFriendTarget,  setRemoveFriendTarget]  = useState<{ id: number; name: string } | null>(null);

  const messagesEndRef = useRef<HTMLDivElement | null>(null);

  const currentUser = user ? { id: String(user.id), name: user.displayName || user.login } : MOCK_USERS[0];
  const activeChat  = projectChats.find(c => c.id === activeChatId);

  async function fetchAvatar(senderId: string) {
    if (senderId in avatarCache) return;
    try {
      const res = await fetch(`/api/users/by-id/${senderId}`, {
        headers: { Authorization: `Bearer ${token}` },
      });
      const data = res.ok ? await res.json() : null;
      setAvatarCache(prev => ({ ...prev, [senderId]: data?.avatar ?? null }));
      if (data?.is_deleted) {
        setDeletedUserIds(prev => new Set(prev).add(senderId));
      }
    } catch {
      setAvatarCache(prev => ({ ...prev, [senderId]: null }));
    }
  }

  function handleOpenChat(chatId: string) {
    setActiveChat(chatId);
    setIsChatOpenOnMobile(true);
    openChat(chatId);
    setTimeout(() => {
      messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
    }, 50);
  }

  useEffect(() => {
    const openChatId = (location.state as { openChatId?: string } | null)?.openChatId;
    if (openChatId) handleOpenChat(openChatId);
  }, [location.state]);

  useEffect(() => {
    messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
  }, [activeChat?.messages]);

  useEffect(() => {
    if (!activeChat) return;
    const senderIds = [...new Set(
      activeChat.messages
        .filter(m => m.senderId !== String(user?.id) && m.senderId !== "ai" && m.senderId !== "meow" && !friends.find(f => String(f.id) === m.senderId))
        .map(m => m.senderId)
    )];
    senderIds.forEach(id => fetchAvatar(id));
  }, [activeChat?.messages, friends]);

  useEffect(() => {
    const otherIds = [...new Set(
      projectChats
        .filter(c => c.id.startsWith("dm-"))
        .map(c => c.participantIds.find(pid => pid !== String(user?.id)))
        .filter((id): id is string => !!id && !friends.find(f => String(f.id) === id))
    )];
    otherIds.forEach(id => fetchAvatar(id));
  }, [projectChats, friends]);

  useEffect(() => {
    if (!activeChat?.id.startsWith("dm-")) return;
    const dmOtherUserId = activeChat.participantIds.find(pid => pid !== String(user?.id)) ?? null;
    if (!dmOtherUserId) return;
    const inFriends = friends.some(f => String(f.id) === dmOtherUserId);
    if (!inFriends && !(dmOtherUserId in avatarCache)) {
      fetchAvatar(dmOtherUserId);
    }
  }, [activeChat?.id, friends, avatarCache]);

  useEffect(() => {
    function onKeyDown(e: KeyboardEvent) {
      if (e.key === "Escape") {
        setLightboxSrc(null);
        setShowMsgSearch(false);
        setMsgSearchValue("");
      }
    }
    window.addEventListener("keydown", onKeyDown);
    return () => window.removeEventListener("keydown", onKeyDown);
  }, []);

  useEffect(() => {
    setShowMsgSearch(false);
    setMsgSearchValue("");
  }, [activeChatId]);

  useEffect(() => {
    const q = msgSearchValue.trim().toLowerCase();
    if (!q || !activeChat) return;
    const first = activeChat.messages.find(m => m.content.toLowerCase().includes(q));
    if (first) document.getElementById(`msg-${first.id}`)?.scrollIntoView({ behavior: "smooth", block: "center" });
  }, [msgSearchValue]);

  function isOnline(chat: ProjectChat) {
    if (chat.id.startsWith("ai-")) return true;
    return chat.participantIds.some(pid => pid !== currentUser.id && (onlineUsers ?? []).some(u => u.id === pid));
  }

  function jumpToMention(messageId: string) {
    document.getElementById(`msg-${messageId}`)?.scrollIntoView({ behavior: "smooth", block: "center" });
    setHighlightedMessageId(messageId);
    setTimeout(() => setHighlightedMessageId(null), 2000);
    setMentionedChats(prev => { const n = new Map(prev); n.delete(activeChatId); return n; });
  }

  async function handleSelectUser(userId: string, displayName: string, avatar?: string) {
    setSelectedUser({ id: userId, displayName, avatar, is_deleted: friends.find(fr => String(fr.id) === userId)?.is_deleted });
    try {
      const knownFriend = friends.find(fr => String(fr.id) === userId);
      let resolvedLogin = knownFriend?.login;
      if (!resolvedLogin) {
        const searchRes = await fetch(`/api/users/search?q=${encodeURIComponent(displayName)}`, {
          headers: { Authorization: `Bearer ${token}` },
        });
        if (searchRes.ok) {
          const results = await searchRes.json() as Array<{ id: number; login: string; displayName: string }>;
          const match = results.find(u => String(u.id) === userId) ?? results[0];
          resolvedLogin = match?.login;
        }
      }
      if (resolvedLogin) {
        const res = await fetch(`/api/users/${resolvedLogin}`, { headers: { Authorization: `Bearer ${token}` } });
        if (res.ok) {
          const fullUser = await res.json();
          setSelectedUser(prev => prev ? { ...prev, login: fullUser.login, email: fullUser.email, bio: fullUser.bio, status: knownFriend?.status, is_deleted: fullUser.is_deleted } : prev);
        }
      }
    } catch { /* keep basic info on error */ }
  }

  if (!activeChat) return (
    <div className="flex-1 flex items-center justify-center text-gray-400 text-sm">
      Select a channel to start chatting
    </div>
  );

  const activeMentionId = mentionedChats.get(activeChat.id);

  return (
    <>
    <div className="flex h-screen bg-[#f5f5f3] font-sans text-gray-900 overflow-hidden">

      {/* ── Sidebar ── */}
      <aside className="w-52 flex-shrink-0 flex flex-col bg-white border-r border-gray-100">
        <div className="h-[52px] px-4 flex items-center gap-2.5 border-b border-gray-100 flex-shrink-0">
          <div className="w-[26px] h-[26px] rounded-[6px] bg-gray-900 flex items-center justify-center flex-shrink-0 hover:rotate-12 transition-transform duration-300 cursor-pointer">
            <Logo42 size={12} />
          </div>
          <span className="text-[13px] font-semibold tracking-tight">Collab42</span>
        </div>
        <nav className="flex-1 px-2 py-3 space-y-0.5">
          <NavItem icon={LayoutDashboard} label="Dashboard" to="/dashboard" />
          <NavItem icon={FolderKanban}    label="Workspace" to="/workspace"  />
          <NavItem icon={CheckSquare}     label="Tasks"     to="/tasks"      />
          <NavItem icon={MessageSquare}   label="Chats"     active           />
        </nav>
      </aside>

      {/* ── Main ── */}
      <div className="flex-1 flex flex-col min-w-0">

        <header className="h-[52px] bg-white border-b border-gray-100 flex items-center justify-between px-6 flex-shrink-0">
          <div>
            <p className="text-[14px] font-semibold">Chats</p>
            <p className="text-[11px] text-gray-400 mt-0.5">Team chat & collaboration</p>
          </div>

          <div className="flex items-center gap-1">
            <NotificationPanel />
            <UserMenu />
          </div>
        </header>

        <main className="flex-1 flex gap-3 px-6 py-5 min-h-0 overflow-x-auto">

          {/* Chat list */}
          <ChatListPanel
            activeChatId={activeChatId}
            sidebarTab={sidebarTab}
            setSidebarTab={setSidebarTab}
            avatarCache={avatarCache}
            sentRequests={sentRequests}
            setSentRequests={setSentRequests}
            onSelectChat={handleOpenChat}
            onSelectUser={handleSelectUser}
            onRemoveFriend={setRemoveFriendTarget}
            onMessageFriend={chatId => {
              setSidebarTab("channels");
              setActiveChat(chatId);
              openChat(chatId);
            }}
            isChatOpenOnMobile={isChatOpenOnMobile}
          />

          {/* Chat messages */}
          <div className={`flex-1 flex flex-row bg-white border border-gray-100 rounded-xl overflow-hidden min-h-0 ${isChatOpenOnMobile ? "flex" : "hidden md:flex"}`}>

            <div className="flex-1 flex flex-col min-w-0 min-w-[300px]">

            <ChatHeader
              activeChat={activeChat}
              avatarCache={avatarCache}
              onSelectUser={handleSelectUser}
              onSelectAI={() => setSelectedUser({ id: "meow", displayName: "Meowlinette", login: "meowlinette", email: "", bio: "Your AI assistant — always online, always ready to help. Ask me to create tasks, answer questions, or just chat. @Meow to summon me anywhere.", avatar: undefined, status: "online" })}
              isMemberPanelOpen={isMemberPanelOpen}
              setIsMemberPanelOpen={setIsMemberPanelOpen}
              showMsgSearch={showMsgSearch}
              setShowMsgSearch={setShowMsgSearch}
              msgSearchValue={msgSearchValue}
              setMsgSearchValue={setMsgSearchValue}
              onBack={() => setIsChatOpenOnMobile(false)}
              activeMentionId={activeMentionId}
              onJumpToMention={jumpToMention}
            />

            <MessageList
              activeChat={activeChat}
              currentUser={currentUser}
              avatarCache={avatarCache}
              deletedUserIds={deletedUserIds}
              msgSearchValue={msgSearchValue}
              highlightedMessageId={highlightedMessageId}
              onSelectUser={handleSelectUser}
              onSelectAI={() => setSelectedUser({ id: "meow", displayName: "Meowlinette", login: "meowlinette", email: "", bio: "Your AI assistant — always online, always ready to help. Ask me to create tasks, answer questions, or just chat. @Meow to summon me anywhere.", avatar: undefined, status: "online" })}
              onImageClick={setLightboxSrc}
              messagesEndRef={messagesEndRef}
            />

            <MessageInput
              activeChatId={activeChatId}
              messagesEndRef={messagesEndRef}
            />

            </div>{/* end flex-1 flex-col */}

            {/* Member panel — fixed overlay on mobile, inline on md+ */}
            {activeChat && activeChat.id.startsWith("workspace_") && (
              <MemberPanel
                activeChat={activeChat}
                onlineUsers={onlineUsers ?? []}
                friends={friends}
                onSelectUser={handleSelectUser}
                isOpen={isMemberPanelOpen}
                onClose={() => setIsMemberPanelOpen(false)}
              />
            )}

          </div>
        </main>
      </div>
    </div>

    <UserCardModal
      selectedUser={selectedUser}
      setSelectedUser={setSelectedUser}
      sentRequests={sentRequests}
      setSentRequests={setSentRequests}
      onOpenDM={chatId => {
        setSidebarTab("channels");
        setActiveChat(chatId);
        openChat(chatId);
      }}
      onRemoveFriend={setRemoveFriendTarget}
    />

    <RemoveFriendDialog
      target={removeFriendTarget}
      onConfirm={id => {
        removeFriend(id);
        setAvatarCache(prev => { const next = { ...prev }; delete next[String(id)]; return next; });
        setProjectChats(prev => prev.filter(c =>
          !(c.id.startsWith("dm-") && c.participantIds.includes(String(id)))
        ));
        setRemoveFriendTarget(null);
      }}
      onCancel={() => setRemoveFriendTarget(null)}
    />

    <ImageLightbox src={lightboxSrc} onClose={() => setLightboxSrc(null)} />
    </>
  );
}