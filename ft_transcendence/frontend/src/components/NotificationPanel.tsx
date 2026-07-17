import { useState, useRef, useEffect, Fragment, type ReactNode } from "react";
import { useNavigate } from "react-router-dom";
import {
  Bell, CheckCheck, Trash2, Users, MessageCircle, Clock3, UserPlus,
} from "lucide-react";
import { formatNotifTime, respondToInvite } from "../api";
import { useNotifications } from "../context/NotificationContext";
import { useChat } from "../context/ChatContext";

type Props = {
  onAcceptInvite?: (workspaceId: number) => void;
};

export default function NotificationPanel({ onAcceptInvite }: Props) {
  const {
    notifications, totalUnread, markAllAsRead, clearAllNotifications,
    markChatAsRead, removeNotification, wsNotifications, wsNotifsLoading,
    pendingInvites, pendingInvitesLoading, fetchWsData, removeInvite,
  } = useNotifications();

  const { acceptFriendRequest, declineFriendRequest } = useChat();
  const navigate = useNavigate();

  const [isNotifOpen, setIsNotifOpen] = useState(false);
  const notifRef = useRef<HTMLDivElement>(null);
  const [, setTick] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => setTick(t => t + 1), 60000);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    function handler(e: MouseEvent) {
      if (notifRef.current && !notifRef.current.contains(e.target as Node)) setIsNotifOpen(false);
    }
    document.addEventListener("mousedown", handler);
    return () => document.removeEventListener("mousedown", handler);
  }, []);

  useEffect(() => {
    if (isNotifOpen) fetchWsData();
  }, [isNotifOpen]); // eslint-disable-line react-hooks/exhaustive-deps

  const handleInviteRespond = async (inviteId: number, action: "accept" | "reject") => {
    try {
      const res = await respondToInvite(inviteId, action);
      removeInvite(inviteId);
      if (action === "accept" && res.workspace_id) onAcceptInvite?.(res.workspace_id);
    } catch (err) {
      console.error("Failed to respond to invite:", err);
    }
  };

  type NotifItem = { time: string; key: string; node: ReactNode };
  const items: NotifItem[] = [
    ...pendingInvites.map(inv => ({
      time: inv.created_at,
      key: `inv-${inv.id}`,
      node: (
        <div className="px-4 py-3 border-b border-gray-100 bg-indigo-50/40">
          <div className="flex items-start gap-2.5">
            <div className="w-7 h-7 rounded-full bg-indigo-100 flex items-center justify-center shrink-0 mt-0.5">
              <Users size={12} className="text-indigo-600" />
            </div>
            <div className="flex-1 min-w-0">
              <p className="text-[12px] font-semibold text-gray-900 truncate">Workspace Invitation</p>
              <p className="text-[11px] text-gray-500 mt-0.5">
                <span className="font-medium">@{inv.inviter_login}</span> invited you to{' '}
                <span className="font-medium">{inv.workspace_name}</span> as{' '}
                <span className="font-medium capitalize">{inv.role}</span>
              </p>
              <div className="flex gap-1.5 mt-2">
                <button onClick={() => handleInviteRespond(inv.id, "accept")}
                  className="px-2.5 py-1 text-[10px] font-semibold bg-indigo-600 text-white rounded-md hover:bg-indigo-700 transition-colors">
                  Accept
                </button>
                <button onClick={() => handleInviteRespond(inv.id, "reject")}
                  className="px-2.5 py-1 text-[10px] font-semibold bg-gray-100 text-gray-600 rounded-md hover:bg-gray-200 transition-colors">
                  Decline
                </button>
              </div>
            </div>
          </div>
        </div>
      ),
    })),
    ...wsNotifications.map(n => ({
      time: n.created_at,
      key: `ws-${n.id}`,
      node: (
        <div className="flex items-start gap-3 px-4 py-3 border-b border-gray-100">
          <div className="w-7 h-7 rounded-full bg-gray-100 flex items-center justify-center shrink-0 mt-0.5">
            <Bell size={12} className="text-gray-500" />
          </div>
          <div className="min-w-0 flex-1">
            <p className="text-[11px] text-gray-700 leading-relaxed">{n.message}</p>
            <span className="text-[10px] text-gray-400">{formatNotifTime(n.created_at)}</span>
          </div>
        </div>
      ),
    })),
    ...notifications.map(n => ({
      time: n.createdAt,
      key: `app-${n.id}`,
      node: n.type === "friend_request" ? (
        <div className="px-4 py-3 border-b border-gray-100 bg-green-50/40">
          <div className="flex items-start gap-2.5">
            <div className="w-7 h-7 rounded-full bg-green-100 flex items-center justify-center shrink-0 mt-0.5">
              <UserPlus size={12} className="text-green-600" />
            </div>
            <div className="flex-1 min-w-0">
              <p className="text-[12px] font-semibold text-gray-900 truncate">Friend Request</p>
              <p className="text-[11px] text-gray-500 mt-0.5">
                <span className="font-medium">{n.title}</span> sent you a friend request
              </p>
              <div className="flex gap-1.5 mt-2">
                <button
                  onClick={() => { acceptFriendRequest(n.requesterId!); removeNotification(n.id); }}
                  className="px-2.5 py-1 text-[10px] font-semibold bg-green-600 text-white rounded-md hover:bg-green-700 transition-colors">
                  Accept
                </button>
                <button
                  onClick={() => { declineFriendRequest(n.requesterId!); removeNotification(n.id); }}
                  className="px-2.5 py-1 text-[10px] font-semibold bg-gray-100 text-gray-600 rounded-md hover:bg-gray-200 transition-colors">
                  Ignore
                </button>
              </div>
              <span className="text-[10px] text-gray-400 mt-1 block">{formatNotifTime(n.createdAt)}</span>
            </div>
          </div>
        </div>
      ) : (() => {
        const Icon = n.type === "deadline" ? Clock3 : MessageCircle;
        return (
          <button
            onClick={() => {
              if (n.sourceId && n.sourceId !== "deadlines") {
                markChatAsRead(n.sourceId);
                setIsNotifOpen(false);
                navigate("/chats", { state: { openChatId: n.sourceId } });
              }
            }}
            className={`w-full text-left flex items-start gap-3 px-4 py-3 border-b border-gray-100 transition-colors ${n.read ? "opacity-60" : "bg-indigo-50/30"} ${n.sourceId && n.sourceId !== "deadlines" ? "hover:bg-gray-50 cursor-pointer" : "cursor-default"}`}>
            <div className="w-7 h-7 rounded-full bg-gray-100 flex items-center justify-center shrink-0 mt-0.5">
              <Icon size={12} className="text-gray-500" />
            </div>
            <div className="min-w-0 flex-1">
              <p className="text-[12px] font-medium text-gray-900 truncate">{n.title}</p>
              <p className="text-[11px] text-gray-500 mt-0.5 line-clamp-2">{n.content}</p>
              <span className="text-[10px] text-gray-400 mt-1 block">{formatNotifTime(n.createdAt)}</span>
            </div>
            {!n.read && <span className="w-1.5 h-1.5 rounded-full bg-red-400 shrink-0 mt-1.5" />}
          </button>
        );
      })(),
    })),
  ].sort((a, b) => new Date(b.time).getTime() - new Date(a.time).getTime());

  return (
    <div ref={notifRef} className="relative">
      <button
        onClick={() => setIsNotifOpen(o => !o)}
        className="w-[30px] h-[30px] flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors relative"
      >
        <Bell size={15} />
        {totalUnread > 0 && (
          <span className="absolute -top-0.5 -right-0.5 min-w-[14px] h-[14px] px-0.5 rounded-full bg-red-500 text-white text-[8px] font-semibold flex items-center justify-center">
            {totalUnread}
          </span>
        )}
      </button>

      {isNotifOpen && (
        <div className="absolute right-0 top-10 w-80 bg-white border border-gray-100 rounded-xl shadow-lg overflow-hidden z-50">
          <div className="px-4 py-3 border-b border-gray-100 flex items-center justify-between">
            <div>
              <p className="text-[13px] font-semibold">Notifications</p>
              <p className="text-[11px] text-gray-400">{totalUnread} unread</p>
            </div>
            <div className="flex items-center gap-1">
              <button onClick={markAllAsRead} title="Mark all as read"
                className="w-[26px] h-[26px] flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors">
                <CheckCheck size={13} />
              </button>
              <button onClick={clearAllNotifications} title="Clear all"
                className="w-[26px] h-[26px] flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors">
                <Trash2 size={13} />
              </button>
            </div>
          </div>

          <div className="max-h-80 overflow-y-auto">
            {pendingInvitesLoading && (
              <div className="px-4 py-2 text-[11px] text-gray-400">Loading invitations…</div>
            )}
            {wsNotifsLoading && (
              <div className="px-4 py-2 text-[11px] text-gray-400">Loading notifications…</div>
            )}
            {!pendingInvitesLoading && !wsNotifsLoading && items.length === 0 ? (
              <div className="py-8 text-center">
                <p className="text-[12px] text-gray-400">No notifications</p>
              </div>
            ) : (
              items.map(item => <Fragment key={item.key}>{item.node}</Fragment>)
            )}
          </div>
        </div>
      )}
    </div>
  );
}
