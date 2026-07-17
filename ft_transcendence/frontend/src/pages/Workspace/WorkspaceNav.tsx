import Logo42 from "../../components/Logo42";
import React from 'react';
import { useNavigate } from 'react-router-dom';
import { LayoutDashboard, FolderKanban, CheckSquare, MessageSquare } from 'lucide-react';

interface Props {
  onBackToList: () => void;
}

export default function WorkspaceNav({ onBackToList }: Props) {
  const navigate = useNavigate();
  return (
    <aside className="w-52 flex-shrink-0 flex flex-col bg-white border-r border-gray-100">
      <div className="h-[52px] px-4 flex items-center gap-2.5 border-b border-gray-100 flex-shrink-0">
        <div className="w-[26px] h-[26px] rounded-[6px] bg-gray-900 flex items-center justify-center flex-shrink-0">
          <Logo42 size={12} />
        </div>
        <span className="text-[13px] font-semibold tracking-tight">Collab42</span>
      </div>
      <nav className="flex-1 px-2 py-3 space-y-0.5">
        <button onClick={() => navigate('/dashboard')}
          className="w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium text-gray-400 hover:text-gray-700 hover:bg-gray-50 transition-colors cursor-pointer">
          <LayoutDashboard size={15} /> Dashboard
        </button>
        <button onClick={onBackToList}
          className="w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium bg-gray-100 text-gray-900 transition-colors cursor-pointer">
          <FolderKanban size={15} /> Workspace
        </button>
        <button onClick={() => navigate('/tasks')}
          className="w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium text-gray-400 hover:text-gray-700 hover:bg-gray-50 transition-colors cursor-pointer">
          <CheckSquare size={15} /> Tasks
        </button>
        <button onClick={() => navigate('/chats')}
          className="w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium text-gray-400 hover:text-gray-700 hover:bg-gray-50 transition-colors cursor-pointer">
          <MessageSquare size={15} /> Chats
        </button>
      </nav>
    </aside>
  );
}
