import { useState, useEffect } from 'react';
import { AlertCircle } from 'lucide-react';
import { useLocation, useNavigate } from 'react-router-dom';
import { useAuth } from '../../context/AuthContext';
import { useChat, ChatMessage } from '../../context/ChatContext';
import { useTasks } from '../../context/TaskContext';
import {
  createWorkspace, deleteWorkspace, updateWorkspaceApi, inviteMember,
  getWorkspaceMembers, kickMember, leaveWorkspace, transferOwnership, setMemberRole,
  getWorkspaceFiles, uploadWorkspaceFile, deleteWorkspaceFile,
  WorkspaceMember, mediaUrl
} from '../../api';
import CreateWorkspaceModal from './CreateWorkspaceModal';
import WorkspaceNav from './WorkspaceNav';
import WorkspaceHeader from './WorkspaceHeader';
import WorkspaceList from './WorkspaceList';
import WorkspaceContent from './WorkspaceContent';
import ManageWorkspaceSidebar from './ManageWorkspaceSidebar';
import WorkspaceConfirmModals from './WorkspaceConfirmModals';
import { Workspace } from './WorkspaceTypes';

export default function WorkspacePage() {
  const { user, token } = useAuth();
  const location = useLocation();
  const navigate = useNavigate();
  const { sendMessage, projectChats, openChat, refreshChannels, sendEditMessage, setProjectChats } = useChat();
  const {
    workspaces: apiWorkspaces,
    tasks: allTasks,
    addTask,
    updateTask: ctxUpdateTask,
    deleteTask: ctxDeleteTask,
    refreshWorkspaces: loadWorkspacesFromBackend,
  } = useTasks();

  // ── Workspace list ────────────────────────────────────────────────────────
  const [workspaces, setWorkspaces] = useState<Workspace[]>([]);

  useEffect(() => {
    setWorkspaces(prev => apiWorkspaces.map(ws => {
      const existing = prev.find(w => w.id === ws.id);
      return {
        id: ws.id,
        name: ws.name,
        description: ws.description,
        deadline: ws.deadline ?? null,
        is_completed: ws.is_completed ?? false,
        tasks: allTasks.filter(t => t.workspace_id === ws.id).map(t => ({
          id: t.id, title: t.title, status: t.status, priority: t.priority, dueDate: t.dueDate,
        })),
        files: existing?.files ?? [],
        messages: [],
      };
    }));
  }, [apiWorkspaces, allTasks]);

  // ── Active workspace ──────────────────────────────────────────────────────
  const [activeWorkspaceId, setActiveWorkspaceId] = useState<number | null>(() => {
    const fromState = (location.state as any)?.workspaceId ?? null;
    if (fromState) return fromState;
    const navType = (performance.getEntriesByType('navigation')[0] as PerformanceNavigationTiming | undefined)?.type;
    if (navType === 'reload') {
      const stored = sessionStorage.getItem('activeWorkspaceId');
      return stored ? Number(stored) : null;
    }
    return null;
  });

  useEffect(() => {
    if (activeWorkspaceId !== null) {
      sessionStorage.setItem('activeWorkspaceId', String(activeWorkspaceId));
    } else {
      sessionStorage.removeItem('activeWorkspaceId');
    }
  }, [activeWorkspaceId]);

  const activeWorkspace = workspaces.find(w => w.id === activeWorkspaceId) ?? null;
  const tasks           = activeWorkspace?.tasks ?? [];

  const workspaceChatId   = activeWorkspaceId ? `workspace_${activeWorkspaceId}` : null;
  const workspaceMessages = projectChats.find(c => c.id === workspaceChatId)?.messages ?? [];
  const chatMessages = workspaceMessages.map(m => ({
    id: m.id,
    user: m.senderName,
    senderId: m.senderId,
    text: m.content,
    createdAt: m.createdAt,
    edited: m.edited ?? false,
  }));

  // ── Members ───────────────────────────────────────────────────────────────
  const [members, setMembers] = useState<WorkspaceMember[]>([]);

  useEffect(() => {
    if (!activeWorkspaceId || !user) { setMembers([]); return; }
    getWorkspaceMembers(activeWorkspaceId).then(setMembers).catch(() => setMembers([]));
  }, [activeWorkspaceId, user?.id]); // eslint-disable-line react-hooks/exhaustive-deps

  useEffect(() => {
    if (!user) return;
    setMembers(prev => prev.map(m =>
      m.user_id === user.id ? { ...m, avatar: user.avatar ?? null } : m
    ));
  }, [user?.avatar]); // eslint-disable-line react-hooks/exhaustive-deps

  useEffect(() => {
    if (!activeWorkspaceId || apiWorkspaces.length === 0) return;
    getWorkspaceFiles(activeWorkspaceId).then(apiFiles => {
      setWorkspaces(prev => prev.map(w =>
        w.id === activeWorkspaceId
          ? { ...w, files: apiFiles.map(f => ({ id: f.id, name: f.name, size: f.size, type: '', url: mediaUrl(f.url) ?? f.url, uploadedAt: f.uploadedAt })) }
          : w
      ));
    }).catch(() => {});
  }, [activeWorkspaceId, apiWorkspaces.length]); // eslint-disable-line react-hooks/exhaustive-deps

  useEffect(() => {
    if (!workspaceChatId) return;
    openChat(workspaceChatId);
  }, [workspaceChatId]); // eslint-disable-line react-hooks/exhaustive-deps

  const myRoleInWorkspace = members.find(m => m.user_id === user?.id)?.role ?? null;

  // ── Manage sidebar ────────────────────────────────────────────────────────
  const [isManageOpen,     setIsManageOpen]     = useState(false);
  const [manageDefaultTab, setManageDefaultTab] = useState<'members' | 'general' | 'danger'>('members');

  const handleOpenManage = (tab: 'members' | 'general' | 'danger' = 'members') => {
    setManageDefaultTab(tab);
    setIsManageOpen(true);
  };

  // ── Confirm modal state ───────────────────────────────────────────────────
  const [kickConfirm,         setKickConfirm]         = useState<{ userId: number; login: string } | null>(null);
  const [leaveConfirm,        setLeaveConfirm]        = useState(false);
  const [roleConfirm,         setRoleConfirm]         = useState<{ userId: number; login: string; newRole: 'admin' | 'member' } | null>(null);
  const [isTransferLeaveOpen, setIsTransferLeaveOpen] = useState(false);
  const [transferTargetId,    setTransferTargetId]    = useState<number | null>(null);
  const [showDeleteConfirm,   setShowDeleteConfirm]   = useState(false);
  const [pendingDeleteId,     setPendingDeleteId]     = useState<number | null>(null);
  const [showCreateModal,     setShowCreateModal]     = useState(false);
  const [incompleteAlert,     setIncompleteAlert]     = useState<{ id: number; title: string; status: string }[] | null>(null);

  useEffect(() => {
    if ((location.state as any)?.openCreateWorkspace) {
      setActiveWorkspaceId(null);
      setShowCreateModal(true);
      navigate(location.pathname, { replace: true, state: {} });
    }
  }, []); // eslint-disable-line react-hooks/exhaustive-deps

  // ── Workspace CRUD handlers ───────────────────────────────────────────────
  const updateWorkspace = (id: number, updater: (ws: Workspace) => Workspace) =>
    setWorkspaces(prev => prev.map(w => w.id === id ? updater(w) : w));

  const handleCreateWorkspace = async (name: string, description: string, deadline: string | null, inviteInputVal?: string) => {
    const isDuplicate = workspaces.some(ws => ws.name.toLowerCase() === name.toLowerCase());
    if (isDuplicate) throw new Error(`A workspace named "${name}" already exists.`);
    try {
      const res = await createWorkspace({ name, description: description || 'New collaborative workspace', deadline });
      if (res?.id) {
        setWorkspaces(prev => [...prev, {
          id: res.id, name: res.name || name,
          description: res.description || description || 'New collaborative workspace',
          deadline: res.deadline ?? null,
          is_completed: false,
          tasks: [], files: [], messages: [],
        }]);
        setActiveWorkspaceId(res.id);
        // Sync to TaskContext so Dashboard updates immediately
        await loadWorkspacesFromBackend();
        if (inviteInputVal?.trim()) {
          try { await inviteMember(res.id, inviteInputVal.trim()); } catch {}
        }
      } else {
        await loadWorkspacesFromBackend();
      }
    } catch (err: any) {
      throw new Error(err.response?.data?.detail || err.message || 'Create workspace failed');
    }
    refreshChannels();
    setShowCreateModal(false);
  };

  const handleDeleteWorkspace = async (id: number) => {
    try {
      await deleteWorkspace(id);
      if (activeWorkspaceId === id) {
        setActiveWorkspaceId(null);
        setIsManageOpen(false);
      }
      await loadWorkspacesFromBackend();
    } catch (err) {
      console.error('delete workspace failed', err);
      setWorkspaces(prev => prev.filter(w => w.id !== id));
    } finally {
      setShowDeleteConfirm(false);
    }
  };

  const handleToggleCompleted = async (id: number) => {
    const ws = workspaces.find(w => w.id === id);
    if (!ws) return;
    const next = !ws.is_completed;

    if (next) {
      const incomplete = allTasks
        .filter(t => t.workspace_id === id && t.status !== 'completed')
        .map(t => ({ id: t.id, title: t.title, status: t.status as string }));
      if (incomplete.length > 0) {
        setIncompleteAlert(incomplete);
        return;
      }
    }

    setWorkspaces(prev => prev.map(w => w.id === id ? { ...w, is_completed: next } : w));
    try {
      await updateWorkspaceApi(id, { is_completed: next });
    } catch (err) {
      console.error('toggleCompleted failed', err);
      setWorkspaces(prev => prev.map(w => w.id === id ? { ...w, is_completed: !next } : w));
    }
  };

  // ── Member action handlers ────────────────────────────────────────────────
  const handleKick = async (targetUserId: number) => {
    if (!activeWorkspaceId) return;
    try {
      await kickMember(activeWorkspaceId, targetUserId);
      setMembers(prev => prev.filter(m => m.user_id !== targetUserId));
    } catch (err: any) {
      console.error('Kick failed:', err);
    }
  };

  const handleLeave = async () => {
    if (!activeWorkspaceId || !user) return;
    const myRole = members.find(m => m.user_id === user.id)?.role;
    if (myRole === 'owner') { setIsTransferLeaveOpen(true); return; }
    try {
      await leaveWorkspace(activeWorkspaceId);
      setIsManageOpen(false);
      await loadWorkspacesFromBackend();
      setActiveWorkspaceId(null);
    } catch (err: any) {
      console.error('Leave failed:', err);
    }
  };

  const handleTransferAndLeave = async () => {
    if (!activeWorkspaceId || transferTargetId === null) return;
    try {
      await transferOwnership(activeWorkspaceId, transferTargetId);
      setMembers(prev => prev.map(m => {
        if (m.user_id === user?.id) return { ...m, role: 'member' as const };
        if (m.user_id === transferTargetId) return { ...m, role: 'owner' as const };
        return m;
      }));
      setIsTransferLeaveOpen(false);
      setTransferTargetId(null);
      await leaveWorkspace(activeWorkspaceId);
      setIsManageOpen(false);
      await loadWorkspacesFromBackend();
      setActiveWorkspaceId(null);
    } catch (err: any) {
      console.error('Transfer & leave failed:', err);
    }
  };

  const handleSetRole = async (targetUserId: number, role: 'admin' | 'member') => {
    if (!activeWorkspaceId) return;
    try {
      await setMemberRole(activeWorkspaceId, targetUserId, role);
      setMembers(prev => prev.map(m => m.user_id === targetUserId ? { ...m, role } : m));
    } catch (err: any) {
      console.error('Failed to update role:', err);
    }
  };

  // ── Chat handlers ────────────────────────────────────────────────────────
  const handleOpenFullChat = () => {
    if (workspaceChatId) openChat(workspaceChatId);
    navigate('/chats');
  };

  // ── Chat message handlers ─────────────────────────────────────────────────
  const handleEditMessage = (messageId: string, content: string) => {
    if (!workspaceChatId) return;
    sendEditMessage(workspaceChatId, messageId, content);
  };

  const handleDeleteMessage = async (messageId: string) => {
    if (!workspaceChatId || !token) return;
    setProjectChats(prev => prev.map(c =>
      c.id === workspaceChatId
        ? { ...c, messages: c.messages.filter(m => m.id !== messageId) }
        : c
    ));
    try {
      await fetch(`/api/messages/${messageId}`, {
        method: 'DELETE',
        headers: { Authorization: `Bearer ${token}` },
      });
    } catch { /* optimistic delete stands */ }
  };

  // ── File handlers ─────────────────────────────────────────────────────────
  const handleFileUpload = async (file: File) => {
    if (!activeWorkspaceId) return;
    const tempId = Date.now();
    const wsId = activeWorkspaceId;
    const sizeStr = file.size < 1024 ? `${file.size} B` : `${(file.size / 1024).toFixed(1)} KB`;
    updateWorkspace(wsId, ws => ({
      ...ws, files: [...ws.files, { id: tempId, name: file.name, size: sizeStr, type: '', status: 'loading' as const, progress: 0 }],
    }));
    try {
      const saved = await uploadWorkspaceFile(wsId, file, (percent) => {
        updateWorkspace(wsId, ws => ({
          ...ws, files: ws.files.map(f => f.id === tempId ? { ...f, progress: percent } : f),
        }));
      });
      updateWorkspace(wsId, ws => ({
        ...ws, files: ws.files.map(f =>
          f.id === tempId
            ? { id: saved.id, name: saved.name, size: saved.size, type: '', url: mediaUrl(saved.url) ?? saved.url, uploadedAt: saved.uploadedAt, status: 'success' as const }
            : f
        ),
      }));
    } catch (err) {
      updateWorkspace(wsId, ws => ({ ...ws, files: ws.files.filter(f => f.id !== tempId) }));
      throw err;
    }
  };

  const handleFileDelete = async (id: number) => {
    if (!activeWorkspaceId) return;
    updateWorkspace(activeWorkspaceId, ws => ({ ...ws, files: ws.files.filter(f => f.id !== id) }));
    try {
      await deleteWorkspaceFile(activeWorkspaceId, id);
    } catch {
      // deletion failed silently; file will reappear on next load
    }
  };

  const handleSendMessage = (text: string) => {
    if (!activeWorkspaceId || !user) return;
    const chatId = `workspace_${activeWorkspaceId}`;
    const msg: ChatMessage = {
      id: crypto.randomUUID?.() ?? `msg-${Date.now()}-${Math.random().toString(36).slice(2)}`,
      senderId: String(user.id),
      senderName: user.displayName || user.login,
      content: text,
      createdAt: new Date().toLocaleTimeString('fr-FR', { hour: '2-digit', minute: '2-digit' }),
      createdAtRaw: new Date().toISOString(),
      type: 'user',
    };
    sendMessage(chatId, msg);
  };

  // ── Render ────────────────────────────────────────────────────────────────
  return (
    <div className="flex h-screen bg-[#f5f5f3] font-sans text-gray-900 overflow-hidden">
      <WorkspaceNav onBackToList={() => setActiveWorkspaceId(null)} />

      <div className="flex-1 flex flex-col min-w-0">
        <WorkspaceHeader
          workspace={activeWorkspace}
          workspaceCount={workspaces.length}
          onBack={() => setActiveWorkspaceId(null)}
          onManage={() => handleOpenManage('members')}
        />

        <div className="flex-1 flex overflow-hidden">
          {!activeWorkspace && (
            <WorkspaceList
              workspaces={workspaces}
              onSelect={id => setActiveWorkspaceId(id)}
              onDeleteRequest={setPendingDeleteId}
              onCreateNew={() => setShowCreateModal(true)}
            />
          )}
          {activeWorkspace && (
            <WorkspaceContent
              workspace={{ ...activeWorkspace, messages: chatMessages }}
              tasks={tasks}
              members={members}
              currentUserId={String(user?.id ?? '')}
              onAddTask={addTask}
              onUpdateTask={ctxUpdateTask}
              onDeleteTask={ctxDeleteTask}
              onSendMessage={handleSendMessage}
              onOpenFullChat={handleOpenFullChat}
              onEditMessage={handleEditMessage}
              onDeleteMessage={handleDeleteMessage}
              onFileUpload={handleFileUpload}
              onFileDelete={handleFileDelete}
            />
          )}
        </div>
      </div>

      {activeWorkspace && (
        <ManageWorkspaceSidebar
          isOpen={isManageOpen}
          onClose={() => setIsManageOpen(false)}
          workspace={activeWorkspace}
          workspaceId={activeWorkspace.id}
          myRole={myRoleInWorkspace}
          currentUserId={user?.id}
          members={members}
          onKickRequest={(userId, login) => setKickConfirm({ userId, login })}
          onRoleChangeRequest={(userId, login, newRole) => setRoleConfirm({ userId, login, newRole })}
          onLeaveRequest={() => setLeaveConfirm(true)}
          onDeleteRequest={() => setShowDeleteConfirm(true)}
          onWorkspaceSaved={(name, desc, deadline) =>
            updateWorkspace(activeWorkspace.id, ws => ({ ...ws, name, description: desc, deadline }))
          }
          onToggleCompleted={handleToggleCompleted}
          loadWorkspacesFromBackend={loadWorkspacesFromBackend}
          defaultTab={manageDefaultTab}
        />
      )}

      <WorkspaceConfirmModals
        kickConfirm={kickConfirm}
        onKickClose={() => setKickConfirm(null)}
        onKickConfirm={userId => { handleKick(userId); setKickConfirm(null); }}
        leaveConfirm={leaveConfirm}
        myRole={myRoleInWorkspace}
        onLeaveClose={() => setLeaveConfirm(false)}
        onLeaveConfirm={() => { setLeaveConfirm(false); handleLeave(); }}
        roleConfirm={roleConfirm}
        onRoleClose={() => setRoleConfirm(null)}
        onRoleConfirm={(userId, newRole) => { handleSetRole(userId, newRole); setRoleConfirm(null); }}
        isTransferLeaveOpen={isTransferLeaveOpen}
        onTransferLeaveClose={() => setIsTransferLeaveOpen(false)}
        transferTargetId={transferTargetId}
        setTransferTargetId={setTransferTargetId}
        members={members}
        currentUserId={user?.id}
        onTransferAndLeave={handleTransferAndLeave}
        showDeleteConfirm={showDeleteConfirm}
        onDeleteClose={() => setShowDeleteConfirm(false)}
        activeWorkspace={activeWorkspace}
        onDeleteConfirm={handleDeleteWorkspace}
        pendingDeleteId={pendingDeleteId}
        workspaces={workspaces}
        onPendingDeleteClose={() => setPendingDeleteId(null)}
        onPendingDeleteConfirm={id => { handleDeleteWorkspace(id); setPendingDeleteId(null); }}
      />

      {showCreateModal && (
        <CreateWorkspaceModal
          onClose={() => setShowCreateModal(false)}
          onCreate={handleCreateWorkspace}
          existingNames={workspaces.map(w => w.name)}
        />
      )}

      {incompleteAlert && (
        <div className="fixed inset-0 bg-black/30 z-50 flex items-center justify-center" onClick={() => setIncompleteAlert(null)}>
          <div className="bg-white rounded-xl border border-gray-100 shadow-xl w-96 p-6" onClick={e => e.stopPropagation()}>
            <h3 className="text-sm font-bold text-gray-900 mb-1 flex items-center gap-2">
              <AlertCircle size={15} className="text-amber-500" />
              Can't mark as completed
            </h3>
            <p className="text-[11px] text-gray-400 mb-4">
              {incompleteAlert.length} task{incompleteAlert.length > 1 ? 's' : ''} still need to be finished:
            </p>
            <div className="space-y-1.5 max-h-52 overflow-y-auto mb-5">
              {incompleteAlert.map(t => (
                <div key={t.id} className="flex items-center gap-2 bg-gray-50 rounded-lg px-3 py-2">
                  <span className="text-[11px] text-gray-700 flex-1 truncate">{t.title}</span>
                  <span className="text-[10px] text-gray-400 capitalize">{t.status.replace('_', ' ')}</span>
                </div>
              ))}
            </div>
            <button
              onClick={() => setIncompleteAlert(null)}
              className="w-full py-2 bg-gray-900 hover:bg-gray-800 text-white text-[11px] font-medium rounded-lg transition-colors"
            >
              Got it
            </button>
          </div>
        </div>
      )}
    </div>
  );
}