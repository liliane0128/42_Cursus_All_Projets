import Logo42 from "../components/Logo42";
import { useState, useRef, useEffect, useMemo, DragEvent } from "react";
import UserMenu from "../components/UserMenu";
import {
  LayoutDashboard, FolderKanban, CheckSquare,
  MessageSquare, Zap, Clock, ChevronDown, Plus, X, Trash2,
  Search, Filter,
} from "lucide-react";
import { useNavigate } from "react-router-dom";
import { useTasks, Task, Priority, TaskStatus } from "../context/TaskContext";
import NotificationPanel from "../components/NotificationPanel";

// ── Config ────────────────────────────────────────────────────────────────────

const columns: { status: TaskStatus; label: string; accent: string; dot: string }[] = [
  { status: "todo",        label: "To Do",       accent: "text-gray-600",   dot: "bg-gray-400"   },
  { status: "in_progress", label: "In Progress", accent: "text-indigo-600", dot: "bg-indigo-500" },
  { status: "in_review",   label: "In Review",   accent: "text-amber-600",  dot: "bg-amber-400"  },
  { status: "completed",   label: "Completed",   accent: "text-gray-400",   dot: "bg-gray-300"   },
];

export const statusCfg: Record<TaskStatus, { label: string; bg: string; text: string }> = {
  todo:        { label: "To Do",       bg: "bg-gray-100",  text: "text-gray-500"   },
  in_progress: { label: "In Progress", bg: "bg-indigo-50", text: "text-indigo-600" },
  in_review:   { label: "In Review",   bg: "bg-amber-50",  text: "text-amber-600"  },
  completed:   { label: "Completed",   bg: "bg-green-50",  text: "text-green-600"  },
};

export const priorityCfg: Record<Priority, { label: string; color: string; dot: string }> = {
  high:   { label: "High", color: "text-red-500",   dot: "bg-red-500"   },
  medium: { label: "Med",  color: "text-amber-500", dot: "bg-amber-400" },
  low:    { label: "Low",  color: "text-gray-400",  dot: "bg-gray-300"  },
};

const today = new Date();
const todayStr = today.toISOString().split("T")[0];

function formatDue(date: Date): { label: string; urgent: boolean; overdue: boolean } {
  const d       = Math.ceil((date.getTime() - today.getTime()) / (1000 * 60 * 60 * 24));
  const overdue = d < 0;
  const urgent  = d >= 0 && d <= 2;
  const label   = overdue ? `${Math.abs(d)}d overdue`
    : d === 0 ? "Due today"
    : d === 1 ? "Tomorrow"
    : date.toLocaleDateString("en-US", { month: "short", day: "numeric" });
  return { label, urgent, overdue };
}

// ── Shared Nav ────────────────────────────────────────────────────────────────

function NavItem({ icon: Icon, label, active = false, to }: {
  icon: React.ElementType; label: string; active?: boolean; to?: string;
}) {
  const navigate = useNavigate();
  return (
    <button
      onClick={() => to && navigate(to)}
      className={`w-full flex items-center gap-2.5 px-3 py-2 rounded-lg text-[13px] font-medium transition-colors cursor-pointer
        ${active ? "bg-gray-100 text-gray-900" : "text-gray-400 hover:text-gray-700 hover:bg-gray-50"}`}>
      <Icon size={15} strokeWidth={active ? 2.5 : 2} />
      {label}
    </button>
  );
}

// ── Task Card ─────────────────────────────────────────────────────────────────

function TaskCard({ task, dimmed, onSelect }: { task: Task; dimmed: boolean; onSelect: () => void }) {
  const pri = priorityCfg[task.priority];
  const { label: dueLabel, urgent, overdue } = formatDue(task.dueDate);

  const handleDragStart = (e: DragEvent) => {
    e.dataTransfer.setData("text/plain", task.id.toString());
    e.dataTransfer.effectAllowed = "move";
  };

  return (
    <div
      onClick={onSelect}
      draggable
      onDragStart={handleDragStart}
      className={`rounded-xl border p-3.5 cursor-grab active:cursor-grabbing transition-all
        ${dimmed ? "bg-gray-50 border-gray-100 hover:border-gray-200" : "bg-white border-gray-100 hover:border-gray-300 hover:shadow-sm"}`}>
      <p className={`text-[13px] font-medium leading-snug mb-3 ${dimmed ? "text-gray-400" : "text-gray-800"}`}>
        {task.title}
      </p>
      <div className="flex items-center gap-1.5 mb-3">
        <span className="w-[6px] h-[6px] rounded-full flex-shrink-0" style={{ backgroundColor: task.wsColor }} />
        <span className={`text-[11px] font-medium ${dimmed ? "text-gray-400" : "text-gray-500"}`}>{task.project}</span>
      </div>
      <div className="flex items-center justify-between">
        <span className={`text-[9px] font-medium flex items-center gap-1 ${pri.color} ${dimmed ? "opacity-50" : ""}`}>
          <span className={`w-[5px] h-[5px] rounded-full ${pri.dot}`} />{pri.label}
        </span>
        <span className={`text-[11px] flex items-center gap-1 font-medium
          ${dimmed ? "text-gray-300" : overdue ? "text-red-400" : urgent ? "text-amber-500" : "text-gray-400"}`}>
          <Clock size={10} />{dueLabel}
        </span>
      </div>
    </div>
  );
}

// ── Side Panel ────────────────────────────────────────────────────────────────

function SidePanel({ task, onClose, workspaceDeadline }: { task: Task; onClose: () => void; workspaceDeadline?: string | null }) {
  const { updateTask, deleteTask } = useTasks();
  const [title,    setTitle]    = useState(task.title);
  const [priority, setPriority] = useState<Priority>(task.priority);
  const [dueDate,  setDueDate]  = useState(task.dueDate.toISOString().split("T")[0]);

  useEffect(() => {
    setTitle(task.title);
    setPriority(task.priority);
    setDueDate(task.dueDate.toISOString().split("T")[0]);
  }, [task.id]);

  function save() {
    updateTask(task.id, { title, priority, dueDate: new Date(dueDate) });
  }

  return (
    <div className="w-72 flex-shrink-0 bg-white border-l border-gray-100 flex flex-col h-full">
      <div className="flex items-center justify-between px-5 py-4 border-b border-gray-100">
        <p className="text-[13px] font-semibold">Task Details</p>
        <button onClick={onClose} className="text-gray-400 hover:text-gray-900 transition-colors">
          <X size={15} />
        </button>
      </div>
      <div className="flex-1 overflow-y-auto px-5 py-4 space-y-5">
        <div>
          <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Title</label>
          <textarea
            value={title}
            onChange={e => setTitle(e.target.value)}
            onBlur={save}
            rows={2}
            className="w-full text-[13px] text-gray-800 border border-gray-100 rounded-lg px-3 py-2 resize-none focus:outline-none focus:border-gray-300 bg-gray-50"
          />
        </div>
        <div>
          <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Project</label>
          <div className="flex items-center gap-2 px-3 py-2 bg-gray-50 rounded-lg border border-gray-100">
            <span className="w-2 h-2 rounded-full" style={{ backgroundColor: task.wsColor }} />
            <span className="text-[13px] text-gray-600">{task.project}</span>
          </div>
        </div>
        <div>
          <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Status</label>
          <div className="grid grid-cols-2 gap-1.5">
            {(Object.keys(statusCfg) as TaskStatus[]).map(s => (
              <button
                key={s}
                onClick={() => updateTask(task.id, { status: s })}
                className={`text-[11px] px-2 py-1.5 rounded-lg font-medium border transition-colors
                  ${task.status === s
                    ? `${statusCfg[s].bg} ${statusCfg[s].text} border-transparent`
                    : "bg-white text-gray-400 border-gray-100 hover:border-gray-300"}`}>
                {statusCfg[s].label}
              </button>
            ))}
          </div>
        </div>
        <div>
          <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Priority</label>
          <div className="flex gap-1.5">
            {(Object.keys(priorityCfg) as Priority[]).map(p => (
              <button
                key={p}
                onClick={() => { setPriority(p); updateTask(task.id, { priority: p }); }}
                className={`flex-1 text-[11px] px-2 py-1.5 rounded-lg font-medium border transition-colors flex items-center justify-center gap-1
                  ${priority === p ? "bg-gray-900 text-white border-transparent" : "bg-white text-gray-400 border-gray-100 hover:border-gray-300"}`}>
                {priorityCfg[p].label}
              </button>
            ))}
          </div>
        </div>
        <div>
          <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Due Date</label>
          <input
            type="date"
            value={dueDate}
            min={todayStr}
            max={workspaceDeadline ?? undefined}
            onChange={e => { setDueDate(e.target.value); updateTask(task.id, { dueDate: new Date(e.target.value) }); }}
            className="w-full text-[13px] text-gray-700 border border-gray-100 rounded-lg px-3 py-2 bg-gray-50 focus:outline-none focus:border-gray-300"
          />
        </div>
      </div>
      <div className="px-5 py-4 border-t border-gray-100">
        <button
          onClick={() => { deleteTask(task.id); onClose(); }}
          className="w-full flex items-center justify-center gap-2 py-2 rounded-lg border border-red-100 text-red-400 text-[13px] font-medium hover:bg-red-50 hover:border-red-200 transition-colors">
          <Trash2 size={13} /> Delete Task
        </button>
      </div>
    </div>
  );
}

// ── Add Task Modal ────────────────────────────────────────────────────────────

function AddTaskModal({ onClose }: { onClose: () => void }) {
  const { addTask, workspaces, tasks } = useTasks();
  const [title,       setTitle]       = useState("");
  const [workspaceId, setWorkspaceId] = useState<number>(workspaces[0]?.id ?? 0);
  const [priority,    setPriority]    = useState<Priority>("medium");
  const [status,      setStatus]      = useState<TaskStatus>("todo");
  const [dueDate,     setDueDate]     = useState("");
  const [error,       setError]       = useState("");

  const selectedWs = workspaces.find(w => w.id === workspaceId);
  const maxDate = selectedWs?.deadline ?? undefined;

  async function handleSubmit() {
    if (!title.trim() || !dueDate || !workspaceId) return;

    const isDuplicate = tasks.some(
      t => t.workspace_id === workspaceId &&
           t.title.trim().toLowerCase() === title.trim().toLowerCase()
    );
    if (isDuplicate) {
      setError("A task with this name already exists in this workspace.");
      return;
    }

    setError("");
    await addTask({ title: title.trim(), workspace_id: workspaceId, status, priority, due_date: dueDate });
    onClose();
  }

  return (
    <div className="fixed inset-0 bg-black/20 z-50 flex items-center justify-center" onClick={onClose}>
      <div className="bg-white rounded-2xl shadow-xl w-[420px] p-6" onClick={e => e.stopPropagation()}>
        <div className="flex items-center justify-between mb-5">
          <p className="text-[15px] font-semibold">New Task</p>
          <button onClick={onClose} className="text-gray-400 hover:text-gray-900"><X size={15} /></button>
        </div>
        <div className="space-y-4">
          <div>
            <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Title</label>
            <input
              autoFocus
              value={title}
              onChange={e => { setTitle(e.target.value); setError(""); }}
              placeholder="Task title..."
              className={`w-full text-[13px] border rounded-lg px-3 py-2 focus:outline-none transition-colors
                ${error ? "border-red-300 focus:border-red-400" : "border-gray-200 focus:border-gray-400"}`}
            />
            {error && <p className="text-[11px] text-red-400 mt-1">{error}</p>}
          </div>
          <div>
            <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Workspace</label>
            <select
              value={workspaceId}
              onChange={e => { setWorkspaceId(Number(e.target.value)); setError(""); }}
              className="w-full text-[13px] border border-gray-200 rounded-lg px-3 py-2 focus:outline-none focus:border-gray-400 bg-white">
              {workspaces.map(w => <option key={w.id} value={w.id}>{w.name}</option>)}
            </select>
          </div>
          <div>
            <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Starting Status</label>
            <div className="grid grid-cols-2 gap-1.5">
              {(Object.keys(statusCfg) as TaskStatus[]).map(s => (
                <button key={s} onClick={() => setStatus(s)}
                  className={`text-[11px] px-2 py-1.5 rounded-lg font-medium border transition-colors
                    ${status === s ? `${statusCfg[s].bg} ${statusCfg[s].text} border-transparent` : "bg-white text-gray-400 border-gray-100 hover:border-gray-300"}`}>
                  {statusCfg[s].label}
                </button>
              ))}
            </div>
          </div>
          <div>
            <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Priority</label>
            <div className="flex gap-1.5">
              {(Object.keys(priorityCfg) as Priority[]).map(p => (
                <button key={p} onClick={() => setPriority(p)}
                  className={`flex-1 text-[11px] py-1.5 rounded-lg font-medium border transition-colors
                    ${priority === p ? "bg-gray-900 text-white border-transparent" : "bg-white text-gray-400 border-gray-100 hover:border-gray-300"}`}>
                  {priorityCfg[p].label}
                </button>
              ))}
            </div>
          </div>
          <div>
            <label className="text-[11px] text-gray-400 font-medium block mb-1.5">Due Date</label>
            <input
              type="date"
              value={dueDate}
              onChange={e => setDueDate(e.target.value)}
              min={todayStr}
              max={maxDate}
              className="w-full text-[13px] border border-gray-200 rounded-lg px-3 py-2 focus:outline-none focus:border-gray-400"
            />
          </div>
        </div>
        <div className="flex gap-2 mt-6">
          <button onClick={onClose} className="flex-1 py-2 rounded-lg border border-gray-200 text-[13px] text-gray-500 hover:bg-gray-50 transition-colors">Cancel</button>
          <button
            onClick={handleSubmit}
            disabled={!title.trim() || !dueDate}
            className="flex-1 py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium hover:bg-gray-700 transition-colors disabled:opacity-30 disabled:cursor-not-allowed">
            Add Task
          </button>
        </div>
      </div>
    </div>
  );
}

// ── Column ────────────────────────────────────────────────────────────────────

function Column({ status, label, accent, dot, tasks, onSelect }: typeof columns[number] & { tasks: Task[]; onSelect: (t: Task) => void }) {
  const { updateTask } = useTasks();
  const [isOver, setIsOver] = useState(false);
  const dimmed   = status === "completed";
  const colTasks = tasks.filter(t => t.status === status).sort((a, b) => a.dueDate.getTime() - b.dueDate.getTime());

  const handleDragOver = (e: DragEvent) => {
    e.preventDefault();
    if (e.dataTransfer.types.includes("text/plain")) setIsOver(true);
  };

  const handleDragLeave = () => setIsOver(false);

  const handleDrop = (e: DragEvent) => {
    e.preventDefault();
    setIsOver(false);
    const idStr = e.dataTransfer.getData("text/plain");
    if (idStr) updateTask(parseInt(idStr, 10), { status });
  };

  return (
    <div
      onDragOver={handleDragOver}
      onDragLeave={handleDragLeave}
      onDrop={handleDrop}
      className={`flex flex-col min-h-0 flex-shrink-0 w-64 rounded-xl transition-colors p-1
        ${isOver ? "bg-gray-100/80 border border-dashed border-gray-300" : "border border-transparent"}`}>
      <div className="flex items-center gap-2 mb-3 px-0.5 flex-shrink-0">
        <span className={`w-2 h-2 rounded-full ${dot}`} />
        <span className={`text-[13px] font-semibold ${accent}`}>{label}</span>
        <span className="ml-auto text-[11px] text-gray-400 font-medium bg-gray-100 px-1.5 py-0.5 rounded-full">{colTasks.length}</span>
      </div>
      <div
        className={`flex-1 overflow-y-auto space-y-2 pr-1 min-h-0 ${dimmed ? "opacity-70" : ""}`}
        style={{ scrollbarWidth: "thin", scrollbarColor: "#e5e7eb transparent" }}>
        {colTasks.length === 0 ? (
          <div className="flex items-center justify-center h-20 rounded-xl border border-dashed border-gray-100">
            <span className="text-[12px] text-gray-300">No tasks</span>
          </div>
        ) : (
          colTasks.map(task => <TaskCard key={task.id} task={task} dimmed={dimmed} onSelect={() => onSelect(task)} />)
        )}
      </div>
    </div>
  );
}

// ── Main ──────────────────────────────────────────────────────────────────────

export default function Tasks() {
  const navigate = useNavigate();
  const [selectedTask,      setSelectedTask]      = useState<Task | null>(null);
  const [showModal,         setShowModal]          = useState(false);
  const [filterWorkspaceId, setFilterWorkspaceId]  = useState<number | null>(null);
  const [showFilter,        setShowFilter]         = useState(false);
  const [searchQuery,       setSearchQuery]        = useState("");
  const [selectedPri,       setSelectedPri]        = useState<Priority | "all">("all");

  const filterRef = useRef<HTMLDivElement>(null);
  const { tasks, workspaces } = useTasks();

  const liveTask = selectedTask ? tasks.find(t => t.id === selectedTask.id) ?? null : null;
  const liveTaskDeadline = liveTask ? workspaces.find(w => w.id === liveTask.workspace_id)?.deadline ?? null : null;

  const filteredTasks = useMemo(() => {
    return tasks.filter(t =>
      (filterWorkspaceId === null || t.workspace_id === filterWorkspaceId) &&
      t.title.toLowerCase().includes(searchQuery.toLowerCase()) &&
      (selectedPri === "all" || t.priority === selectedPri)
    );
  }, [tasks, filterWorkspaceId, searchQuery, selectedPri]);

  useEffect(() => {
    function handler(e: MouseEvent) {
      if (filterRef.current && !filterRef.current.contains(e.target as Node)) setShowFilter(false);
    }
    document.addEventListener("mousedown", handler);
    return () => document.removeEventListener("mousedown", handler);
  }, []);

  return (
    <div className="flex h-screen bg-[#f5f5f3] font-sans text-gray-900 overflow-hidden">

      {/* Sidebar */}
      <aside className="w-52 flex-shrink-0 flex flex-col bg-white border-r border-gray-100">
        <div className="h-[52px] px-4 flex items-center gap-2.5 border-b border-gray-100 flex-shrink-0">
          <div className="w-[26px] h-[26px] rounded-[6px] bg-gray-900 flex items-center justify-center flex-shrink-0">
            <Logo42 size={12} />
          </div>
          <span className="text-[13px] font-semibold tracking-tight">Collab42</span>
        </div>
        <nav className="flex-1 px-2 py-3 space-y-0.5">
          <NavItem icon={LayoutDashboard} label="Dashboard" to="/dashboard" />
          <NavItem icon={FolderKanban}    label="Workspace" to="/workspace"  />
          <NavItem icon={CheckSquare}     label="Tasks"     to="/tasks"      active />
          <NavItem icon={MessageSquare}   label="Chats"     to="/chats"      />
        </nav>
      </aside>

      {/* Main */}
      <div className="flex-1 flex flex-col min-w-0">
        <header className="h-[52px] bg-white border-b border-gray-100 flex items-center justify-between px-6 flex-shrink-0">
          <div>
            <p className="text-[14px] font-semibold">Tasks</p>
            <p className="text-[11px] text-gray-400 mt-0.5">All workspaces</p>
          </div>
          <div className="flex items-center gap-3">
            <button onClick={() => setShowModal(true)}
              className="flex items-center gap-1.5 text-[12px] font-medium bg-gray-900 text-white px-3 py-1.5 rounded-lg hover:bg-gray-700 transition-colors">
              <Plus size={13} /> Add Task
            </button>

            {/* Workspace filter dropdown */}
            <div ref={filterRef} className="relative">
              <button
                onClick={() => setShowFilter(o => !o)}
                className="flex items-center gap-1.5 text-[12px] text-gray-500 bg-gray-100 hover:bg-gray-200 px-3 py-1.5 rounded-lg font-medium transition-colors">
                {filterWorkspaceId ? workspaces.find(w => w.id === filterWorkspaceId)?.name : "All projects"}
                <ChevronDown size={12} />
              </button>
              {showFilter && (
                <div className="absolute top-full mt-1 right-0 bg-white border border-gray-100 rounded-lg shadow-lg z-50 py-1 min-w-[150px]">
                  <button
                    onClick={() => { setFilterWorkspaceId(null); setShowFilter(false); }}
                    className={`w-full text-left px-3 py-1.5 text-[12px] hover:bg-gray-50 transition-colors
                      ${filterWorkspaceId === null ? "font-semibold text-gray-900" : "text-gray-500"}`}>
                    All projects
                  </button>
                  {workspaces.map(w => (
                    <button
                      key={w.id}
                      onClick={() => { setFilterWorkspaceId(w.id); setShowFilter(false); }}
                      className={`w-full text-left px-3 py-1.5 text-[12px] hover:bg-gray-50 transition-colors flex items-center gap-2
                        ${filterWorkspaceId === w.id ? "font-semibold text-gray-900" : "text-gray-500"}`}>
                      <span className="w-2 h-2 rounded-full flex-shrink-0" style={{ backgroundColor: w.color }} />
                      {w.name}
                    </button>
                  ))}
                </div>
              )}
            </div>

            <div className="flex items-center gap-1">
              <NotificationPanel />
              <UserMenu />
            </div>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
          <main className="flex-1 overflow-hidden px-6 py-5 flex flex-col">

            {/* Search + priority filter bar */}
            <div className="flex flex-col gap-1.5 mb-3 sm:flex-row sm:items-center sm:justify-between flex-shrink-0 border-b border-gray-100 pb-3">
              <div className="relative w-full sm:w-56">
                <Search size={11} className="absolute left-2 top-1/2 -translate-y-1/2 text-gray-400" />
                <input
                  type="text"
                  placeholder="Search tasks..."
                  value={searchQuery}
                  onChange={e => setSearchQuery(e.target.value)}
                  className="w-full pl-7 pr-2.5 py-1 bg-gray-50 border border-gray-100 rounded-md text-[11px] focus:outline-none focus:border-gray-300 focus:bg-white transition-all"
                />
              </div>
              <div className="flex items-center gap-1 overflow-x-auto py-0.5">
                <Filter size={10} className="text-gray-400 mr-0.5 shrink-0" />
                <button
                  onClick={() => setSelectedPri("all")}
                  className={`px-2 py-0.5 rounded text-[10px] font-medium border shrink-0
                    ${selectedPri === "all" ? "bg-gray-900 border-gray-900 text-white" : "bg-white border-gray-100 text-gray-500"}`}>
                  All
                </button>
                {(Object.keys(priorityCfg) as Priority[]).map(p => (
                  <button key={p} onClick={() => setSelectedPri(p)}
                    className={`px-2 py-0.5 rounded text-[10px] font-medium border shrink-0
                      ${selectedPri === p ? "bg-gray-900 border-gray-900 text-white" : "bg-white border-gray-100 text-gray-500"}`}>
                    {priorityCfg[p].label}
                  </button>
                ))}
              </div>
            </div>

            {/* Kanban board — horizontal scroll */}
            <div className="flex gap-4 flex-1 min-h-0 overflow-x-auto pb-1"
              style={{ scrollbarWidth: "thin", scrollbarColor: "#e5e7eb transparent" }}>
              {columns.map(col => <Column key={col.status} {...col} tasks={filteredTasks} onSelect={setSelectedTask} />)}
            </div>
          </main>

          {liveTask && <SidePanel task={liveTask} onClose={() => setSelectedTask(null)} workspaceDeadline={liveTaskDeadline} />}
        </div>
      </div>

      {showModal && <AddTaskModal onClose={() => setShowModal(false)} />}
    </div>
  );
}