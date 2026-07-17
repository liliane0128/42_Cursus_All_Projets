import UserMenu from "../../components/UserMenu";
import { Settings, ArrowLeft, CalendarDays } from 'lucide-react';
import { useTasks } from '../../context/TaskContext';
import { Workspace } from './WorkspaceTypes';
import NotificationPanel from "../../components/NotificationPanel";

interface Props {
  workspace: Workspace | null;
  workspaceCount: number;
  onBack: () => void;
  onManage: () => void;
  onAcceptInvite?: (workspaceId: number) => void;
}

export default function WorkspaceHeader({ workspace, workspaceCount, onBack, onManage }: Props) {
  const { refreshWorkspaces: loadWorkspacesFromBackend } = useTasks();

  return (
    <header className="h-[52px] bg-white border-b border-gray-100 flex items-center justify-between px-6 flex-shrink-0">
      <div className="flex items-center gap-3">
        {workspace && (
          <button onClick={onBack}
            className="w-[28px] h-[28px] flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors mr-1">
            <ArrowLeft size={14} />
          </button>
        )}
        <div>
          <h1 className="text-[14px] font-semibold text-gray-900 leading-tight">
            {workspace ? workspace.name : 'All Workspaces'}
          </h1>
          <div className="flex items-center gap-2 text-[11px] text-gray-400">
            <span>{workspace ? workspace.description : `${workspaceCount} workspaces`}</span>
            {workspace?.deadline && (() => {
              const d = new Date(workspace.deadline);
              const overdue = d < new Date();
              const label = d.toLocaleDateString("en-US", { month: 'short', day: 'numeric', year: 'numeric' });
              return (
                <span className={`flex items-center gap-1 font-medium ${overdue ? 'text-red-400' : 'text-gray-400'}`}>
                  · <CalendarDays size={10} /> {label}
                </span>
              );
            })()}
          </div>
        </div>
      </div>

      <div className="flex items-center gap-1">
        {workspace && (
          <button onClick={onManage}
            className="flex items-center gap-1.5 px-3 py-1.5 bg-gray-100 text-gray-700 rounded-lg text-[12px] font-medium hover:bg-gray-200 transition-colors">
            <Settings size={13} />
            Manage Workspace
          </button>
        )}

        <NotificationPanel onAcceptInvite={() => loadWorkspacesFromBackend()} />
        <UserMenu />
      </div>
    </header>
  );
}
