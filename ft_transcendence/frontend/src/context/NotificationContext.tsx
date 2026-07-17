import { createContext, useContext, useState, useEffect, useRef, useCallback } from "react";
import { useAuth } from "./AuthContext";
import {
  getPendingInvites, getWsNotifications, markWsNotificationsRead,
  getWsUnreadCount, clearWsNotifications,
  type PendingInvite, type WorkspaceNotif,
} from "../api";

type AppNotificationType = "message" | "deadline" | "friend_request" | "workspace";

type AppNotification = {
  id: string;
  type: AppNotificationType;
  title: string;
  content: string;
  createdAt: string;
  read: boolean;
  sourceId?: string;
  count?: number;
  requesterId?: number;
};

export type ChatUnreadPayload = {
  chatId: string;
  chatTitle: string;
  unreadCount: number;
};

type NotificationContextType = {
  // Chat notifications
  notifications: AppNotification[];
  upsertChatUnread: (payload: ChatUnreadPayload) => void;
  markChatAsRead: (chatId: string) => void;
  setWatcherUser: (id: string, name: string) => void;
  setActiveChatId: (id: string | null) => void;

  // Workspace notifications — single source of truth for all pages
  wsNotifications: WorkspaceNotif[];
  wsNotifsLoading: boolean;
  pendingInvites: PendingInvite[];
  pendingInvitesLoading: boolean;
  wsUnreadCount: number;

  // Unified
  totalUnread: number;
  fetchWsData: () => void;
  markAllAsRead: () => void;
  clearAllNotifications: () => void;
  removeInvite: (id: number) => void;
  addFriendRequestNotif: (displayName: string, requesterId: number) => void;
  removeNotification: (id: string) => void;
};

const NotificationContext = createContext<NotificationContextType | null>(null);

// Mock chat titles — replace with DB lookup later
const CHAT_TITLES: Record<string, string> = {
  transcendence: "ft_transcendence",
  frontend:      "Frontend",
  backend:       "Backend",
  deadlines:     "Deadlines",
  ai:            "Meowlinette",
};

function createId() {
  return crypto.randomUUID?.() ?? `n-${Date.now()}-${Math.random()}`;
}

function getTime() {
  return new Date().toISOString();
}

export function NotificationProvider({ children }: { children: React.ReactNode }) {
  const { user, token } = useAuth();

  const chatTitleCacheRef = useRef<Record<string, string>>({});

  // ── Chat notifications ──────────────────────────────────────────────────
  const cacheKey = user ? `notifications_cache_${user.id}` : null;

  const [notifications, setNotifications] = useState<AppNotification[]>(() => {
    try {
      if (!cacheKey) return [];
      const raw = localStorage.getItem(cacheKey);
      if (!raw) return [];
      const parsed = JSON.parse(raw);
      if (!Array.isArray(parsed)) return [];
      return parsed.map((n: Partial<AppNotification>) => ({
        id:        n.id        ?? createId(),
        type:      (n.type ?? "message") as AppNotificationType,
        title:     n.title     ?? "",
        content:   n.content   ?? "",
        createdAt: n.createdAt ?? "",
        read:      n.read      ?? false,
        ...(n.sourceId !== undefined ? { sourceId: n.sourceId } : {}),
        ...(n.count    !== undefined ? { count:    n.count    } : {}),
      }));
    } catch {
      return [];
    }
  });

  // ── Workspace notifications ─────────────────────────────────────────────
  const [wsNotifications,       setWsNotifications]       = useState<WorkspaceNotif[]>([]);
  const [wsNotifsLoading,       setWsNotifsLoading]       = useState(false);
  const [pendingInvites,        setPendingInvites]        = useState<PendingInvite[]>([]);
  const [pendingInvitesLoading, setPendingInvitesLoading] = useState(false);
  const [wsUnreadCount,         setWsUnreadCount]         = useState(0);

  // ── Watcher / active chat refs ──────────────────────────────────────────
  const [watcherUserId,   setWatcherUserId]   = useState<string>(user ? String(user.id) : "");
  const [watcherUserName, setWatcherUserName] = useState<string>(user?.displayName ?? "");
  const [activeChatId,    setActiveChatId]    = useState<string | null>(null);

  useEffect(() => {
    if (user) {
      setWatcherUserId(String(user.id));
      setWatcherUserName(user.displayName);
      // Load this user's cached chat notifications after user is known
      try {
        const raw = localStorage.getItem(`notifications_cache_${user.id}`);
        if (raw) {
          const parsed = JSON.parse(raw);
          if (Array.isArray(parsed)) {
            setNotifications(parsed.map((n: Partial<AppNotification>) => ({
              id:        n.id        ?? createId(),
              type:      (n.type ?? "message") as AppNotificationType,
              title:     n.title     ?? "",
              content:   n.content   ?? "",
              createdAt: n.createdAt ?? "",
              read:      n.read      ?? false,
              ...(n.sourceId !== undefined ? { sourceId: n.sourceId } : {}),
              ...(n.count    !== undefined ? { count:    n.count    } : {}),
            })));
          }
        }
      } catch { /* ignore */ }
    } else {
      // Clear in-memory notifications when user logs out
      setNotifications([]);
    }
  }, [user?.id]); // eslint-disable-line react-hooks/exhaustive-deps

  const watcherUserIdRef   = useRef(watcherUserId);
  const watcherUserNameRef = useRef(watcherUserName);
  const activeChatIdRef    = useRef(activeChatId);

  useEffect(() => { watcherUserIdRef.current   = watcherUserId;   }, [watcherUserId]);
  useEffect(() => { watcherUserNameRef.current  = watcherUserName; }, [watcherUserName]);
  useEffect(() => { activeChatIdRef.current     = activeChatId;    }, [activeChatId]);

  async function resolveChatTitle(chatId: string): Promise<string> {
    if (CHAT_TITLES[chatId]) return CHAT_TITLES[chatId];
    if (chatTitleCacheRef.current[chatId]) return chatTitleCacheRef.current[chatId];
    if (chatId.startsWith("dm-") && token) {
      try {
        const res = await fetch("/api/dm", { headers: { Authorization: `Bearer ${token}` } });
        if (res.ok) {
          const dms = await res.json();
          dms.forEach((d: { chat_id: string; display_name: string }) => {
            chatTitleCacheRef.current[d.chat_id] = d.display_name;
          });
          if (chatTitleCacheRef.current[chatId]) return chatTitleCacheRef.current[chatId];
        }
      } catch { /* ignore */ }
    }
    if (!chatId.startsWith("dm-") && !chatId.startsWith("ai-") && token) {
      try {
        const res = await fetch("/api/channels", { headers: { Authorization: `Bearer ${token}` } });
        if (res.ok) {
          const channels = await res.json();
          channels.forEach((ch: any) => {
            chatTitleCacheRef.current[ch.id] = ch.name;
          });
          if (chatTitleCacheRef.current[chatId]) return chatTitleCacheRef.current[chatId];
        }
      } catch { /* ignore */ }
    }
    return chatId;
  }

  // ── Persist chat notifications ──────────────────────────────────────────
  useEffect(() => {
    try {
      if (!cacheKey) return;
      const toStore = notifications.map(({ id, type, title, content, sourceId, count, read, createdAt }) => ({
        id, type, title, content, sourceId, count, read, createdAt,
      }));
      localStorage.setItem(cacheKey, JSON.stringify(toStore));
    } catch { /* ignore */ }
  }, [notifications]);

  // ── Poll ws unread count every 30s ──────────────────────────────────────
  useEffect(() => {
    if (!user) { setWsUnreadCount(0); return; }
    const poll = () => getWsUnreadCount().then(setWsUnreadCount).catch(() => {});
    poll();
    const id = setInterval(poll, 30_000);
    return () => clearInterval(id);
  }, [user?.id]); // eslint-disable-line react-hooks/exhaustive-deps

  // ── WebSocket for chat notifications ────────────────────────────────────
  useEffect(() => {
    if (!watcherUserId) return;
    const url    = `${window.location.protocol === 'https:' ? 'wss' : 'ws'}://${window.location.host}/ws?userId=notif-${watcherUserId}&userName=notif`;
    const socket = new WebSocket(url);

    socket.onmessage = async (event) => {
      try {
        const e = JSON.parse(event.data);

        if (e.type === "workspace:notification") {
          setWsUnreadCount(prev => prev + 1);
          if (e.payload.notif_type === "invite_received") {
            getPendingInvites().then(setPendingInvites).catch(() => {});
          } else {
            setWsNotifications(prev => [{
              id: Date.now(),
              type: e.payload.notif_type,
              message: e.payload.message,
              workspace_id: e.payload.workspace_id,
              is_read: false,
              created_at: new Date().toISOString(),
            }, ...prev]);
          }
          return;
        }

        if (e.type === "mention:all") {
          const { chatId, message } = e.payload;
          if (String(message.senderId) === String(watcherUserIdRef.current)) return;
          const chatTitle = await resolveChatTitle(chatId);
          setNotifications(prev => {
            const existing = prev.find(n => n.type === "message" && n.sourceId === chatId);
            const count    = (existing?.count ?? 0) + 1;
            const updated: AppNotification = {
              id: existing?.id ?? createId(), type: "message",
              title: chatTitle,
              content: `${count} unread message${count > 1 ? "s" : ""} in this project chat.`,
              sourceId: chatId, count, createdAt: getTime(), read: false,
            };
            if (existing) return prev.map(n => n.id === existing.id ? updated : n);
            return [updated, ...prev];
          });
          return;
        }

        if (e.type !== "chat:new_message") return;

        const { chatId, message } = e.payload;
        const isFromMe     = message.senderId === watcherUserIdRef.current;
        const isActiveChat = chatId === activeChatIdRef.current;
        const isAI         = message.type === "ai";

        if (isFromMe || isActiveChat || isAI) return;

        if (chatId.startsWith("dm-") && message.senderId !== String(watcherUserIdRef.current)) {
          try {
            const res = await fetch(`/api/dm/check/${chatId}`, {
              headers: { "x-sender-id": message.senderId },
            });
            const data = await res.json();
            if (!data.allowed) return;
          } catch { /* allow by default */ }
        }

        const chatTitle = await resolveChatTitle(chatId);
        setNotifications(prev => {
          const existing = prev.find(n => n.type === "message" && n.sourceId === chatId);
          const count    = (existing?.count ?? 0) + 1;
          const updated: AppNotification = {
            id: existing?.id ?? createId(), type: "message",
            title: chatTitle,
            content: `${count} unread message${count > 1 ? "s" : ""} in this project chat.`,
            sourceId: chatId, count, createdAt: getTime(), read: false,
          };
          if (existing) return prev.map(n => n.id === existing.id ? updated : n);
          return [updated, ...prev];
        });
      } catch { /* ignore */ }
    };

    return () => { socket.close(); };
  }, [watcherUserId]);

  // ── Actions ─────────────────────────────────────────────────────────────

  function setWatcherUser(id: string, name: string) {
    setWatcherUserId(id);
    setWatcherUserName(name);
  }

  function upsertChatUnread(payload: ChatUnreadPayload) {
    setNotifications(prev => {
      const existing = prev.find(n => n.type === "message" && n.sourceId === payload.chatId);
      const updated: AppNotification = {
        id:        existing?.id ?? createId(),
        type:      "message",
        title:     payload.chatTitle,
        content:   `${payload.unreadCount} unread message${payload.unreadCount > 1 ? "s" : ""} in this project chat.`,
        sourceId:  payload.chatId,
        count:     payload.unreadCount,
        createdAt: getTime(),
        read:      false,
      };
      if (existing) return prev.map(n => n.id === existing.id ? updated : n);
      return [updated, ...prev];
    });
  }

  function markChatAsRead(chatId: string) {
    setNotifications(prev =>
      prev.map(n => n.type === "message" && n.sourceId === chatId ? { ...n, read: true } : n)
    );
  }

  function markAllAsRead() {
    setNotifications(prev => prev.map(n => ({ ...n, read: true })));
    markWsNotificationsRead().catch(() => {});
    setWsUnreadCount(0);
  }

  function clearAllNotifications() {
    setNotifications([]);
    setWsNotifications([]);
    setWsUnreadCount(0);
    clearWsNotifications().catch(() => {});
  }

  function removeInvite(id: number) {
    setPendingInvites(prev => prev.filter(i => i.id !== id));
  }

  function addFriendRequestNotif(displayName: string, requesterId: number) {
    setNotifications(prev => [{
      id:          createId(),
      type:        "friend_request" as AppNotificationType,
      title:       displayName,
      content:     "sent you a friend request",
      sourceId:    "friends",
      requesterId,
      createdAt:   getTime(),
      read:        false,
    }, ...prev]);
  }

  function removeNotification(id: string) {
    setNotifications(prev => prev.filter(n => n.id !== id));
  }

  const fetchWsData = useCallback(() => {
    if (!user) return;
    setNotifications(prev => prev.map(n => ({ ...n, read: true })));
    setPendingInvitesLoading(true);
    setWsNotifsLoading(true);
    getPendingInvites()
      .then(setPendingInvites).catch(() => setPendingInvites([]))
      .finally(() => setPendingInvitesLoading(false));
    getWsNotifications()
      .then(data => {
        setWsNotifications(data);
        markWsNotificationsRead().catch(() => {});
        setWsUnreadCount(0);
      })
      .catch(() => setWsNotifications([]))
      .finally(() => setWsNotifsLoading(false));
  }, [user?.id]); // eslint-disable-line react-hooks/exhaustive-deps

  const chatUnread  = notifications.filter(n => !n.read).length;
  const totalUnread = chatUnread + wsUnreadCount;

  useEffect(() => {
    document.title = totalUnread > 0
      ? `(${totalUnread}) Collab42`
      : "Collab42";
  }, [totalUnread]);

  return (
    <NotificationContext.Provider value={{
      notifications,
      upsertChatUnread,
      markChatAsRead,
      setWatcherUser,
      setActiveChatId,
      wsNotifications,
      wsNotifsLoading,
      pendingInvites,
      pendingInvitesLoading,
      wsUnreadCount,
      totalUnread,
      fetchWsData,
      markAllAsRead,
      clearAllNotifications,
      removeInvite,
      addFriendRequestNotif,
      removeNotification,
    }}>
      {children}
    </NotificationContext.Provider>
  );
}

export function useNotifications() {
  const ctx = useContext(NotificationContext);
  if (!ctx) throw new Error("useNotifications must be used inside NotificationProvider");
  return ctx;
}
