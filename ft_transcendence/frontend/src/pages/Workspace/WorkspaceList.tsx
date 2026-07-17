import { useState, useMemo } from 'react';
import { X, Plus, Search, CalendarDays, ChevronDown, ChevronUp, ArrowUpDown } from 'lucide-react';
import { Workspace } from './WorkspaceTypes';

interface Props {
  workspaces: Workspace[];
  onSelect: (id: number) => void;
  onDeleteRequest: (id: number) => void;
  onCreateNew: () => void;
}

type SortKey = 'default' | 'deadline_asc' | 'deadline_desc';

const SORT_LABELS: Record<SortKey, string> = {
  default:       'Default',
  deadline_asc:  'Due: Soonest',
  deadline_desc: 'Due: Latest',
};

function formatDeadline(deadline: string | null): { label: string; overdue: boolean } | null {
  if (!deadline) return null;
  const d = new Date(deadline);
  if (isNaN(d.getTime())) return null;
  return {
    label: d.toLocaleDateString(undefined, { month: 'short', day: 'numeric', year: 'numeric' }),
    overdue: d < new Date(),
  };
}

function sortWorkspaces(list: Workspace[], key: SortKey): Workspace[] {
  if (key === 'default') return list;
  return [...list].sort((a, b) => {
    const da = a.deadline ? new Date(a.deadline).getTime() : Infinity;
    const db = b.deadline ? new Date(b.deadline).getTime() : Infinity;
    return key === 'deadline_asc' ? da - db : db - da;
  });
}

interface CardProps {
  ws: Workspace;
  faded?: boolean;
  onSelect: (id: number) => void;
  onDeleteRequest: (id: number) => void;
}

function WorkspaceCard({ ws, faded, onSelect, onDeleteRequest }: CardProps) {
  const dl = formatDeadline(ws.deadline);
  return (
    <div className={`relative group ${faded ? 'opacity-50 grayscale' : ''}`}>
      <button
        onClick={() => onSelect(ws.id)}
        className="w-full bg-white border border-gray-100 rounded-xl p-4 shadow-2xs text-left hover:border-indigo-300 hover:shadow-md transition-all cursor-pointer"
      >
        <h2 className="text-sm font-semibold text-gray-900 group-hover:text-indigo-600 transition-colors truncate">
          {ws.name}
        </h2>
        <p className="text-[11px] text-gray-400 mt-1 line-clamp-2">{ws.description}</p>
        <div className="flex flex-wrap items-center gap-2 mt-3 text-[10px] text-gray-400">
          <span>{ws.tasks.length} tasks</span>
          <span>·</span>
          <span>{ws.files.length} files</span>
          {dl && (
            <>
              <span>·</span>
              <span className={`flex items-center gap-1 ${dl.overdue && !ws.is_completed ? 'text-red-400' : ''}`}>
                <CalendarDays size={9} />
                Due {dl.label}
              </span>
            </>
          )}
          {ws.is_completed && (
            <span className="ml-auto px-1.5 py-0.5 bg-gray-100 text-gray-400 rounded text-[9px] font-medium">
              Completed
            </span>
          )}
        </div>
      </button>
      <button
        onClick={e => { e.stopPropagation(); onDeleteRequest(ws.id); }}
        className="absolute top-2 right-2 w-[22px] h-[22px] flex items-center justify-center rounded-md bg-white border border-gray-100 text-gray-300 hover:text-red-500 hover:border-red-200 hover:bg-red-50 opacity-0 group-hover:opacity-100 transition-all shadow-sm cursor-pointer z-10"
      >
        <X size={11} />
      </button>
    </div>
  );
}

export default function WorkspaceList({ workspaces, onSelect, onDeleteRequest, onCreateNew }: Props) {
  const [query,          setQuery]          = useState('');
  const [sortKey,        setSortKey]        = useState<SortKey>('default');
  const [showSortMenu,   setShowSortMenu]   = useState(false);
  const [showCompleted,  setShowCompleted]  = useState(false);

  const active    = useMemo(() => workspaces.filter(w => !w.is_completed), [workspaces]);
  const completed = useMemo(() => workspaces.filter(w =>  w.is_completed), [workspaces]);

  const filteredActive = useMemo(() => {
    const q = query.trim().toLowerCase();
    const filtered = q ? active.filter(w => w.name.toLowerCase().includes(q)) : active;
    return sortWorkspaces(filtered, sortKey);
  }, [active, query, sortKey]);

  const filteredCompleted = useMemo(() => {
    const q = query.trim().toLowerCase();
    return q ? completed.filter(w => w.name.toLowerCase().includes(q)) : completed;
  }, [completed, query]);

  return (
    <div className="flex-1 p-6 overflow-y-auto">
      {/* ── Toolbar ── */}
      <div className="flex items-center gap-2 mb-4">
        {/* Search */}
        <div className="relative flex-1">
          <Search size={12} className="absolute left-2.5 top-1/2 -translate-y-1/2 text-gray-400" />
          <input
            type="text"
            placeholder="Search workspaces…"
            value={query}
            onChange={e => setQuery(e.target.value)}
            className="w-full pl-8 pr-3 py-1.5 text-[12px] bg-white border border-gray-200 rounded-lg focus:outline-none focus:border-indigo-400 transition-colors"
          />
        </div>

        {/* Sort */}
        <div className="relative">
          <button
            onClick={() => setShowSortMenu(o => !o)}
            className="flex items-center gap-1.5 px-3 py-1.5 text-[12px] bg-white border border-gray-200 rounded-lg hover:border-gray-300 transition-colors text-gray-600"
          >
            <ArrowUpDown size={11} />
            {SORT_LABELS[sortKey]}
          </button>
          {showSortMenu && (
            <div className="absolute right-0 top-9 bg-white border border-gray-100 rounded-xl shadow-lg z-20 py-1 w-36">
              {(Object.keys(SORT_LABELS) as SortKey[]).map(k => (
                <button
                  key={k}
                  onClick={() => { setSortKey(k); setShowSortMenu(false); }}
                  className={`w-full text-left px-3 py-1.5 text-[12px] hover:bg-gray-50 transition-colors ${sortKey === k ? 'text-indigo-600 font-medium' : 'text-gray-700'}`}
                >
                  {SORT_LABELS[k]}
                </button>
              ))}
            </div>
          )}
        </div>
      </div>

      {/* ── Active workspaces ── */}
      <div className="grid grid-cols-2 gap-3 mb-4">
        {filteredActive.map(ws => (
          <WorkspaceCard
            key={ws.id}
            ws={ws}
            onSelect={onSelect}
            onDeleteRequest={onDeleteRequest}
          />
        ))}
        {filteredActive.length === 0 && (
          <p className="col-span-2 text-[12px] text-gray-400 py-4 text-center">
            {query ? 'No workspaces match your search.' : 'No active workspaces.'}
          </p>
        )}
      </div>

      {/* ── Create new ── */}
      <button
        onClick={onCreateNew}
        className="w-full h-14 bg-white border border-gray-100 hover:border-indigo-500 hover:bg-indigo-50/20 rounded-xl flex items-center justify-center gap-3 transition-all group shrink-0 shadow-2xs cursor-pointer mb-4"
      >
        <div className="w-6 h-6 rounded-md bg-indigo-50 group-hover:bg-indigo-600 flex items-center justify-center text-indigo-600 group-hover:text-white transition-colors">
          <Plus size={12} strokeWidth={3} />
        </div>
        <div className="text-left">
          <span className="block text-xs font-bold text-gray-800 group-hover:text-indigo-600 transition-colors">Create New Workspace</span>
          <span className="block text-[10px] text-gray-400">Set up a new workspace with custom tools instantly</span>
        </div>
      </button>

      {/* ── Completed section ── */}
      {(completed.length > 0 || (query && filteredCompleted.length > 0)) && (
        <div>
          <button
            onClick={() => setShowCompleted(o => !o)}
            className="flex items-center gap-2 text-[11px] text-gray-400 hover:text-gray-600 transition-colors mb-2"
          >
            {showCompleted ? <ChevronUp size={12} /> : <ChevronDown size={12} />}
            {showCompleted ? 'Hide' : 'Show'} completed ({filteredCompleted.length})
          </button>

          {showCompleted && (
            <div className="grid grid-cols-2 gap-3">
              {filteredCompleted.map(ws => (
                <WorkspaceCard
                  key={ws.id}
                  ws={ws}
                  faded
                  onSelect={onSelect}
                  onDeleteRequest={onDeleteRequest}
                />
              ))}
            </div>
          )}
        </div>
      )}
    </div>
  );
}
