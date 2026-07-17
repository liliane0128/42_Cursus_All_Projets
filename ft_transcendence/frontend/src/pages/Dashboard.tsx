import Logo42 from "../components/Logo42";
import UserMenu from "../components/UserMenu";
import { useState, useRef, useEffect } from "react";
import { createPortal } from "react-dom";
import {
  LayoutDashboard, FolderKanban,
  CheckSquare, MessageSquare, Plus, ChevronRight,
  Clock, Users, TrendingUp, Zap, ChevronDown,
} from "lucide-react";
import { useNavigate } from "react-router-dom";
import { useTasks, TaskStatus, Priority } from "../context/TaskContext";
import { useAuth } from "../context/AuthContext";
import { getWorkspaceMembers, WorkspaceMember, mediaUrl } from "../api";
import NotificationPanel from "../components/NotificationPanel";

// ── Config ────────────────────────────────────────────────────────────────────

const today = new Date();

const statusCfg: Record<TaskStatus, { label: string; bg: string; text: string }> = {
  todo: { label: "To Do", bg: "bg-gray-100", text: "text-gray-500" },
  in_progress: { label: "In Progress", bg: "bg-indigo-50", text: "text-indigo-600" },
  in_review: { label: "In Review", bg: "bg-amber-50", text: "text-amber-600" },
  completed: { label: "Completed", bg: "bg-green-50", text: "text-green-600" },
};

const priorityCfg: Record<Priority, { label: string; color: string; dot: string }> = {
  high: { label: "High", color: "text-red-500", dot: "bg-red-500" },
  medium: { label: "Med", color: "text-amber-500", dot: "bg-amber-400" },
  low: { label: "Low", color: "text-gray-400", dot: "bg-gray-300" },
};

const projectStatusCfg: Record<string, { bg: string; text: string }> = {
  "In Progress": { bg: "bg-indigo-50", text: "text-indigo-600" },
  "Completed": { bg: "bg-green-50", text: "text-green-700" },
};

function timeProgress(start: Date, due: Date) {
  const total = due.getTime() - start.getTime();
  const elapsed = today.getTime() - start.getTime();
  const pct = Math.min(100, Math.max(0, Math.round((elapsed / total) * 100)));
  const daysLeft = Math.ceil((due.getTime() - today.getTime()) / (1000 * 60 * 60 * 24));
  const overdue = daysLeft < 0;
  const label = overdue ? "Overdue" : daysLeft === 0 ? "Due today" : `${daysLeft}d left`;
  return { pct, label, overdue };
}

function formatDate(d: Date) {
  return d.toLocaleDateString("en-US", { month: "short", day: "numeric" });
}

// ── Sub-components ────────────────────────────────────────────────────────────

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

function ProgressBar({ value, color }: { value: number; color: string }) {
  return (
    <div className="w-full h-[3px] bg-gray-100 rounded-full overflow-hidden">
      <div className="h-full rounded-full transition-all" style={{ width: `${value}%`, backgroundColor: color }} />
    </div>
  );
}

function StatusBadge({ taskId, status }: { taskId: number; status: TaskStatus }) {
  const { updateTask } = useTasks();
  const [open, setOpen] = useState(false);
  const ref = useRef<HTMLDivElement>(null);
  const cfg = statusCfg[status];

  useEffect(() => {
    function handler(e: MouseEvent) {
      if (ref.current && !ref.current.contains(e.target as Node)) setOpen(false);
    }
    document.addEventListener("mousedown", handler);
    return () => document.removeEventListener("mousedown", handler);
  }, []);

  return (
    <div ref={ref} className="relative">
      <button
        onClick={e => { e.stopPropagation(); setOpen(o => !o); }}
        className={`text-[9px] px-1.5 py-0.5 rounded font-medium flex items-center gap-1 ${cfg.bg} ${cfg.text}`}>
        {cfg.label} <ChevronDown size={8} />
      </button>
      {open && (
        <div className="absolute top-full mt-1 left-0 bg-white border border-gray-100 rounded-lg shadow-lg z-50 py-1 min-w-[120px]">
          {(Object.keys(statusCfg) as TaskStatus[]).map(s => (
            <button
              key={s}
              onClick={e => { e.stopPropagation(); updateTask(taskId, { status: s }); setOpen(false); }}
              className={`w-full flex items-center gap-2 px-3 py-1.5 text-[12px] hover:bg-gray-50 transition-colors
                ${s === status ? "font-semibold" : "font-normal"}`}>
              <span className={`w-1.5 h-1.5 rounded-full ${s === "todo" ? "bg-gray-400" :
                s === "in_progress" ? "bg-indigo-400" :
                  s === "in_review" ? "bg-amber-400" :
                    "bg-green-400"
                }`} />
              {statusCfg[s].label}
            </button>
          ))}
        </div>
      )}
    </div>
  );
}

// ── StatCard popover content types ──────────────────────────────────────────

type PopoverItem =
  | { kind: "workspace"; name: string; color: string }
  | { kind: "task"; title: string; project: string; wsColor: string; priority: Priority }
  | { kind: "completed"; title: string; project: string; wsColor: string }
  | { kind: "member"; id: number; name: string; avatar: string | null };

function StatCardPopover({
  rect,
  title,
  count,
  items,
  accentColor,
  onMouseEnter,
  onMouseLeave,
}: {
  rect: DOMRect;
  title: string;
  count: number;
  items: PopoverItem[];
  accentColor: string;
  onMouseEnter: () => void;
  onMouseLeave: () => void;
}) {
  return createPortal(
    <div
      style={{
        position: "fixed",
        top: rect.bottom + 6,
        left: Math.min(rect.left, window.innerWidth - 240 - 16),
        width: 240,
        zIndex: 9999,
      }}
      className="bg-white border border-gray-100 rounded-xl shadow-xl p-3.5"
      onMouseEnter={onMouseEnter}
      onMouseLeave={onMouseLeave}
    >
      <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">
        {title} · {count}
      </p>
      <div className="space-y-2 max-h-52 overflow-y-auto">
        {items.length === 0 ? (
          <p className="text-[11px] text-gray-400">Nothing here yet</p>
        ) : items.map((item, i) => {
          if (item.kind === "workspace") return (
            <div key={i} className="flex items-center gap-2 py-1 px-2 rounded-lg hover:bg-gray-50 transition-colors">
              <div className="w-2 h-2 rounded-full flex-shrink-0" style={{ backgroundColor: item.color }} />
              <p className="text-[11px] font-medium text-gray-800 truncate">{item.name}</p>
            </div>
          );
          if (item.kind === "task") {
            const pri = priorityCfg[item.priority];
            return (
              <div key={i} className="flex items-center gap-2 py-1 px-2 rounded-lg hover:bg-gray-50 transition-colors">
                <div className="w-1.5 h-1.5 rounded-full flex-shrink-0" style={{ backgroundColor: item.wsColor }} />
                <p className="text-[11px] text-gray-700 truncate flex-1">{item.title}</p>
                <span className={`text-[9px] font-medium flex-shrink-0 ${pri.color}`}>{pri.label}</span>
              </div>
            );
          }
          if (item.kind === "completed") return (
            <div key={i} className="flex items-center gap-2 py-1 px-2 rounded-lg hover:bg-gray-50 transition-colors">
              <div className="w-1.5 h-1.5 rounded-full bg-green-400 flex-shrink-0" />
              <p className="text-[11px] text-gray-700 truncate flex-1">{item.title}</p>
              <span
                className="text-[9px] flex-shrink-0 px-1.5 py-0.5 rounded font-medium"
                style={{ backgroundColor: `${item.wsColor}18`, color: item.wsColor }}
              >
                {item.project}
              </span>
            </div>
          );
          if (item.kind === "member") return (
            <div key={i} className="flex items-center gap-2.5">
              {item.avatar ? (
                <img
                  src={mediaUrl(item.avatar) ?? undefined}
                  alt={item.name}
                  className="w-6 h-6 rounded-full object-cover flex-shrink-0"
                />
              ) : (
                <div className="w-6 h-6 rounded-full bg-violet-100 flex items-center justify-center text-[9px] font-bold text-violet-600 uppercase flex-shrink-0">
                  {item.name[0]}
                </div>
              )}
              <p className="text-[11px] font-medium text-gray-800 truncate">{item.name}</p>
            </div>
          );
          return null;
        })}
      </div>
    </div>,
    document.body
  );
}

function StatCard({ label, value, sub, Icon, iconBg, iconColor, valueColor, border, accent, popoverItems, popoverTitle }: {
  label: string; value: string; sub: string;
  Icon: React.ElementType; iconBg: string; iconColor: string;
  valueColor: string; border: string; accent: string;
  popoverItems?: PopoverItem[];
  popoverTitle?: string;
}) {
  const [hovered, setHovered] = useState(false);
  const [rect, setRect] = useState<DOMRect | null>(null);
  const closeTimer = useRef<ReturnType<typeof setTimeout> | null>(null);

  const handleMouseEnter = (e: React.MouseEvent<HTMLDivElement>) => {
    if (!popoverItems) return;
    if (closeTimer.current) clearTimeout(closeTimer.current);
    setRect(e.currentTarget.getBoundingClientRect());
    setHovered(true);
  };
  const handleMouseLeave = () => {
    if (!popoverItems) return;
    closeTimer.current = setTimeout(() => setHovered(false), 120);
  };
  const cancelClose = () => {
    if (closeTimer.current) clearTimeout(closeTimer.current);
  };

  return (
    <>
      <div
        onMouseEnter={handleMouseEnter}
        onMouseLeave={handleMouseLeave}
        className={`bg-white border ${border} rounded-xl p-4 relative overflow-hidden ${popoverItems ? "cursor-default" : ""}`}>
        <div className={`absolute top-0 left-0 right-0 h-[3px] ${accent}`} />
        <div className="flex items-start justify-between mb-2.5">
          <p className="text-[11px] text-gray-400 font-medium">{label}</p>
          <div className={`w-6 h-6 rounded-md ${iconBg} flex items-center justify-center`}>
            <Icon size={13} className={iconColor} />
          </div>
        </div>
        <p className={`text-[22px] font-semibold leading-none tracking-tight ${valueColor}`}>{value}</p>
        <p className="text-[10px] text-gray-400 mt-1">{sub}</p>
      </div>

      {hovered && popoverItems && rect && (
        <StatCardPopover
          rect={rect}
          title={popoverTitle ?? label}
          count={popoverItems.length}
          items={popoverItems}
          accentColor={accent}
          onMouseEnter={cancelClose}
          onMouseLeave={handleMouseLeave}
        />
      )}
    </>
  );
}

// ── Main ──────────────────────────────────────────────────────────────────────

export default function Dashboard() {
  const navigate = useNavigate();
  const { user } = useAuth();
  const { tasks } = useTasks();
  const { workspaces } = useTasks();

  const activeWorkspaces = workspaces.filter(ws => !ws.is_completed);

  const projects = activeWorkspaces.map(ws => ({
    id: ws.id,
    name: ws.name,
    color: ws.color,
    members: 1,
    startDate: new Date(ws.created_at),
    dueDate: ws.deadline
      ? new Date(ws.deadline)
      : new Date(new Date(ws.created_at).getTime() + 30 * 24 * 60 * 60 * 1000),
    status: "In Progress",
  }));

  const myTasks = tasks.filter(t => t.status !== "completed");
  const completedTasks = tasks.filter(t => t.status === "completed");
  const openTaskCount = myTasks.length;
  const completedCount = completedTasks.length;

  // Team members aggregated across every workspace the user belongs to,
  // deduplicated and excluding the current user. Polled instead of relying
  // on the WebSocket presence list, which proved unreliable.
  const [teamMembers, setTeamMembers] = useState<WorkspaceMember[]>([]);

  useEffect(() => {
    let cancelled = false;

    async function loadTeamMembers() {
      if (workspaces.length === 0) {
        if (!cancelled) setTeamMembers([]);
        return;
      }
      try {
        const results = await Promise.all(workspaces.map(ws => getWorkspaceMembers(ws.id)));
        if (cancelled) return;
        const merged = new Map<number, WorkspaceMember>();
        results.flat().forEach(m => {
          if (m.user_id === user?.id) return;
          merged.set(m.user_id, m);
        });
        setTeamMembers(Array.from(merged.values()));
      } catch {
        if (!cancelled) setTeamMembers([]);
      }
    }

    loadTeamMembers();
    const interval = setInterval(loadTeamMembers, 15000);
    return () => { cancelled = true; clearInterval(interval); };
  }, [workspaces, user?.id]);

  // Each stat card has its own color scheme
  // Popover data for each card
  const activeProjectsItems: PopoverItem[] = workspaces.map(ws => ({
    kind: "workspace",
    name: ws.name,
    color: ws.color,
  }));

  const openTasksItems: PopoverItem[] = myTasks.map(t => ({
    kind: "task",
    title: t.title,
    project: t.project,
    wsColor: t.wsColor,
    priority: t.priority,
  }));

  const completedItems: PopoverItem[] = completedTasks.map(t => ({
    kind: "completed",
    title: t.title,
    project: t.project,
    wsColor: t.wsColor,
  }));

  const memberItems: PopoverItem[] = teamMembers.map(m => ({
    kind: "member",
    id: m.user_id,
    name: m.display_name,
    avatar: m.avatar,
  }));

  const stats = [
    {
      label: "Active Projects",
      value: String(activeWorkspaces.length),
      sub: "+1 this month",
      Icon: FolderKanban,
      iconBg: "bg-indigo-50",
      iconColor: "text-indigo-500",
      valueColor: "text-indigo-600",
      border: "border-indigo-100",
      accent: "bg-indigo-500",
      popoverItems: activeProjectsItems,
      popoverTitle: "Workspaces",
    },
    {
      label: "Open Tasks",
      value: String(openTaskCount),
      sub: "across all workspaces",
      Icon: CheckSquare,
      iconBg: "bg-amber-50",
      iconColor: "text-amber-500",
      valueColor: "text-amber-600",
      border: "border-amber-100",
      accent: "bg-amber-400",
      popoverItems: openTasksItems,
      popoverTitle: "Open Tasks",
    },
    {
      label: "Completed",
      value: String(completedCount),
      sub: "total",
      Icon: TrendingUp,
      iconBg: "bg-green-50",
      iconColor: "text-green-500",
      valueColor: "text-green-600",
      border: "border-green-100",
      accent: "bg-green-500",
      popoverItems: completedItems,
      popoverTitle: "Completed",
    },
    {
      label: "Team Members",
      value: String(teamMembers.length),
      sub: "across your workspaces",
      Icon: Users,
      iconBg: "bg-violet-50",
      iconColor: "text-violet-500",
      valueColor: "text-violet-600",
      border: "border-violet-100",
      accent: "bg-violet-500",
      popoverItems: memberItems,
      popoverTitle: "Team Members",
    },
  ];

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
          <NavItem icon={LayoutDashboard} label="Dashboard" active to="/dashboard" />
          <NavItem icon={FolderKanban} label="Workspace" to="/workspace" />
          <NavItem icon={CheckSquare} label="Tasks" to="/tasks" />
          <NavItem icon={MessageSquare} label="Chats" to="/chats" />
        </nav>
      </aside>

      {/* Main */}
      <div className="flex-1 flex flex-col min-w-0">
        <header className="h-[52px] bg-white border-b border-gray-100 flex items-center justify-between px-6 flex-shrink-0">
          <div>
            <p className="text-[14px] font-semibold">Dashboard</p>
            <p className="text-[11px] text-gray-400 mt-0.5">Welcome back, {user?.displayName ?? user?.login ?? ""}</p>
          </div>

          <div className="flex items-center gap-1">
            <NotificationPanel />
            <UserMenu />
          </div>
        </header>

        <main className="flex-1 flex flex-col min-h-0 px-6 py-5 gap-3">

          {/* Stats */}
          <div className="grid grid-cols-4 gap-2.5 flex-shrink-0">
            {stats.map(({ label, value, sub, Icon, iconBg, iconColor, valueColor, border, accent, popoverItems, popoverTitle }) => (
              <StatCard
                key={label}
                label={label}
                value={value}
                sub={sub}
                Icon={Icon}
                iconBg={iconBg}
                iconColor={iconColor}
                valueColor={valueColor}
                border={border}
                accent={accent}
                popoverItems={popoverItems}
                popoverTitle={popoverTitle}
              />
            ))}
          </div>

          {/* Two-column cards */}
          <div className="grid grid-cols-2 gap-3 flex-1 min-h-0">

            {/* Workspace Overview */}
            <div className="bg-white border border-gray-100 rounded-xl p-5 flex flex-col min-h-0">
              <div className="flex items-center justify-between mb-4 flex-shrink-0">
                <h2 className="text-[13px] font-semibold">Workspace Overview</h2>
                <button onClick={() => navigate("/workspace")} className="text-[11px] text-gray-400 hover:text-gray-900 flex items-center gap-1 transition-colors">
                  View all <ChevronRight size={11} />
                </button>
              </div>
              <div className="space-y-4 overflow-y-auto flex-1 pr-1">
                {[...projects]
                  .sort((a, b) => (a.status === "Completed" ? 1 : 0) - (b.status === "Completed" ? 1 : 0))
                  .map(p => {
                    const { pct, label: timeLabel, overdue } = timeProgress(p.startDate, p.dueDate);
                    const sc = projectStatusCfg[p.status] ?? { bg: "bg-gray-100", text: "text-gray-500" };
                    return (
                      <div key={p.id} className="cursor-pointer" onClick={() => navigate("/workspace", { state: { workspaceId: p.id } })}>
                        <div className="flex items-start justify-between mb-2">
                          <div className="flex items-center gap-2">
                            <div className="w-[7px] h-[7px] rounded-full mt-0.5 flex-shrink-0" style={{ backgroundColor: p.color }} />
                            <div>
                              <p className="text-[12px] font-medium">{p.name}</p>
                              <p className="text-[10px] text-gray-400 mt-0.5 flex items-center gap-2">
                                <span className="flex items-center gap-1"><Users size={9} />{p.members}</span>
                                <span className="flex items-center gap-1"><Clock size={9} />{formatDate(p.dueDate)}</span>
                              </p>
                            </div>
                          </div>
                          <span className={`text-[10px] px-2 py-0.5 rounded-full font-medium ${sc.bg} ${sc.text}`}>{p.status}</span>
                        </div>
                        <div className="flex items-center gap-2.5">
                          <ProgressBar value={pct} color={p.status === "Completed" ? "#22c55e" : overdue ? "#ef4444" : p.color} />
                          <span className={`text-[10px] w-14 text-right flex-shrink-0 ${overdue ? "text-red-400" : "text-gray-400"}`}>
                            {timeLabel}
                          </span>
                        </div>
                      </div>
                    );
                  })}
              </div>
            </div>

            {/* My Tasks */}
            <div className="bg-white border border-gray-100 rounded-xl p-5 flex flex-col min-h-0">
              <div className="flex items-center justify-between mb-4 flex-shrink-0">
                <h2 className="text-[13px] font-semibold">My Tasks</h2>
                <button
                  onClick={() => navigate("/tasks")}
                  className="text-[11px] text-gray-400 hover:text-gray-900 flex items-center gap-1 transition-colors">
                  View all <ChevronRight size={11} />
                </button>
              </div>
              <div className="space-y-1.5 overflow-y-auto flex-1 pr-1">
                {myTasks.map(t => {
                  const pri = priorityCfg[t.priority];
                  return (
                    <div key={t.id} className="flex items-center gap-2 px-2.5 py-2 rounded-lg bg-gray-50 hover:bg-gray-100 border border-gray-100 transition-colors cursor-pointer">
                      <span className="flex-1 text-[12px] text-gray-600 truncate">{t.title}</span>
                      <span className="text-[9px] px-1.5 py-0.5 rounded font-medium flex-shrink-0"
                        style={{ backgroundColor: `${t.wsColor}18`, color: t.wsColor }}>
                        {t.project}
                      </span>
                      <StatusBadge taskId={t.id} status={t.status} />
                      <span className={`text-[9px] font-medium flex-shrink-0 flex items-center gap-1 ${pri.color}`}>
                        <span className={`w-[5px] h-[5px] rounded-full ${pri.dot}`} />{pri.label}
                      </span>
                      <span className="text-[9px] text-gray-400 w-12 text-right flex-shrink-0">
                        {t.dueDate.toLocaleDateString("en-US", { month: "short", day: "numeric" })}
                      </span>
                    </div>
                  );
                })}
              </div>
            </div>
          </div>

          {/* Create Workspace */}
          <button
            onClick={() => navigate("/workspace", { state: { openCreateWorkspace: true } })}
            className="w-full h-14 flex-shrink-0 bg-white border border-gray-100 hover:border-indigo-500 hover:bg-indigo-50/20 rounded-xl flex items-center justify-center gap-3 transition-all group shadow-2xs cursor-pointer"
          >
            <div className="w-6 h-6 rounded-md bg-indigo-50 group-hover:bg-indigo-600 flex items-center justify-center text-indigo-600 group-hover:text-white transition-colors">
              <Plus size={12} strokeWidth={3} />
            </div>
            <div className="text-left">
              <span className="block text-xs font-bold text-gray-800 group-hover:text-indigo-600 transition-colors">Create New Workspace</span>
              <span className="block text-[10px] text-gray-400">Set up a new workspace with custom tools instantly</span>
            </div>
          </button>

        </main>
      </div>
    </div>
  );
}