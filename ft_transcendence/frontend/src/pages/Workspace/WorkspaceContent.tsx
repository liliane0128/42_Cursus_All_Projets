import { useState, useRef, useEffect } from 'react';
import {
  MessageSquare, Plus, Maximize2, Minimize2,
  FolderOpen, Pencil, Trash2
} from 'lucide-react';
import { SharedFiles } from './SharedFilesPanel';
import TasksPanel, { Task, TaskStatus, Priority } from './TasksPanel';
import DashboardOverview from './DashboardOverview';
import { WorkspaceMember } from '../../api';
import { Workspace } from './WorkspaceTypes';

type FocusMode = 'none' | 'tasks' | 'chat' | 'files';

interface Props {
  workspace: Workspace;
  tasks: Task[];
  members: WorkspaceMember[];
  currentUserId: string;
  onAddTask: (data: { title: string; workspace_id: number; status: TaskStatus; priority: Priority; due_date?: string }) => void;
  onUpdateTask: (id: number, data: Partial<Task>) => void;
  onDeleteTask: (id: number) => void;
  onSendMessage: (text: string) => void;
  onOpenFullChat: () => void;
  onEditMessage: (messageId: string, content: string) => void;
  onDeleteMessage: (messageId: string) => void;
  onFileUpload: (file: File) => Promise<void>;
  onFileDelete: (id: number) => void;
}

export default function WorkspaceContent({
  workspace, tasks, members, currentUserId,
  onAddTask, onUpdateTask, onDeleteTask,
  onSendMessage, onOpenFullChat, onEditMessage, onDeleteMessage,
  onFileUpload, onFileDelete,
}: Props) {
  const [focusMode, setFocusMode] = useState<FocusMode>('none');
  const [message,   setMessage]   = useState('');
  const [editingId, setEditingId] = useState<string | null>(null);
  const [editText,  setEditText]  = useState('');
  const uploadTriggerRef = useRef<(() => void) | null>(null);
  const messagesEndRef   = useRef<HTMLDivElement>(null);
  const isInitialLoad   = useRef(true);
  const prevChatId      = useRef<string | null>(null);

  useEffect(() => {
    const chatId = workspace.id + '';
    if (prevChatId.current !== chatId) {
      prevChatId.current = chatId;
      isInitialLoad.current = true;
    }
    if (workspace.messages.length === 0) return;
    if (isInitialLoad.current) {
      isInitialLoad.current = false;
      messagesEndRef.current?.scrollIntoView({ behavior: 'instant' });
    } else {
      messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
    }
  }, [workspace.messages.length, workspace.id]);

  useEffect(() => {
    function handleKeyDown(e: KeyboardEvent) {
      if ((e.metaKey || e.ctrlKey) && e.key === '1') {
        e.preventDefault();
        setFocusMode(f => f === 'tasks' ? 'none' : 'tasks');
      }
      if ((e.metaKey || e.ctrlKey) && e.key === '2') {
        e.preventDefault();
        setFocusMode(f => f === 'files' ? 'none' : 'files');
      }
      if ((e.metaKey || e.ctrlKey) && e.key === '3') {
        e.preventDefault();
        setFocusMode(f => f === 'chat' ? 'none' : 'chat');
      }
      if (e.key === 'Escape') { setFocusMode('none'); setEditingId(null); }
    }
    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, []);

  const handleSend = () => {
    if (!message.trim()) return;
    onSendMessage(message.trim());
    setMessage('');
  };

  const handleSaveEdit = (msgId: string) => {
    const trimmed = editText.trim();
    if (trimmed) onEditMessage(msgId, trimmed);
    setEditingId(null);
  };

  return (
    <div className="flex-1 flex flex-col overflow-hidden bg-[#f5f5f3]">
      {/* Stats bar */}
      <div className="px-6 pt-5 flex-shrink-0 mb-3">
        <DashboardOverview tasks={tasks} members={members} />
      </div>

      {/* Panels row — all three panels are siblings at the same flex level */}
      <div className="flex-1 flex overflow-hidden min-h-0">

        {/* Tasks panel */}
        {focusMode !== 'files' && focusMode !== 'chat' && (
          <div className={`flex flex-col pb-5 overflow-y-auto bg-[#f5f5f3] px-6 transition-all duration-300 ease-in-out ${
            focusMode === 'tasks' ? 'flex-1' : 'flex-none w-[58%]'
          }`}>
            <div className="flex flex-col flex-1 bg-white rounded-xl p-4 relative min-h-0">
              <div className="absolute top-4 right-4 z-10">
                <button onClick={() => setFocusMode(focusMode === 'tasks' ? 'none' : 'tasks')}
                  className="w-6 h-6 rounded-md border border-gray-100 flex items-center justify-center bg-white text-gray-400 hover:text-gray-900 hover:bg-gray-50 transition-colors">
                  {focusMode === 'tasks' ? <Minimize2 size={12} /> : <Maximize2 size={12} />}
                </button>
              </div>
              <TasksPanel
                tasks={tasks}
                workspaceId={workspace.id}
                workspaceDeadline={workspace.deadline}
                onAddTask={onAddTask}
                onUpdateTask={onUpdateTask}
                onDeleteTask={onDeleteTask}
              />
            </div>
          </div>
        )}

        {/* Right column: Files + Chat (hidden when tasks is focused) */}
        {focusMode !== 'tasks' && (
          <div className={`flex flex-col pb-5 overflow-hidden bg-[#f5f5f3] px-6 ${
            focusMode === 'none' ? 'flex-none w-[42%]' : 'flex-1'
          }`}>

            {/* Files panel */}
            {focusMode !== 'chat' && (
              <div className={`bg-white border border-gray-100 rounded-xl p-4 flex flex-col relative overflow-hidden min-h-0 ${
                focusMode === 'none' ? 'flex-1 mb-4' : 'flex-1'
              }`}>
                <div className="absolute top-4 right-4 z-10">
                  <button onClick={() => setFocusMode(focusMode === 'files' ? 'none' : 'files')}
                    className="w-6 h-6 rounded-md border border-gray-100 flex items-center justify-center bg-white text-gray-400 hover:text-gray-900 hover:bg-gray-50 transition-colors">
                    {focusMode === 'files' ? <Minimize2 size={12} /> : <Maximize2 size={12} />}
                  </button>
                </div>
                <div className="flex items-center justify-between mb-3 flex-shrink-0 pr-8">
                  <h2 className="text-xs font-bold text-gray-800 mb-3 flex items-center gap-2 flex-shrink-0">
                    <FolderOpen size={13} className="text-gray-500" /> Shared Files
                  </h2>
                  <button onClick={() => uploadTriggerRef.current?.()}
                    className="bg-gray-950 hover:bg-gray-800 text-white font-semibold text-[11px] px-3 py-1.5 rounded-lg flex items-center gap-1 transition-all shadow-xs active:scale-95 cursor-pointer">
                    <Plus size={12} strokeWidth={2.5} /> Upload
                  </button>
                </div>
                <div className="flex-1 min-h-0 overflow-hidden">
                  <SharedFiles files={workspace.files} onUpload={onFileUpload} onDelete={onFileDelete} uploadTriggerRef={uploadTriggerRef} />
                </div>
              </div>
            )}

            {/* Chat panel */}
            {focusMode !== 'files' && (
              <div className="bg-white border border-gray-100 rounded-xl p-4 flex flex-col relative overflow-hidden min-h-0 flex-1">
                <div className="absolute top-4 right-4 z-10">
                  <button onClick={() => setFocusMode(focusMode === 'chat' ? 'none' : 'chat')}
                    className="w-6 h-6 rounded-md border border-gray-100 flex items-center justify-center bg-white text-gray-400 hover:text-gray-900 hover:bg-gray-50 transition-colors">
                    {focusMode === 'chat' ? <Minimize2 size={12} /> : <Maximize2 size={12} />}
                  </button>
                </div>
                <div className="flex items-center mb-3 flex-shrink-0 pr-8">
                  <h2 className="text-xs font-bold text-gray-800 flex items-center gap-2">
                    <span className="relative group/chaticon">
                      <button onClick={onOpenFullChat} className="flex items-center justify-center text-gray-400 hover:text-indigo-600 transition-colors cursor-pointer">
                        <MessageSquare size={13} />
                      </button>
                      <span className="pointer-events-none absolute left-0 top-5 z-20 w-max max-w-[160px] rounded-md bg-gray-100 border border-gray-200 px-2 py-1 text-[10px] text-gray-600 opacity-0 group-hover/chaticon:opacity-100 transition-opacity duration-150 whitespace-nowrap">
                        Open in full chat
                      </span>
                    </span>
                    Channel Chat
                  </h2>
                </div>

                {/* Messages */}
                <div className="flex-1 bg-gray-50/50 border border-gray-100 rounded-xl p-3 overflow-y-auto space-y-3 text-[11px] mb-3 min-h-0">
                  {workspace.messages.length === 0 && (
                    <p className="text-gray-300 text-center mt-4">No messages yet</p>
                  )}
                  {workspace.messages.map(msg => {
                    const isMe = msg.senderId === currentUserId;
                    const isEditing = editingId === msg.id;
                    return (
                      <div key={msg.id} className={`group ${isMe ? 'text-right' : 'text-left'}`}>
                        <span className="font-bold block text-[9px] mb-0.5 text-gray-800">
                          {msg.user}
                        </span>

                        {isEditing ? (
                          <div className="inline-block text-left max-w-[85%]">
                            <input
                              autoFocus
                              value={editText}
                              onChange={e => setEditText(e.target.value)}
                              onKeyDown={e => {
                                if (e.key === 'Enter') handleSaveEdit(msg.id);
                                if (e.key === 'Escape') setEditingId(null);
                              }}
                              className="w-full px-3 py-1.5 border border-indigo-300 rounded-xl text-[11px] outline-none focus:border-indigo-500 bg-white"
                            />
                            <div className="flex justify-end gap-1 mt-1">
                              <button onClick={() => setEditingId(null)}
                                className="text-[10px] text-gray-400 hover:text-gray-600 px-2 py-0.5 rounded transition-colors">
                                Cancel
                              </button>
                              <button onClick={() => handleSaveEdit(msg.id)}
                                className="text-[10px] bg-indigo-600 hover:bg-indigo-700 text-white px-2 py-0.5 rounded font-medium transition-colors">
                                Save
                              </button>
                            </div>
                          </div>
                        ) : (
                          <div className={`inline-block px-3 py-1.5 rounded-xl border border-gray-100 shadow-2xs max-w-[85%] text-left
                            ${isMe ? 'bg-indigo-50 text-gray-800 border-indigo-100' : 'bg-gray-100 text-gray-800 border-gray-100'}`}>
                            <p className="break-words">{msg.text}</p>
                            <div className={`flex items-center gap-1 mt-1 ${isMe ? 'justify-end' : ''}`}>
                              <span className="text-[9px] text-gray-400">{msg.createdAt}</span>
                              {msg.edited && (
                                <span className="text-[9px] text-gray-400">(edited)</span>
                              )}
                            </div>
                          </div>
                        )}

                        {isMe && !isEditing && (
                          <div className="hidden group-hover:flex justify-end gap-1 mt-1">
                            <button
                              onClick={() => { setEditingId(msg.id); setEditText(msg.text); }}
                              className="w-5 h-5 flex items-center justify-center rounded bg-gray-100 hover:bg-indigo-50 text-gray-400 hover:text-indigo-600 transition-colors"
                            >
                              <Pencil size={9} />
                            </button>
                            <button
                              onClick={() => onDeleteMessage(msg.id)}
                              className="w-5 h-5 flex items-center justify-center rounded bg-gray-100 hover:bg-red-50 text-gray-400 hover:text-red-500 transition-colors"
                            >
                              <Trash2 size={9} />
                            </button>
                          </div>
                        )}
                      </div>
                    );
                  })}
                  <div ref={messagesEndRef} />
                </div>

                {/* Input */}
                <div className="relative flex-shrink-0">
                  <input
                    type="text"
                    placeholder="Type a message…"
                    value={message}
                    onChange={e => setMessage(e.target.value)}
                    onKeyDown={e => e.key === 'Enter' && handleSend()}
                    className="w-full bg-white border border-gray-200 rounded-xl py-2 pl-4 pr-14 text-[11px] outline-none focus:border-indigo-500 transition-all"
                  />
                  <button onClick={handleSend}
                    className="absolute right-3 top-2 text-indigo-600 hover:text-indigo-800 font-bold text-[11px] cursor-pointer">
                    Send
                  </button>
                </div>
              </div>
            )}

          </div>
        )}

      </div>
    </div>
  );
}
