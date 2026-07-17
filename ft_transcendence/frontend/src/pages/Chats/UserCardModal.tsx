import { Cat } from "lucide-react";
import { useChat } from "../../context/ChatContext";
import Avatar from "./Avatar";
import { useAuth } from "../../context/AuthContext";

export type UserCardData = {
  id: string;
  displayName: string;
  avatar?: string;
  status?: string;
  login?: string;
  email?: string;
  bio?: string;
  is_deleted?: boolean;
};

type Props = {
  selectedUser: UserCardData | null;
  setSelectedUser: (u: UserCardData | null) => void;
  sentRequests: Set<number>;
  setSentRequests: (fn: (prev: Set<number>) => Set<number>) => void;
  onOpenDM: (chatId: string) => void;
  onRemoveFriend: (target: { id: number; name: string }) => void;
};

export default function UserCardModal({
  selectedUser, setSelectedUser,
  sentRequests, setSentRequests,
  onOpenDM, onRemoveFriend,
}: Props) {
  const { friends, onlineUsers, sendFriendRequest, refreshChannels, openChat } = useChat();
  const { user, token } = useAuth();

  if (!selectedUser) return null;

  return (
    <div
      className="fixed inset-0 z-50 flex items-center justify-center bg-black/40"
      onClick={() => setSelectedUser(null)}
    >
      <div
        className="bg-white rounded-2xl shadow-2xl w-72 p-6 flex flex-col items-center gap-3"
        onClick={e => e.stopPropagation()}
      >
        {/* Avatar */}
        <div>
          {selectedUser.id === "meow"
            ? <div className="w-16 h-16 rounded-full overflow-hidden bg-indigo-50 border-2 border-indigo-100 flex items-center justify-center">
                <Cat className="w-8 h-8 text-indigo-400" />
              </div>
            : <Avatar
                src={selectedUser.avatar}
                name={selectedUser.displayName}
                size="lg"
                className="border-2 border-indigo-100"
              />
          }
        </div>

        {/* Name + login */}
        <div className="text-center">
          <p className="text-[15px] font-semibold text-gray-900">
            {selectedUser.displayName}
            {selectedUser.is_deleted && <span className="text-gray-400 text-[10px] ml-1">(deactivated)</span>}
          </p>
          {selectedUser.login && <p className="text-[12px] text-gray-400 mt-0.5">@{selectedUser.login}</p>}
        </div>

        {/* Email */}
        {selectedUser.email && (
          <p className="text-[11px] text-gray-500 text-center">{selectedUser.email}</p>
        )}

        {/* Bio */}
        {selectedUser.bio && (
          <p className="text-[12px] text-gray-600 text-center leading-relaxed bg-gray-50 rounded-lg px-3 py-2 w-full">{selectedUser.bio}</p>
        )}

        {/* Buttons — context-aware */}
        {selectedUser.id !== String(user?.id) && (
          selectedUser.is_deleted ? (
            <p className="text-[11px] text-gray-400">This account has been deactivated</p>
          ) : (
            <div className="flex gap-2 w-full mt-1">
              {friends.some(f => String(f.id) === selectedUser.id) ? (
                <>
                  <button
                    onClick={async () => {
                      try {
                        const res = await fetch(`/api/dm/${selectedUser.id}`, {
                          method: "POST",
                          headers: { Authorization: `Bearer ${token}` },
                        });
                        if (!res.ok) return;
                        const { chat_id } = await res.json() as { chat_id: string };
                        await refreshChannels();
                        onOpenDM(chat_id);
                        setSelectedUser(null);
                      } catch { /* ignore */ }
                    }}
                    className="flex-1 py-2 rounded-lg bg-gray-900 text-white text-[12px] font-medium hover:bg-gray-700 transition-colors"
                  >
                    Message
                  </button>
                  <button
                    onClick={() => onRemoveFriend({ id: Number(selectedUser.id), name: selectedUser.displayName })}
                    className="flex-1 py-2 rounded-lg border border-red-200 text-red-500 text-[12px] font-medium hover:bg-red-50 transition-colors"
                  >
                    Remove
                  </button>
                </>
              ) : sentRequests.has(Number(selectedUser.id)) ? (
                <button disabled className="flex-1 py-2 rounded-lg border border-gray-200 text-gray-300 text-[12px] font-medium cursor-not-allowed">
                  Sent
                </button>
              ) : (
                <button
                  onClick={async () => {
                    try { await sendFriendRequest(Number(selectedUser.id)); } catch { /* ignore */ }
                    setSentRequests(prev => new Set(prev).add(Number(selectedUser.id)));
                  }}
                  className="flex-1 py-2 rounded-lg bg-indigo-600 text-white text-[12px] font-medium hover:bg-indigo-700 transition-colors"
                >
                  Add Friend
                </button>
              )}
            </div>
          )
        )}
      </div>
    </div>
  );
}
