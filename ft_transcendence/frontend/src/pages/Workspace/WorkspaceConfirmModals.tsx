import React from 'react';
import { Trash2, Check, LogOut } from 'lucide-react';
import { WorkspaceMember } from '../../api';
import { Workspace } from './WorkspaceTypes';

interface Props {
  // Kick member
  kickConfirm: { userId: number; login: string } | null;
  onKickClose: () => void;
  onKickConfirm: (userId: number) => void;

  // Leave workspace
  leaveConfirm: boolean;
  myRole: string | null;
  onLeaveClose: () => void;
  onLeaveConfirm: () => void;

  // Role change
  roleConfirm: { userId: number; login: string; newRole: 'admin' | 'member' } | null;
  onRoleClose: () => void;
  onRoleConfirm: (userId: number, newRole: 'admin' | 'member') => void;

  // Transfer & leave
  isTransferLeaveOpen: boolean;
  onTransferLeaveClose: () => void;
  transferTargetId: number | null;
  setTransferTargetId: (id: number | null) => void;
  members: WorkspaceMember[];
  currentUserId: number | undefined;
  onTransferAndLeave: () => void;

  // Delete workspace (from manage sidebar danger tab)
  showDeleteConfirm: boolean;
  onDeleteClose: () => void;
  activeWorkspace: Workspace | null;
  onDeleteConfirm: (id: number) => void;

  // Delete workspace (from card list hover button)
  pendingDeleteId: number | null;
  workspaces: Workspace[];
  onPendingDeleteClose: () => void;
  onPendingDeleteConfirm: (id: number) => void;
}

export default function WorkspaceConfirmModals({
  kickConfirm, onKickClose, onKickConfirm,
  leaveConfirm, myRole, onLeaveClose, onLeaveConfirm,
  roleConfirm, onRoleClose, onRoleConfirm,
  isTransferLeaveOpen, onTransferLeaveClose, transferTargetId, setTransferTargetId, members, currentUserId, onTransferAndLeave,
  showDeleteConfirm, onDeleteClose, activeWorkspace, onDeleteConfirm,
  pendingDeleteId, workspaces, onPendingDeleteClose, onPendingDeleteConfirm,
}: Props) {
  return (
    <>
      {/* Kick member */}
      {kickConfirm && (
        <div className="fixed inset-0 bg-black/40 flex items-center justify-center p-4 z-[70]" onClick={onKickClose}>
          <div className="bg-white border border-gray-100 rounded-xl w-full max-w-[320px] shadow-xl p-5" onClick={e => e.stopPropagation()}>
            <h3 className="text-[14px] font-semibold text-gray-900 mb-1">Remove Member</h3>
            <p className="text-[12px] text-gray-500 mb-5">
              Remove <span className="font-semibold text-gray-800">@{kickConfirm.login}</span> from this workspace? They will lose access immediately.
            </p>
            <div className="flex gap-2">
              <button onClick={onKickClose}
                className="flex-1 py-2 text-[12px] border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors">
                Cancel
              </button>
              <button onClick={() => onKickConfirm(kickConfirm.userId)}
                className="flex-1 py-2 text-[12px] font-medium rounded-lg text-white bg-red-500 hover:bg-red-600 transition-colors">
                Remove
              </button>
            </div>
          </div>
        </div>
      )}

      {/* Leave workspace */}
      {leaveConfirm && (
        <div className="fixed inset-0 bg-black/40 flex items-center justify-center p-4 z-[70]" onClick={onLeaveClose}>
          <div className="bg-white border border-gray-100 rounded-xl w-full max-w-[320px] shadow-xl p-5" onClick={e => e.stopPropagation()}>
            <h3 className="text-[14px] font-semibold text-gray-900 mb-1">
              {myRole === 'owner' ? 'Transfer & Leave' : 'Leave Workspace'}
            </h3>
            <p className="text-[12px] text-gray-500 mb-5">
              {myRole === 'owner'
                ? 'You will be asked to transfer ownership before leaving.'
                : 'Are you sure you want to leave this workspace? You will lose access immediately.'}
            </p>
            <div className="flex gap-2">
              <button onClick={onLeaveClose}
                className="flex-1 py-2 text-[12px] border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors">
                Cancel
              </button>
              <button onClick={onLeaveConfirm}
                className="flex-1 py-2 text-[12px] font-medium rounded-lg text-white bg-red-500 hover:bg-red-600 transition-colors">
                {myRole === 'owner' ? 'Continue' : 'Leave'}
              </button>
            </div>
          </div>
        </div>
      )}

      {/* Role change */}
      {roleConfirm && (
        <div className="fixed inset-0 bg-black/40 flex items-center justify-center p-4 z-[70]" onClick={onRoleClose}>
          <div className="bg-white border border-gray-100 rounded-xl w-full max-w-[320px] shadow-xl p-5" onClick={e => e.stopPropagation()}>
            <h3 className="text-[14px] font-semibold text-gray-900 mb-1">
              {roleConfirm.newRole === 'admin' ? 'Promote to Admin' : 'Remove Admin'}
            </h3>
            <p className="text-[12px] text-gray-500 mb-5">
              {roleConfirm.newRole === 'admin'
                ? <><span className="font-semibold text-gray-800">@{roleConfirm.login}</span> will be able to invite and remove members.</>
                : <><span className="font-semibold text-gray-800">@{roleConfirm.login}</span> will become a regular member.</>
              }
            </p>
            <div className="flex gap-2">
              <button onClick={onRoleClose}
                className="flex-1 py-2 text-[12px] border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors">
                Cancel
              </button>
              <button onClick={() => onRoleConfirm(roleConfirm.userId, roleConfirm.newRole)}
                className={`flex-1 py-2 text-[12px] font-medium rounded-lg text-white transition-colors ${roleConfirm.newRole === 'admin' ? 'bg-indigo-600 hover:bg-indigo-700' : 'bg-orange-500 hover:bg-orange-600'}`}>
                Confirm
              </button>
            </div>
          </div>
        </div>
      )}

      {/* Transfer ownership & leave */}
      {isTransferLeaveOpen && (
        <div className="fixed inset-0 bg-black/40 flex items-center justify-center p-4 z-[70]" onClick={onTransferLeaveClose}>
          <div className="bg-white border border-gray-100 rounded-xl w-full max-w-[380px] shadow-xl overflow-hidden" onClick={e => e.stopPropagation()}>
            <div className="px-5 py-4 border-b border-gray-100">
              <h3 className="text-[14px] font-semibold text-gray-900">Transfer Ownership</h3>
              <p className="text-[11px] text-gray-400 mt-0.5">Select a new owner before you leave</p>
            </div>
            <div className="max-h-[300px] overflow-y-auto">
              {members.filter(m => m.user_id !== currentUserId).map(member => (
                <div key={member.user_id}
                  onClick={() => setTransferTargetId(transferTargetId === member.user_id ? null : member.user_id)}
                  className={`flex items-center gap-3 px-5 py-3 border-b border-gray-50 last:border-0 cursor-pointer transition-colors ${transferTargetId === member.user_id ? 'bg-indigo-50' : 'hover:bg-gray-50'}`}>
                  <div className="w-7 h-7 rounded-full overflow-hidden flex-shrink-0">
                    {member.avatar
                      ? <img src={member.avatar} className="w-full h-full object-cover" alt="" />
                      : <div className="w-full h-full bg-gray-200 flex items-center justify-center text-[10px] font-bold text-gray-500 uppercase">{member.login[0]}</div>
                    }
                  </div>
                  <div className="flex-1 min-w-0">
                    <p className="text-[12px] font-medium text-gray-900">{member.display_name}</p>
                    <p className="text-[10px] text-gray-400">@{member.login} · <span className="capitalize">{member.role}</span></p>
                  </div>
                  {transferTargetId === member.user_id && <Check size={14} className="text-indigo-600 flex-shrink-0" />}
                </div>
              ))}
            </div>
            <div className="flex gap-2 px-5 py-4 border-t border-gray-100">
              <button onClick={onTransferLeaveClose}
                className="flex-1 py-2 text-[12px] border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors">
                Cancel
              </button>
              <button onClick={onTransferAndLeave} disabled={transferTargetId === null}
                className="flex-1 py-2 text-[12px] font-medium bg-red-500 hover:bg-red-600 text-white rounded-lg disabled:opacity-40 transition-colors">
                Transfer & Leave
              </button>
            </div>
          </div>
        </div>
      )}

      {/* Delete from manage sidebar */}
      {showDeleteConfirm && activeWorkspace && (
        <div className="fixed inset-0 bg-black/30 z-[70] flex items-center justify-center" onClick={onDeleteClose}>
          <div className="bg-white rounded-xl border border-gray-100 shadow-xl w-80 p-6" onClick={e => e.stopPropagation()}>
            <div className="w-10 h-10 rounded-full bg-red-50 flex items-center justify-center mb-4">
              <Trash2 size={18} className="text-red-500" />
            </div>
            <h3 className="text-sm font-bold text-gray-900 mb-1">Delete Workspace</h3>
            <p className="text-[11px] text-gray-400 mb-5">
              Are you sure you want to delete <span className="font-semibold text-gray-700">"{activeWorkspace.name}"</span>? This will permanently remove all tasks, files, and messages.
            </p>
            <div className="flex gap-2 text-[11px]">
              <button onClick={onDeleteClose} className="flex-1 py-2 border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors cursor-pointer">Cancel</button>
              <button onClick={() => onDeleteConfirm(activeWorkspace.id)} className="flex-1 py-2 bg-red-500 hover:bg-red-600 text-white font-medium rounded-lg transition-colors cursor-pointer">Delete</button>
            </div>
          </div>
        </div>
      )}

      {/* Delete from workspace card list */}
      {pendingDeleteId !== null && (() => {
        const ws = workspaces.find(w => w.id === pendingDeleteId);
        if (!ws) return null;
        return (
          <div className="fixed inset-0 bg-black/30 z-50 flex items-center justify-center" onClick={onPendingDeleteClose}>
            <div className="bg-white rounded-xl border border-gray-100 shadow-xl w-80 p-6" onClick={e => e.stopPropagation()}>
              <div className="w-10 h-10 rounded-full bg-red-50 flex items-center justify-center mb-4">
                <Trash2 size={18} className="text-red-500" />
              </div>
              <h3 className="text-sm font-bold text-gray-900 mb-1">Delete Workspace</h3>
              <p className="text-[11px] text-gray-400 mb-5">
                Are you sure you want to delete <span className="font-semibold text-gray-700">"{ws.name}"</span>? This will permanently remove all tasks, files, and messages.
              </p>
              <div className="flex gap-2 text-[11px]">
                <button onClick={onPendingDeleteClose} className="flex-1 py-2 border border-gray-200 rounded-lg text-gray-500 hover:bg-gray-50 transition-colors cursor-pointer">Cancel</button>
                <button onClick={() => onPendingDeleteConfirm(ws.id)} className="flex-1 py-2 bg-red-500 hover:bg-red-600 text-white font-medium rounded-lg transition-colors cursor-pointer">Delete</button>
              </div>
            </div>
          </div>
        );
      })()}
    </>
  );
}
