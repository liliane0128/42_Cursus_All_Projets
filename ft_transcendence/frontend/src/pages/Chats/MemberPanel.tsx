import type { ProjectChat, Friend } from "../../context/ChatContext";
import Avatar from "./Avatar";

type Props = {
  activeChat: ProjectChat;
  onlineUsers: { id: string; name: string }[];
  friends: Friend[];
  onSelectUser: (userId: string, displayName: string, avatar?: string) => void;
  isOpen: boolean;
  onClose: () => void;
};

export default function MemberPanel({ activeChat, onlineUsers, friends, onSelectUser, isOpen, onClose }: Props) {
  if (!isOpen) return null;

  return (
    <>
      <div
        className="fixed inset-0 bg-black/30 z-20 md:hidden"
        onClick={onClose}
      />
      <div className="
        fixed top-0 right-0 h-full w-64 z-30 flex flex-col bg-white shadow-xl
        md:static md:w-56 md:flex-shrink-0 md:shadow-none md:z-auto
        border-l border-gray-100
      ">
        <div className="h-[52px] px-4 border-b border-gray-100 flex items-center gap-1.5 flex-shrink-0">
          <p className="text-[12px] font-semibold text-gray-900">Members</p>
          <span className="text-[11px] text-gray-400">{(activeChat.participantIds ?? []).length}</span>
        </div>
        <div className="flex-1 overflow-y-auto py-2">
          {(activeChat.participantIds ?? []).map(uid => {
            const member = activeChat.members?.find(m => String(m.id) === uid);
            const online = onlineUsers.find(u => u.id === uid);
            const friend = friends.find(f => String(f.id) === uid);
            const memberInfo = {
              id: uid,
              name: member?.displayName ?? online?.name ?? friend?.displayName ?? `User ${uid}`,
              isOnline: !!online,
              avatar: member?.avatar ?? friend?.avatar ?? undefined,
              role: member?.role ?? "member",
            };
            return (
              <div key={memberInfo.id} className="flex items-center gap-2.5 px-4 py-2">
                <div className="relative flex-shrink-0">
                  <Avatar
                    src={memberInfo.avatar}
                    name={memberInfo.name}
                    size="sm"
                    className="border border-indigo-100"
                    onClick={() => onSelectUser(memberInfo.id, memberInfo.name, memberInfo.avatar)}
                  />
                  <span className={`absolute bottom-0 right-0 w-2 h-2 rounded-full border border-white ${memberInfo.isOnline ? "bg-green-400" : "bg-gray-300"}`} />
                </div>
                <p className="text-[12px] text-gray-700 truncate">{memberInfo.name}</p>
                {memberInfo.role === "owner" && (
                  <span className="text-[9px] px-1.5 py-0.5 rounded-full bg-amber-100 text-amber-600 font-semibold flex-shrink-0">Owner</span>
                )}
                {memberInfo.role === "admin" && (
                  <span className="text-[9px] px-1.5 py-0.5 rounded-full bg-blue-100 text-blue-600 font-semibold flex-shrink-0">Admin</span>
                )}
              </div>
            );
          })}
        </div>
      </div>
    </>
  );
}
