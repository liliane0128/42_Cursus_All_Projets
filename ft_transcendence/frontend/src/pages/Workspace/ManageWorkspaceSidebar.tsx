import React, { useState, useEffect } from 'react';
import {
  X, Check, AlertCircle, UserPlus, CalendarDays, Trash2, LogOut, CheckCircle2, RotateCcw
} from 'lucide-react';
import {
  WorkspaceMember, WorkspacePendingInvite,
  inviteMember, lookupUser, getWorkspacePendingInvites,
  cancelWorkspaceInvite, updateWorkspaceApi, formatNotifTime
} from '../../api';
import { Workspace } from './WorkspaceTypes';
import { useChat } from '../../context/ChatContext';

interface Props {
  isOpen: boolean;
  onClose: () => void;
  workspace: Workspace;
  workspaceId: number;
  myRole: string | null;
  currentUserId: number | undefined;
  members: WorkspaceMember[];
  onKickRequest: (userId: number, login: string) => void;
  onRoleChangeRequest: (userId: number, login: string, newRole: 'admin' | 'member') => void;
  onLeaveRequest: () => void;
  onDeleteRequest: () => void;
  onWorkspaceSaved: (name: string, desc: string, deadline: string | null) => void;
  onToggleCompleted: (id: number) => void;
  loadWorkspacesFromBackend: () => Promise<void>;
  defaultTab: 'members' | 'general' | 'danger';
}

export default function ManageWorkspaceSidebar({
  isOpen, onClose, workspace, workspaceId, myRole, currentUserId, members,
  onKickRequest, onRoleChangeRequest, onLeaveRequest, onDeleteRequest,
  onWorkspaceSaved, onToggleCompleted, loadWorkspacesFromBackend, defaultTab,
}: Props) {
  const canAdmin = myRole === 'owner' || myRole === 'admin';
  const { onlineUsers } = useChat();
  const onlineIds = new Set(onlineUsers.map(u => u.id));

  const [manageTab, setManageTab]                 = useState<'members' | 'general' | 'danger'>(defaultTab);
  const [membersSubTab, setMembersSubTab]         = useState<'active' | 'pending'>('active');
  const [wsPendingInvites, setWsPendingInvites]   = useState<WorkspacePendingInvite[]>([]);
  const [pendingInvitesLoaded, setPendingInvitesLoaded] = useState(false);

  // Invite form
  const [inviteInput,     setInviteInput]     = useState('');
  const [inviteRole,      setInviteRole]      = useState<'member' | 'admin'>('member');
  const [inviteFoundUser, setInviteFoundUser] = useState<{ id: number; login: string; displayName: string; avatar: string | null } | null>(null);
  const [inviteSearching, setInviteSearching] = useState(false);
  const [inviteNotFound,  setInviteNotFound]  = useState(false);
  const [inviteLoading,   setInviteLoading]   = useState(false);
  const [inviteError,     setInviteError]     = useState('');
  const [inviteSuccess,   setInviteSuccess]   = useState(false);

  // General tab
  const [wsEditName,     setWsEditName]     = useState('');
  const [wsEditDesc,     setWsEditDesc]     = useState('');
  const [wsEditDeadline, setWsEditDeadline] = useState('');
  const [generalSaving,  setGeneralSaving]  = useState(false);
  const [generalSaved,   setGeneralSaved]   = useState(false);

  // Seed state whenever the sidebar opens
  useEffect(() => {
    if (!isOpen) return;
    setManageTab(defaultTab);
    setMembersSubTab('active');
    setPendingInvitesLoaded(false);
    setWsPendingInvites([]);
    setInviteInput(''); setInviteFoundUser(null); setInviteNotFound(false);
    setInviteRole('member'); setInviteError(''); setInviteSuccess(false);
    setWsEditName(workspace.name);
    setWsEditDesc(workspace.description);
    setWsEditDeadline(workspace.deadline ?? '');
  }, [isOpen]); // eslint-disable-line react-hooks/exhaustive-deps

  const loadWsPendingInvites = async () => {
    try {
      const data = await getWorkspacePendingInvites(workspaceId);
      setWsPendingInvites(data);
      setPendingInvitesLoaded(true);
    } catch {
      setWsPendingInvites([]);
      setPendingInvitesLoaded(true);
    }
  };

  const handleCancelInvite = async (inviteId: number) => {
    try {
      await cancelWorkspaceInvite(workspaceId, inviteId);
      setWsPendingInvites(prev => prev.filter(i => i.id !== inviteId));
    } catch (err: any) {
      console.error('Cancel invite failed:', err);
    }
  };

  const handleFindUser = async () => {
    const q = inviteInput.trim();
    if (!q) return;
    setInviteSearching(true);
    setInviteNotFound(false);
    setInviteFoundUser(null);
    try {
      const u = await lookupUser(q);
      if (u) setInviteFoundUser({ id: u.id, login: u.login, displayName: u.displayName, avatar: u.avatar ?? null });
      else setInviteNotFound(true);
    } finally {
      setInviteSearching(false);
    }
  };

  const handleInviteSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!inviteFoundUser && !inviteInput.trim()) return;
    setInviteLoading(true);
    setInviteError('');
    setInviteSuccess(false);
    try {
      await inviteMember(workspaceId, inviteFoundUser?.login ?? inviteInput, inviteRole);
      setInviteSuccess(true);
      setInviteInput(''); setInviteFoundUser(null); setInviteNotFound(false); setInviteRole('member');
      setPendingInvitesLoaded(false);
      getWorkspacePendingInvites(workspaceId)
        .then(data => { setWsPendingInvites(data); setPendingInvitesLoaded(true); })
        .catch(() => {});
      setTimeout(() => setInviteSuccess(false), 2000);
    } catch (err: any) {
      setInviteError(err.message || 'Failed to invite member');
    } finally {
      setInviteLoading(false);
    }
  };

  const handleSaveGeneral = async () => {
    if (!wsEditName.trim()) return;
    setGeneralSaving(true);
    try {
      await updateWorkspaceApi(workspaceId, {
        name: wsEditName.trim(),
        description: wsEditDesc.trim(),
        deadline: wsEditDeadline || null,
      });
      onWorkspaceSaved(wsEditName.trim(), wsEditDesc.trim(), wsEditDeadline || null);
      await loadWorkspacesFromBackend();
      setGeneralSaved(true);
      setTimeout(() => setGeneralSaved(false), 2000);
    } catch (err) {
      console.error('Save workspace failed:', err);
    } finally {
      setGeneralSaving(false);
    }
  };

  const manageTabs = [
    { key: 'members' as const, label: 'Members' },
    ...(canAdmin ? [{ key: 'general' as const, label: 'General' }] : []),
    { key: 'danger' as const, label: 'Actions' },
  ];

  if (!isOpen) return null;

  return (
    <>
      <div className="fixed inset-0 bg-black/20 z-40" onClick={onClose} />
      <div className="fixed top-0 right-0 bottom-0 w-[380px] bg-white border-l border-gray-100 shadow-2xl z-50 flex flex-col">

        {/* Header */}
        <div className="flex items-center justify-between px-5 py-4 border-b border-gray-100 flex-shrink-0">
          <div>
            <h2 className="text-[14px] font-semibold text-gray-900">Manage Workspace</h2>
            <p className="text-[11px] text-gray-400 mt-0.5">{workspace.name}</p>
          </div>
          <button onClick={onClose}
            className="w-7 h-7 flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors">
            <X size={14} />
          </button>
        </div>

        {/* Tabs */}
        <div className="flex border-b border-gray-100 flex-shrink-0">
          {manageTabs.map(tab => (
            <button key={tab.key} onClick={() => setManageTab(tab.key)}
              className={`px-4 py-2.5 text-[12px] font-medium border-b-2 transition-colors ${manageTab === tab.key ? 'text-gray-900 border-gray-900' : 'border-transparent text-gray-400 hover:text-gray-700'}`}>
              {tab.label}
            </button>
          ))}
        </div>

        <div className="flex-1 flex flex-col overflow-hidden">

          {/* Members tab */}
          {manageTab === 'members' && (
            <div className="flex flex-col flex-1 overflow-hidden">
              {canAdmin && (
                <div className="flex gap-0 px-5 pt-3 border-b border-gray-100 flex-shrink-0">
                  <button onClick={() => setMembersSubTab('active')}
                    className={`pb-2 mr-5 text-[12px] font-medium border-b-2 transition-colors ${membersSubTab === 'active' ? 'text-gray-900 border-gray-900' : 'border-transparent text-gray-400 hover:text-gray-600'}`}>
                    Members <span className="text-gray-400 font-normal">({members.length})</span>
                  </button>
                  <button onClick={() => { setMembersSubTab('pending'); if (!pendingInvitesLoaded) loadWsPendingInvites(); }}
                    className={`pb-2 text-[12px] font-medium border-b-2 transition-colors ${membersSubTab === 'pending' ? 'text-gray-900 border-gray-900' : 'border-transparent text-gray-400 hover:text-gray-600'}`}>
                    Pending{wsPendingInvites.length > 0 && <span className="ml-1 px-1.5 py-0.5 rounded-full bg-amber-100 text-amber-700 text-[10px] font-semibold">{wsPendingInvites.length}</span>}
                  </button>
                </div>
              )}

              {membersSubTab === 'active' && (
                <div className="flex-1 overflow-y-auto">
                  {members.length === 0 ? (
                    <div className="py-10 text-center text-[12px] text-gray-400">No members yet</div>
                  ) : (
                    <>
                      {[...members]
                        .sort((a, b) => ({ owner: 0, admin: 1, member: 2 }[a.role] ?? 3) - ({ owner: 0, admin: 1, member: 2 }[b.role] ?? 3))
                        .map(member => {
                          const isCurrentUser  = currentUserId === member.user_id;
                          const canKick = !isCurrentUser && member.role !== 'owner' &&
                            (myRole === 'owner' || (myRole === 'admin' && member.role === 'member'));
                          const canSetRole = myRole === 'owner' && !isCurrentUser && member.role !== 'owner';
                          return (
                            <div key={member.user_id} className="flex items-center gap-3 px-5 py-3 border-b border-gray-50 last:border-0 hover:bg-gray-50/40 transition-colors">
                              <div className="relative w-8 h-8 flex-shrink-0">
                                <div className="w-8 h-8 rounded-full overflow-hidden">
                                  {member.avatar
                                    ? <img src={member.avatar} alt={member.login} className="w-full h-full object-cover" />
                                    : <div className="w-full h-full bg-gray-200 flex items-center justify-center text-[11px] font-bold text-gray-500 uppercase">{member.login[0]}</div>
                                  }
                                </div>
                                <span className={`absolute bottom-0 right-0 w-2.5 h-2.5 rounded-full border-2 border-white ${onlineIds.has(String(member.user_id)) ? 'bg-green-400' : 'bg-gray-300'}`} />
                              </div>
                              <div className="flex-1 min-w-0">
                                <p className="text-[12px] font-medium text-gray-900 truncate">{member.display_name}</p>
                                <p className="text-[10px] text-gray-400">@{member.login}{isCurrentUser ? ' · you' : ''}</p>
                              </div>
                              <div className="flex items-center gap-1.5 flex-shrink-0">
                                {member.role === 'owner' && <span className="px-2 py-0.5 rounded-md bg-gray-900 text-white text-[10px] font-semibold">Owner</span>}
                                {member.role === 'admin' && <span className="px-2 py-0.5 rounded-md bg-blue-100 text-blue-700 text-[10px] font-semibold">Admin</span>}
                                {member.role === 'member' && <span className="px-2 py-0.5 rounded-md bg-gray-100 text-gray-500 text-[10px] font-semibold">Member</span>}
                                {canSetRole && (
                                  <button onClick={() => onRoleChangeRequest(member.user_id, member.login, member.role === 'admin' ? 'member' : 'admin')}
                                    className={`text-[10px] px-2 py-0.5 rounded-md font-medium transition-colors ${member.role === 'admin' ? 'bg-orange-50 text-orange-600 hover:bg-orange-100' : 'bg-indigo-50 text-indigo-600 hover:bg-indigo-100'}`}>
                                    {member.role === 'admin' ? '−Admin' : '+Admin'}
                                  </button>
                                )}
                                {canKick && (
                                  <button onClick={() => onKickRequest(member.user_id, member.login)}
                                    className="text-[10px] px-2 py-0.5 rounded-md font-medium bg-red-50 text-red-600 hover:bg-red-100 transition-colors">
                                    Kick
                                  </button>
                                )}
                              </div>
                            </div>
                          );
                        })}
                    </>
                  )}
                </div>
              )}

              {membersSubTab === 'pending' && (
                <div className="flex-1 overflow-y-auto">
                  {!pendingInvitesLoaded ? (
                    <div className="py-10 text-center text-[12px] text-gray-400">Loading…</div>
                  ) : wsPendingInvites.length === 0 ? (
                    <div className="py-10 text-center text-[12px] text-gray-400">No pending invitations</div>
                  ) : wsPendingInvites.map(inv => (
                    <div key={inv.id} className="flex items-center gap-3 px-5 py-3 border-b border-gray-50 last:border-0 hover:bg-gray-50/40 transition-colors">
                      <div className="w-8 h-8 rounded-full flex-shrink-0 overflow-hidden">
                        {inv.invitee_avatar
                          ? <img src={inv.invitee_avatar} alt={inv.invitee_login} className="w-full h-full object-cover" />
                          : <div className="w-full h-full bg-amber-100 flex items-center justify-center text-[11px] font-bold text-amber-600 uppercase">{inv.invitee_login[0]}</div>
                        }
                      </div>
                      <div className="flex-1 min-w-0">
                        <p className="text-[12px] font-medium text-gray-900 truncate">{inv.invitee_display_name}</p>
                        <p className="text-[10px] text-gray-400">@{inv.invitee_login} · <span className="capitalize">{inv.role}</span> · {formatNotifTime(inv.created_at)}</p>
                      </div>
                      <div className="flex items-center gap-1.5 flex-shrink-0">
                        <span className="px-2 py-0.5 rounded-md bg-amber-50 text-amber-600 text-[10px] font-semibold">Pending</span>
                        <button onClick={() => handleCancelInvite(inv.id)}
                          className="text-[10px] px-2 py-0.5 rounded-md font-medium bg-gray-100 text-gray-500 hover:bg-red-50 hover:text-red-600 transition-colors">
                          Cancel
                        </button>
                      </div>
                    </div>
                  ))}
                </div>
              )}

              {/* Invite form — owner/admin, active sub-tab only */}
              {membersSubTab === 'active' && canAdmin && (
                <div className="border-t border-gray-100 p-4 flex-shrink-0 bg-gray-50/40">
                  <p className="text-[11px] font-semibold text-gray-700 mb-3 flex items-center gap-1.5">
                    <UserPlus size={12} className="text-gray-500" /> Invite Member
                  </p>
                  <form onSubmit={handleInviteSubmit} className="space-y-2.5">
                    <div className="flex gap-2">
                      <input
                        type="text" value={inviteInput}
                        onChange={e => { setInviteInput(e.target.value); setInviteFoundUser(null); setInviteNotFound(false); }}
                        onKeyDown={e => { if (e.key === 'Enter') { e.preventDefault(); handleFindUser(); } }}
                        placeholder="Username or email"
                        disabled={inviteLoading || inviteSuccess}
                        className="flex-1 min-w-0 px-3 py-1.5 text-[12px] border border-gray-200 rounded-lg bg-white focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent transition-all"
                      />
                      <button type="button" onClick={handleFindUser}
                        disabled={inviteSearching || !inviteInput.trim() || inviteLoading || inviteSuccess}
                        className="px-3 py-1.5 text-[12px] font-medium bg-gray-100 text-gray-700 rounded-lg hover:bg-gray-200 disabled:opacity-40 transition-colors flex-shrink-0">
                        {inviteSearching ? '…' : 'Find'}
                      </button>
                    </div>
                    {inviteFoundUser && (
                      <div className="flex items-center gap-2 px-2.5 py-1.5 rounded-lg bg-green-50 border border-green-100">
                        <div className="w-6 h-6 rounded-full overflow-hidden flex-shrink-0">
                          {inviteFoundUser.avatar
                            ? <img src={inviteFoundUser.avatar} className="w-full h-full object-cover" alt="" />
                            : <div className="w-full h-full bg-green-200 flex items-center justify-center text-[9px] font-bold text-green-700 uppercase">{inviteFoundUser.login[0]}</div>
                          }
                        </div>
                        <div className="min-w-0 flex-1">
                          <p className="text-[11px] font-medium text-gray-900 truncate">{inviteFoundUser.displayName}</p>
                          <p className="text-[9px] text-gray-500">@{inviteFoundUser.login}</p>
                        </div>
                        <Check size={11} className="text-green-500 flex-shrink-0" />
                      </div>
                    )}
                    {inviteNotFound && (
                      <div className="flex items-center gap-1.5 px-2.5 py-1.5 rounded-lg bg-red-50 border border-red-100">
                        <AlertCircle size={11} className="text-red-400 flex-shrink-0" />
                        <p className="text-[11px] text-red-500">User not found</p>
                      </div>
                    )}
                    <div className="flex gap-1 bg-gray-100 rounded-lg p-0.5">
                      {(['member', 'admin'] as const).map(r => (
                        <button key={r} type="button" onClick={() => setInviteRole(r)}
                          disabled={inviteLoading || inviteSuccess}
                          className={`flex-1 py-1 rounded-md text-[11px] font-medium capitalize transition-colors ${inviteRole === r ? 'bg-white text-gray-900 shadow-sm' : 'text-gray-500 hover:text-gray-700'}`}>
                          {r}
                        </button>
                      ))}
                    </div>
                    {inviteError && (
                      <div className="flex items-center gap-1.5 px-2.5 py-1.5 rounded-lg bg-red-50 border border-red-100">
                        <AlertCircle size={11} className="text-red-400 flex-shrink-0" />
                        <p className="text-[11px] text-red-500">{inviteError}</p>
                      </div>
                    )}
                    {inviteSuccess && (
                      <div className="flex items-center gap-1.5 px-2.5 py-1.5 rounded-lg bg-green-50 border border-green-100">
                        <Check size={11} className="text-green-500 flex-shrink-0" />
                        <p className="text-[11px] text-green-600">Invitation sent!</p>
                      </div>
                    )}
                    <button type="submit" disabled={inviteLoading || inviteSuccess || !inviteFoundUser}
                      className="w-full py-1.5 text-[12px] font-medium bg-gray-900 text-white hover:bg-gray-700 rounded-lg disabled:opacity-40 transition-colors">
                      {inviteLoading ? 'Sending…' : 'Send Invite'}
                    </button>
                  </form>
                </div>
              )}
            </div>
          )}

          {/* General tab */}
          {manageTab === 'general' && canAdmin && (
            <div className="flex-1 overflow-y-auto p-5 space-y-5">
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Workspace Name</label>
                <input type="text" value={wsEditName} onChange={e => setWsEditName(e.target.value)}
                  className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent transition-all bg-gray-50"
                />
              </div>
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Description</label>
                <textarea value={wsEditDesc} onChange={e => setWsEditDesc(e.target.value)} rows={3}
                  className="w-full px-3 py-2 text-[12px] border border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent transition-all bg-gray-50 resize-none"
                />
              </div>
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5 flex items-center gap-1.5">
                  <CalendarDays size={11} /> Deadline <span className="font-normal text-gray-400">(optional)</span>
                </label>
                <div className="flex gap-2">
                  <input type="date" value={wsEditDeadline} onChange={e => setWsEditDeadline(e.target.value)}
                    min={new Date().toISOString().slice(0, 10)}
                    className="flex-1 px-3 py-2 text-[12px] border border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent transition-all bg-gray-50 text-gray-700"
                  />
                  {wsEditDeadline && (
                    <button type="button" onClick={() => setWsEditDeadline('')}
                      className="px-2.5 py-2 text-[11px] text-gray-400 hover:text-gray-700 hover:bg-gray-100 rounded-lg transition-colors" title="Clear deadline">
                      <X size={13} />
                    </button>
                  )}
                </div>
              </div>
              <button onClick={handleSaveGeneral} disabled={generalSaving || !wsEditName.trim()}
                className="w-full py-2 text-[12px] font-medium rounded-lg transition-colors disabled:opacity-40 bg-gray-900 text-white hover:bg-gray-700">
                {generalSaving ? 'Saving…' : generalSaved ? 'Saved ✓' : 'Save Changes'}
              </button>
            </div>
          )}

          {/* Leave & Delete tab */}
          {manageTab === 'danger' && (
            <div className="flex-1 overflow-y-auto p-5 space-y-4">
              {/* Mark as completed */}
              <div className="rounded-xl border border-gray-200 p-4">
                <p className="text-[13px] font-semibold text-gray-900 mb-1">
                  {workspace.is_completed ? 'Reopen Workspace' : 'Mark as Completed'}
                </p>
                <p className="text-[11px] text-gray-500 mb-4 leading-relaxed">
                  {workspace.is_completed
                    ? 'Mark this workspace as active again. It will reappear in your main list.'
                    : 'Mark this workspace as finished. It will be hidden from the main list and shown in gray.'}
                </p>
                <button
                  onClick={() => onToggleCompleted(workspaceId)}
                  className={`flex items-center gap-2 px-4 py-2 text-[12px] font-medium rounded-lg transition-colors ${
                    workspace.is_completed
                      ? 'text-indigo-600 bg-indigo-50 hover:bg-indigo-100'
                      : 'text-green-700 bg-green-50 hover:bg-green-100'
                  }`}
                >
                  {workspace.is_completed
                    ? <><RotateCcw size={13} /> Reopen Workspace</>
                    : <><CheckCircle2 size={13} /> Mark as Completed</>}
                </button>
              </div>

              {members.length > 1 && (
                <div className="rounded-xl border border-gray-200 p-4">
                  <p className="text-[13px] font-semibold text-gray-900 mb-1">Leave Workspace</p>
                  <p className="text-[11px] text-gray-500 mb-4 leading-relaxed">
                    {myRole === 'owner'
                      ? 'As the owner, you must transfer ownership to another member before leaving.'
                      : 'You will lose access to this workspace and all its content.'}
                  </p>
                  <button onClick={onLeaveRequest}
                    className="flex items-center gap-2 px-4 py-2 text-[12px] font-medium text-red-600 bg-red-50 hover:bg-red-100 rounded-lg transition-colors">
                    <LogOut size={13} />
                    {myRole === 'owner' ? 'Transfer & Leave' : 'Leave Workspace'}
                  </button>
                </div>
              )}
              {myRole === 'owner' && (
                <div className="rounded-xl border border-red-200 bg-red-50/30 p-4">
                  <p className="text-[13px] font-semibold text-red-700 mb-1">Delete Workspace</p>
                  <p className="text-[11px] text-gray-500 mb-4 leading-relaxed">
                    Permanently delete this workspace. All tasks, files, and messages will be lost and this cannot be undone.
                  </p>
                  <button onClick={onDeleteRequest}
                    className="flex items-center gap-2 px-4 py-2 text-[12px] font-medium text-white bg-red-500 hover:bg-red-600 rounded-lg transition-colors">
                    <Trash2 size={13} />
                    Delete Workspace
                  </button>
                </div>
              )}
            </div>
          )}
        </div>
      </div>
    </>
  );
}
