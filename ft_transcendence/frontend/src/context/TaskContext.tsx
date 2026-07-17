import { createContext, useContext, useState, useEffect, ReactNode } from "react";
import { getTasks, createTask, updateTask as apiUpdateTask, deleteTask as apiDeleteTask, getWorkspaces, ApiTask, Workspace } from "../api";
import { useAuth } from "./AuthContext";

// ── Types ─────────────────────────────────────────────────────────────────────

export type Priority = "high" | "medium" | "low";
export type TaskStatus = "todo" | "in_progress" | "in_review" | "completed";

export interface Task {
  id: number;
  title: string;
  project: string;
  wsColor: string;
  priority: Priority;
  status: TaskStatus;
  dueDate: Date;
  workspace_id: number;
}

interface TaskContextType {
  tasks: Task[];
  workspaces: Workspace[];
  loading: boolean;
  addTask: (data: { title: string; workspace_id: number; status?: TaskStatus; priority?: Priority; due_date?: string }) => Promise<void>;
  updateTask: (id: number, updates: Partial<Task>) => Promise<void>;
  deleteTask: (id: number) => Promise<void>;
  refreshWorkspaces: () => Promise<void>;
}

// ── Helpers ───────────────────────────────────────────────────────────────────

function apiTaskToTask(t: ApiTask, workspaces: Workspace[]): Task {
  const ws = workspaces.find(w => w.id === t.workspace_id);
  return {
    id: t.id,
    title: t.title,
    status: t.status as TaskStatus,
    priority: t.priority as Priority,
    project: ws?.name ?? "Unknown",
    wsColor: ws?.color ?? "#6366f1",
    dueDate: t.due_date ? new Date(t.due_date) : new Date(),
    workspace_id: t.workspace_id,
  };
}

// ── Context ───────────────────────────────────────────────────────────────────

const TaskContext = createContext<TaskContextType | null>(null);

export function TaskProvider({ children }: { children: ReactNode }) {
  const { user } = useAuth();
  const [tasks, setTasks] = useState<Task[]>([]);
  const [workspaces, setWorkspaces] = useState<Workspace[]>([]);
  const [loading, setLoading] = useState(true);

  async function refreshWorkspaces() {
    const [wsList, taskList] = await Promise.all([getWorkspaces(), getTasks()]);
    setWorkspaces(wsList);
    setTasks(taskList.map(t => apiTaskToTask(t, wsList)));
  }

  useEffect(() => {
    if (!user) {
      setWorkspaces([]);
      setTasks([]);
      setLoading(false);
      return;
    }
    async function load() {
      setLoading(true);
      const [wsList, taskList] = await Promise.all([getWorkspaces(), getTasks()]);
      setWorkspaces(wsList);
      setTasks(taskList.map(t => apiTaskToTask(t, wsList)));
      setLoading(false);
    }
    load();
  }, [user?.id]);

  useEffect(() => {
    window.addEventListener('ai-task-updated', refreshWorkspaces);
    return () => window.removeEventListener('ai-task-updated', refreshWorkspaces);
  }, []);

  async function addTask(data: { title: string; workspace_id: number; status?: TaskStatus; priority?: Priority; due_date?: string }) {
    const created = await createTask(data);
    setTasks(prev => [...prev, apiTaskToTask(created, workspaces)]);
  }

  async function updateTask(id: number, updates: Partial<Task>) {
    const apiUpdates: Record<string, string> = {};
    if (updates.title) apiUpdates.title = updates.title;
    if (updates.status) apiUpdates.status = updates.status;
    if (updates.priority) apiUpdates.priority = updates.priority;
    if (updates.dueDate) apiUpdates.due_date = updates.dueDate.toISOString().split("T")[0];

    const updated = await apiUpdateTask(id, apiUpdates);
    setTasks(prev => prev.map(t => t.id === id ? apiTaskToTask(updated, workspaces) : t));
  }

  async function deleteTask(id: number) {
    await apiDeleteTask(id);
    setTasks(prev => prev.filter(t => t.id !== id));
  }

  return (
    <TaskContext.Provider value={{ tasks, workspaces, loading, addTask, updateTask, deleteTask, refreshWorkspaces }}>
      {children}
    </TaskContext.Provider>
  );
}

export function useTasks() {
  const ctx = useContext(TaskContext);
  if (!ctx) throw new Error("useTasks must be used inside TaskProvider");
  return ctx;
}