import { useState, useEffect, useMemo, DragEvent } from "react";
import {
  Clock, Plus, X, Trash2, Search, Filter, CheckSquare
} from "lucide-react";

export type TaskStatus = 'todo' | 'in_progress' | 'in_review' | 'completed';
export type Priority = 'low' | 'medium' | 'high';

export interface Task {
  id: number;
  title: string;
  status: TaskStatus;
  priority: Priority;
  dueDate: Date;
}

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
const todayStr = new Date().toISOString().split("T")[0];

function formatDue(dateInput: Date | string): { label: string; urgent: boolean; overdue: boolean } {
  const date = new Date(dateInput);
  
  if (isNaN(date.getTime())) {
    return { label: "Invalid Date", urgent: false, overdue: false };
  }

  const d       = Math.ceil((date.getTime() - today.getTime()) / (1000 * 60 * 60 * 24));
  const overdue = d < 0;
  const urgent  = d >= 0 && d <= 2;
  const label   = overdue ? `${Math.abs(d)}d overdue`
    : d === 0 ? "Due today"
    : d === 1 ? "Tomorrow"
    : date.toLocaleDateString("en-US", { month: "short", day: "numeric" });
  return { label, urgent, overdue };
}

function TaskCard({ task, dimmed, onSelect, onUpdateStatus }: { task: Task; dimmed: boolean; onSelect: () => void; onUpdateStatus: (id: number, s: TaskStatus) => void }) {
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
      className={`rounded-xl border p-2.5 cursor-grab active:cursor-grabbing transition-all flex-shrink-0 w-full hover:shadow-sm
        ${dimmed ? "bg-gray-50 border-gray-100 opacity-75 hover:border-gray-200" : "bg-white border-gray-100 hover:border-gray-300"}`}>
      <p className={`text-[12px] font-medium leading-normal mb-2 break-words ${dimmed ? "text-gray-400 line-through" : "text-gray-800"}`}>
        {task.title}
      </p>
      <div className="flex items-center justify-between gap-1">
        <div className="flex items-center gap-1.5">
          <span className={`text-[9px] font-medium flex items-center gap-1 ${pri.color}`}>
            <span className={`w-1 h-1 rounded-full ${pri.dot}`} />{pri.label}
          </span>
        </div>
        <span className={`text-[10px] flex items-center gap-0.5 font-medium shrink-0
          ${dimmed ? "text-gray-300" : overdue ? "text-red-400" : urgent ? "text-amber-500" : "text-gray-400"}`}>
          <Clock size={9} />{dueLabel}
        </span>
      </div>
    </div>
  );
}

function SidePanel({ task, onClose, onUpdateTask, onDeleteTask, workspaceDeadline, existingTitles }: { task: Task; onClose: () => void; onUpdateTask: (id: number, data: Partial<Task>) => void; onDeleteTask: (id: number) => void; workspaceDeadline?: string | null; existingTitles: string[] }) {
  const [title,      setTitle]      = useState(task.title);
  const [priority,   setPriority]   = useState<Priority>(task.priority);
  const [dueDate,    setDueDate]    = useState(task.dueDate.toISOString().split("T")[0]);
  const [titleError, setTitleError] = useState<string | null>(null);

  useEffect(() => {
    setTitle(task.title);
    setPriority(task.priority);
    setDueDate(task.dueDate.toISOString().split("T")[0]);
    setTitleError(null);
  }, [task]);

  function save() {
    const trimmed = title.trim();
    if (!trimmed) { setTitle(task.title); return; }
    if (trimmed.toLowerCase() !== task.title.toLowerCase() && existingTitles.includes(trimmed.toLowerCase())) {
      setTitleError("A task with this name already exists");
      setTitle(task.title);
      return;
    }
    setTitleError(null);
    onUpdateTask(task.id, { title: trimmed, priority, dueDate: new Date(dueDate) });
  }

  return (
    <div className="w-64 flex-shrink-0 bg-white border-l border-gray-100 flex flex-col h-full ml-2">
      <div className="flex items-center justify-between px-4 py-3 border-b border-gray-100">
        <p className="text-[12px] font-semibold">Details</p>
        <button onClick={onClose} className="text-gray-400 hover:text-gray-900"><X size={14} /></button>
      </div>
      <div className="flex-1 overflow-y-auto px-4 py-3 space-y-4">
        <div>
          <label className="text-[10px] text-gray-400 font-medium block mb-1">Title</label>
          <textarea
            value={title}
            onChange={e => { setTitle(e.target.value); setTitleError(null); }}
            onBlur={save}
            rows={2}
            className={`w-full text-[12px] text-gray-800 border rounded-lg px-2.5 py-1.5 resize-none focus:outline-none bg-gray-50 ${titleError ? 'border-rose-400' : 'border-gray-100 focus:border-gray-300'}`}
          />
          {titleError && <p className="text-[10px] text-rose-500 mt-1">{titleError}</p>}
        </div>
        <div>
          <label className="text-[10px] text-gray-400 font-medium block mb-1">Status</label>
          <div className="grid grid-cols-2 gap-1">
            {(Object.keys(statusCfg) as TaskStatus[]).map(s => (
              <button
                key={s}
                onClick={() => onUpdateTask(task.id, { status: s })}
                className={`text-[10px] py-1 rounded-md font-medium border
                  ${task.status === s ? `${statusCfg[s].bg} ${statusCfg[s].text} border-transparent` : "bg-white text-gray-400 border-gray-100"}`}>
                {statusCfg[s].label}
              </button>
            ))}
          </div>
        </div>
        <div>
          <label className="text-[10px] text-gray-400 font-medium block mb-1">Priority</label>
          <div className="flex gap-1">
            {(Object.keys(priorityCfg) as Priority[]).map(p => (
              <button
                key={p}
                onClick={() => { setPriority(p); onUpdateTask(task.id, { priority: p }); }}
                className={`flex-1 text-[10px] py-1 rounded-md font-medium border flex items-center justify-center gap-0.5
                  ${priority === p ? "bg-gray-900 text-white border-transparent" : "bg-white text-gray-400 border-gray-100"}`}>
                {priorityCfg[p].label}
              </button>
            ))}
          </div>
        </div>
        <div>
          <label className="text-[10px] text-gray-400 font-medium block mb-1">Due Date</label>
          <input
            type="date"
            value={dueDate}
            min={todayStr}
            max={workspaceDeadline || undefined}
            onChange={e => { setDueDate(e.target.value); onUpdateTask(task.id, { dueDate: new Date(e.target.value) }); }}
            className="w-full text-[12px] text-gray-700 border border-gray-100 rounded-lg px-2 py-1.5 bg-gray-50 focus:outline-none focus:border-gray-300"
          />
        </div>
      </div>
      <div className="px-4 py-3 border-t border-gray-100">
        <button
          onClick={() => { onDeleteTask(task.id); onClose(); }}
          className="w-full flex items-center justify-center gap-1.5 py-1.5 rounded-lg border border-red-100 text-red-400 text-[12px] font-medium hover:bg-red-50">
          <Trash2 size={12} /> Delete Task
        </button>
      </div>
    </div>
  );
}

function AddTaskModal({ onClose, onAddTask, workspaceDeadline, existingTitles }: { onClose: () => void; onAddTask: (task: Omit<Task, 'id'>) => void; workspaceDeadline?: string | null; existingTitles: string[] }) {
  const [title,    setTitle]    = useState("");
  const [priority, setPriority] = useState<Priority>("medium");
  const [status,   setStatus]   = useState<TaskStatus>("todo");
  const [dueDate,  setDueDate]  = useState("");
  const [error,    setError]    = useState<string | null>(null);

  function handleSubmit() {
    if (!title.trim() || !dueDate) return;
    if (existingTitles.includes(title.trim().toLowerCase())) {
      setError("A task with this name already exists");
      return;
    }
    onAddTask({ title: title.trim(), priority, status, dueDate: new Date(dueDate) });
    onClose();
  }

  return (
    <div className="fixed inset-0 bg-black/20 z-50 flex items-center justify-center" onClick={onClose}>
      <div className="bg-white rounded-xl shadow-xl w-[360px] p-5" onClick={e => e.stopPropagation()}>
        <div className="flex items-center justify-between mb-4">
          <p className="text-[14px] font-semibold">New Task</p>
          <button onClick={onClose} className="text-gray-400 hover:text-gray-900"><X size={14} /></button>
        </div>
        <div className="space-y-3.5">
          <div>
            <label className="text-[10px] text-gray-400 font-medium block mb-1">Title</label>
            <input
              autoFocus value={title} onChange={e => { setTitle(e.target.value); setError(null); }} placeholder="Task title..."
              className={`w-full text-[12px] border rounded-lg px-2.5 py-1.5 focus:outline-none ${error ? 'border-rose-400 focus:border-rose-400' : 'border-gray-200 focus:border-gray-400'}`}
            />
            {error && <p className="text-[10px] text-rose-500 mt-1">{error}</p>}
          </div>
          <div>
            <label className="text-[10px] text-gray-400 font-medium block mb-1">Starting Status</label>
            <div className="grid grid-cols-2 gap-1">
              {(Object.keys(statusCfg) as TaskStatus[]).map(s => (
                <button key={s} onClick={() => setStatus(s)}
                  className={`text-[10px] py-1 rounded-md font-medium border
                    ${status === s ? `${statusCfg[s].bg} ${statusCfg[s].text} border-transparent` : "bg-white text-gray-400 border-gray-100"}`}>
                  {statusCfg[s].label}
                </button>
              ))}
            </div>
          </div>
          <div>
            <label className="text-[10px] text-gray-400 font-medium block mb-1">Priority</label>
            <div className="flex gap-1">
              {(Object.keys(priorityCfg) as Priority[]).map(p => (
                <button key={p} onClick={() => setPriority(p)}
                  className={`flex-1 text-[10px] py-1 rounded-md font-medium border
                    ${priority === p ? "bg-gray-900 text-white border-transparent" : "bg-white text-gray-400 border-gray-100"}`}>
                  {priorityCfg[p].label}
                </button>
              ))}
            </div>
          </div>
          <div>
            <label className="text-[10px] text-gray-400 font-medium block mb-1">Due Date</label>
            <input type="date" value={dueDate} onChange={e => setDueDate(e.target.value)}
              min={todayStr}
              max={workspaceDeadline || undefined}
              className="w-full text-[12px] border border-gray-200 rounded-lg px-2.5 py-1.5 focus:outline-none focus:border-gray-400"
            />
          </div>
        </div>
        <div className="flex gap-2 mt-5">
          <button onClick={onClose} className="flex-1 py-1.5 rounded-lg border border-gray-200 text-[12px] text-gray-500">Cancel</button>
          <button onClick={handleSubmit} disabled={!title.trim() || !dueDate}
            className="flex-1 py-1.5 rounded-lg bg-gray-900 text-white text-[12px] font-medium disabled:opacity-30">
            Add Task
          </button>
        </div>
      </div>
    </div>
  );
}

function Column({ status, label, accent, dot, tasks, onSelect, onUpdateStatus }: typeof columns[number] & { tasks: Task[]; onSelect: (t: Task) => void; onUpdateStatus: (id: number, s: TaskStatus) => void }) {
  const [isOver, setIsOver] = useState(false);
  const dimmed = status === "completed";

  const handleDragOver = (e: DragEvent) => {
    e.preventDefault();
    if (e.dataTransfer.types.includes("text/plain")) setIsOver(true);
  };

  const handleDragLeave = () => setIsOver(false);

  const handleDrop = (e: DragEvent) => {
    e.preventDefault();
    setIsOver(false);
    const idStr = e.dataTransfer.getData("text/plain");
    if (idStr) onUpdateStatus(parseInt(idStr, 10), status);
  };

  return (
    <div
      onDragOver={handleDragOver}
      onDragLeave={handleDragLeave}
      onDrop={handleDrop}
      className={`flex flex-col min-h-0 flex-1 min-w-0 rounded-xl transition-colors p-1.5
        ${isOver ? "bg-gray-50/80 border border-dashed border-gray-300" : "border border-transparent"}`}>
      <div className="flex items-center gap-1.5 mb-2 px-0.5 flex-shrink-0">
        <span className={`w-1.5 h-1.5 rounded-full ${dot}`} />
        <span className={`text-[12px] font-semibold ${accent}`}>{label}</span>
        <span className="ml-auto text-[10px] text-gray-400 font-medium bg-gray-100 px-1.5 py-0.5 rounded-full">{tasks.length}</span>
      </div>
      <div className="flex-1 overflow-y-auto space-y-1.5 pr-0.5 min-h-0">
        {tasks.length === 0 ? (
          <div className="flex items-center justify-center h-16 rounded-lg border border-dashed border-gray-100 bg-gray-50/20">
            <span className="text-[11px] text-gray-300">No tasks</span>
          </div>
        ) : (
          tasks.map(task => <TaskCard key={task.id} task={task} dimmed={dimmed} onSelect={() => onSelect(task)} onUpdateStatus={onUpdateStatus} />)
        )}
      </div>
    </div>
  );
}

interface WorkspaceTasksProps {
  tasks: Task[];
  workspaceId: number;
  workspaceDeadline?: string | null;
  onAddTask: (data: { title: string; workspace_id: number; status: TaskStatus; priority: Priority; due_date?: string }) => void;
  onUpdateTask: (id: number, data: Partial<Task>) => void;
  onDeleteTask: (id: number) => void;
}

export default function WorkspaceTasks({ tasks, workspaceId, workspaceDeadline, onAddTask, onUpdateTask, onDeleteTask }: WorkspaceTasksProps) {
  const [selectedTask, setSelectedTask] = useState<Task | null>(null);
  const [showModal,    setShowModal]    = useState(false);
  const [searchQuery,  setSearchQuery]  = useState("");
  const [selectedPri,  setSelectedPri]  = useState<Priority | "all">("all");

  const liveTask = selectedTask ? tasks.find(t => t.id === selectedTask.id) ?? null : null;

  const handleAddTask = (newTask: Omit<Task, 'id'>) => {
    onAddTask({
      title: newTask.title,
      workspace_id: workspaceId,
      status: newTask.status,
      priority: newTask.priority,
      due_date: newTask.dueDate.toISOString().split("T")[0],
    });
  };

  const handleUpdateTask = (id: number, updatedFields: Partial<Task>) => {
    onUpdateTask(id, updatedFields);
  };

  const handleUpdateStatus = (id: number, newStatus: TaskStatus) => {
    onUpdateTask(id, { status: newStatus });
  };

  const handleDeleteTask = (id: number) => {
    onDeleteTask(id);
    setSelectedTask(null);
  };

  const filteredTasks = useMemo(() => {
    return tasks.filter(task =>
      task.title.toLowerCase().includes(searchQuery.toLowerCase()) &&
      (selectedPri === "all" || task.priority === selectedPri)
    );
  }, [tasks, searchQuery, selectedPri]);

  return (
    <div className="flex-1 flex flex-col min-h-0 overflow-hidden">
      <div className="flex items-center justify-between mb-3 flex-shrink-0">
        <h2 className="text-[14px] font-bold text-gray-800 flex items-center gap-2">
          <CheckSquare size={10} className="text-gray-600" /> Tasks
        </h2>
        <div className="flex items-center gap-1.5 pr-[38px]">
          <button
            onClick={() => setShowModal(true)}
            className="bg-gray-900 hover:bg-gray-800 text-white text-[11px] font-bold px-3 py-1.5 rounded-lg transition-colors flex items-center gap-1 h-[32px]">
            <Plus size={10} /> Add Task
          </button>
        </div>
      </div>

      <div className="flex flex-col gap-1.5 mb-3 sm:flex-row sm:items-center sm:justify-between flex-shrink-0 border-b border-gray-50 pb-2">
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
            className={`px-2 py-0.5 rounded text-[10px] font-medium border shrink-0 ${
              selectedPri === "all" ? "bg-gray-900 border-gray-900 text-white" : "bg-white border-gray-100 text-gray-500"}`}>
            All
          </button>
          {(Object.keys(priorityCfg) as Priority[]).map(p => (
            <button
              key={p}
              onClick={() => setSelectedPri(p)}
              className={`px-2 py-0.5 rounded text-[10px] font-medium border shrink-0 ${
                selectedPri === p ? "bg-gray-900 border-gray-900 text-white" : "bg-white border-gray-100 text-gray-500"}`}>
              {priorityCfg[p].label}
            </button>
          ))}
        </div>
      </div>

      <div className="flex-1 flex overflow-hidden min-h-0">
        <div className="flex-1 overflow-hidden pb-1.5">
          <div className="flex gap-2 h-full">
            {columns.map(col => {
              const colTasks = [...filteredTasks]
                .filter(t => t.status === col.status)
                .sort((a, b) => {
                  const dateA = a.dueDate ? new Date(a.dueDate).getTime() : 0;
                  const dateB = b.dueDate ? new Date(b.dueDate).getTime() : 0;
                  return dateA - dateB;
                });
              return (
                <Column
                  key={col.status}
                  {...col}
                  tasks={colTasks}
                  onSelect={setSelectedTask}
                  onUpdateStatus={handleUpdateStatus}
                />
              );
            })}
          </div>
        </div>
        {liveTask && (
          <SidePanel
            task={liveTask}
            onClose={() => setSelectedTask(null)}
            onUpdateTask={handleUpdateTask}
            onDeleteTask={handleDeleteTask}
            workspaceDeadline={workspaceDeadline}
            existingTitles={tasks.filter(t => t.id !== liveTask.id).map(t => t.title.toLowerCase())}
          />
        )}
      </div>

      {showModal && <AddTaskModal onClose={() => setShowModal(false)} onAddTask={handleAddTask} workspaceDeadline={workspaceDeadline} existingTitles={tasks.map(t => t.title.toLowerCase())} />}
    </div>
  );
}