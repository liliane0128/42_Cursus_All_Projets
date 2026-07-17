import React, { useState, useRef } from 'react';
import { createPortal } from 'react-dom';
import { Task, statusCfg } from './TasksPanel';
import { WorkspaceMember } from '../../api';
import { useChat } from '../../context/ChatContext';

interface Props {
  tasks: Task[];
  members: WorkspaceMember[];
}

function parseDue(d: Date | string | null | undefined): Date | null {
  if (!d) return null;
  const parsed = new Date(d as any);
  return isNaN(parsed.getTime()) ? null : parsed;
}

function dueLabelShort(d: Date): string {
  const base = new Date();
  base.setHours(0, 0, 0, 0);
  const days = Math.ceil((d.getTime() - base.getTime()) / 86_400_000);
  if (days < 0) return 'Overdue';
  if (days === 0) return 'Today';
  if (days === 1) return 'Tmrw';
  return `${days}d`;
}

const STATUS_DOT: Record<string, string> = {
  completed:   '#22c55e',
  in_progress: '#6366f1',
  in_review:   '#f59e0b',
  todo:        '#9ca3af',
};

type Section = 'pending' | 'done' | 'members' | 'recent' | 'due';

export default function DashboardOverview({ tasks, members }: Props) {
  const [hovered, setHovered]       = useState<Section | null>(null);
  const [anchorRect, setAnchorRect] = useState<DOMRect | null>(null);
  const closeTimer = useRef<ReturnType<typeof setTimeout> | null>(null);

  const { onlineUsers } = useChat();
  const onlineIds = new Set(onlineUsers.map(u => u.id));

  const total        = tasks.length;
  const doneTasks    = tasks.filter(t => t.status === 'completed');
  const pendingTasks = tasks.filter(t => t.status === 'in_progress' || t.status === 'in_review');
  const progressPct  = total ? Math.round((doneTasks.length / total) * 100) : 0;

  const now     = new Date();
  const in7Days = new Date(now.getTime() + 7 * 24 * 60 * 60 * 1000);

  const dueSoonTasks = tasks
    .filter(t => {
      if (t.status === 'completed') return false;
      const d = parseDue(t.dueDate);
      return d !== null && d >= now && d <= in7Days;
    })
    .sort((a, b) => parseDue(a.dueDate)!.getTime() - parseDue(b.dueDate)!.getTime())
    .slice(0, 6);

  const recentTasks = [...tasks].sort((a, b) => b.id - a.id).slice(0, 5);

  // ── hover helpers ──────────────────────────────────────────────────────────
  const open = (s: Section, e: React.MouseEvent<HTMLDivElement>) => {
    if (closeTimer.current) clearTimeout(closeTimer.current);
    setAnchorRect(e.currentTarget.getBoundingClientRect());
    setHovered(s);
  };
  const startClose  = () => { closeTimer.current = setTimeout(() => setHovered(null), 120); };
  const cancelClose = () => { if (closeTimer.current) clearTimeout(closeTimer.current); };

  // ── popover ────────────────────────────────────────────────────────────────
  const popover = () => {
    if (!hovered || !anchorRect) return null;
    let w = 240;
    let body: React.ReactNode;

    switch (hovered) {
      case 'pending':
        body = (
          <>
            <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">
              In Progress &amp; In Review · {pendingTasks.length}
            </p>
            <div className="space-y-2 max-h-52 overflow-y-auto">
              {pendingTasks.length === 0 ? (
                <p className="text-[11px] text-gray-400">No active tasks</p>
              ) : pendingTasks.map(t => (
                <div key={t.id} className="flex items-center gap-2">
                  <span className={`text-[9px] font-semibold px-1.5 py-0.5 rounded flex-shrink-0 ${statusCfg[t.status].bg} ${statusCfg[t.status].text}`}>
                    {statusCfg[t.status].label}
                  </span>
                  <span className="text-[11px] text-gray-700 truncate">{t.title}</span>
                </div>
              ))}
            </div>
          </>
        );
        break;

      case 'done':
        body = (
          <>
            <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">
              Completed · {doneTasks.length}
            </p>
            <div className="space-y-2 max-h-52 overflow-y-auto">
              {doneTasks.length === 0 ? (
                <p className="text-[11px] text-gray-400">No completed tasks yet</p>
              ) : doneTasks.map(t => (
                <div key={t.id} className="flex items-center gap-2">
                  <span className="w-3 h-3 rounded-full bg-green-100 flex items-center justify-center flex-shrink-0">
                    <span className="w-1.5 h-1.5 rounded-full bg-green-500 block" />
                  </span>
                  <span className="text-[11px] text-gray-700 truncate">{t.title}</span>
                </div>
              ))}
            </div>
          </>
        );
        break;

      case 'members':
        w = 230;
        body = (
          <>
            <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">
              Team · {members.length}
            </p>
            <div className="space-y-2 max-h-52 overflow-y-auto">
              {members.length === 0 ? (
                <p className="text-[11px] text-gray-400">No members</p>
              ) : members.map(m => (
                <div key={m.user_id} className="flex items-center gap-2.5">
                  <div className="relative w-6 h-6 flex-shrink-0">
                    <div className="w-6 h-6 rounded-full overflow-hidden">
                      {m.avatar
                        ? <img src={m.avatar} alt={m.login} className="w-full h-full object-cover" />
                        : <div className="w-full h-full bg-gray-200 flex items-center justify-center text-[9px] font-bold text-gray-500 uppercase">{m.login[0]}</div>
                      }
                    </div>
                    <span className={`absolute bottom-0 right-0 w-2 h-2 rounded-full border border-white ${onlineIds.has(String(m.user_id)) ? 'bg-green-400' : 'bg-gray-300'}`} />
                  </div>
                  <div className="min-w-0 flex-1">
                    <p className="text-[11px] font-medium text-gray-800 truncate">{m.display_name}</p>
                    <p className="text-[9px] text-gray-400">@{m.login}</p>
                  </div>
                  <span className={`text-[9px] px-1.5 py-0.5 rounded font-semibold flex-shrink-0 capitalize ${
                    m.role === 'owner' ? 'bg-gray-900 text-white' :
                    m.role === 'admin' ? 'bg-blue-50 text-blue-600' :
                    'bg-gray-100 text-gray-500'
                  }`}>{m.role}</span>
                </div>
              ))}
            </div>
          </>
        );
        break;

      case 'recent':
        body = (
          <>
            <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">Recent Tasks</p>
            <div className="space-y-2">
              {recentTasks.length === 0 ? (
                <p className="text-[11px] text-gray-400">No tasks yet</p>
              ) : recentTasks.map(t => (
                <div key={t.id} className="flex items-center gap-2">
                  <span className={`text-[9px] font-semibold px-1.5 py-0.5 rounded flex-shrink-0 ${statusCfg[t.status].bg} ${statusCfg[t.status].text}`}>
                    {statusCfg[t.status].label}
                  </span>
                  <span className="text-[11px] text-gray-700 truncate">{t.title}</span>
                </div>
              ))}
            </div>
          </>
        );
        break;

      case 'due':
        body = (
          <>
            <p className="text-[10px] font-bold text-gray-400 uppercase tracking-wider mb-2.5">Due This Week</p>
            <div className="space-y-2">
              {dueSoonTasks.length === 0 ? (
                <p className="text-[11px] text-gray-400">Nothing due this week</p>
              ) : dueSoonTasks.map(t => {
                const d = parseDue(t.dueDate)!;
                const label = dueLabelShort(d);
                const isOverdue = d < new Date();
                return (
                  <div key={t.id} className="flex items-center gap-2">
                    <span className={`text-[9px] font-bold px-1.5 py-0.5 rounded flex-shrink-0 ${isOverdue ? 'bg-red-50 text-red-500' : 'bg-amber-50 text-amber-600'}`}>
                      {label}
                    </span>
                    <span className="text-[11px] text-gray-700 truncate flex-1">{t.title}</span>
                    <span className={`text-[9px] flex-shrink-0 ${
                      t.priority === 'high' ? 'text-red-400' :
                      t.priority === 'medium' ? 'text-amber-400' : 'text-gray-300'
                    }`}>●</span>
                  </div>
                );
              })}
            </div>
          </>
        );
        break;
    }

    const left = Math.min(anchorRect.left, window.innerWidth - w - 16);
    const top  = anchorRect.bottom + 6;

    return createPortal(
      <div
        style={{ position: 'fixed', top, left, width: w, zIndex: 9999 }}
        className="bg-white border border-gray-100 rounded-xl shadow-xl p-3.5"
        onMouseEnter={cancelClose}
        onMouseLeave={startClose}
      >
        {body}
      </div>,
      document.body
    );
  };

  const cls = (flex: string, extra = '') =>
    `${flex} px-5 pt-[14px] pb-[12px] flex flex-col min-w-0 ${extra}`;

  const title = (label: string) => (
    <span className="text-[9px] font-bold text-gray-400 uppercase tracking-widest leading-none mb-2 flex-shrink-0">
      {label}
    </span>
  );

  return (
    <>
      <div className="bg-white border border-gray-100 rounded-xl shadow-sm flex h-[88px] select-none flex-shrink-0">

        {/* ── PROGRESS (flex-3) ─────────────────────────────────────────── */}
        <div className={cls('flex-[3]')}>
          {title('Progress')}
          <div className="flex items-center gap-2.5 mt-auto mb-1">
            <div className="flex-1 bg-gray-100 rounded-full h-1.5 overflow-hidden">
              <div className="bg-indigo-600 h-1.5 rounded-full transition-all duration-500" style={{ width: `${progressPct}%` }} />
            </div>
            <span className="text-[13px] font-black text-indigo-600 w-7 text-right flex-shrink-0 leading-none">{progressPct}%</span>
          </div>
          <span className="text-[10px] text-gray-400 leading-none">{doneTasks.length} of {total} tasks</span>
        </div>

        {/* ── PENDING (flex-2, hover) ───────────────────────────────────── */}
        <div className={cls('flex-[2]', 'hover:bg-gray-50/60 transition-colors cursor-default')}
          onMouseEnter={e => open('pending', e)} onMouseLeave={startClose}>
          {title('Pending')}
          <span className="text-[24px] font-black text-amber-500 leading-none mt-auto mb-0.5">{pendingTasks.length}</span>
          <span className="text-[10px] text-gray-400 leading-none">tasks active</span>
        </div>

        {/* ── DONE (flex-2, hover) ─────────────────────────────────────── */}
        <div className={cls('flex-[2]', 'hover:bg-gray-50/60 transition-colors cursor-default')}
          onMouseEnter={e => open('done', e)} onMouseLeave={startClose}>
          {title('Done')}
          <span className="text-[24px] font-black text-green-600 leading-none mt-auto mb-0.5">{doneTasks.length}</span>
          <span className="text-[10px] text-gray-400 leading-none">{total - doneTasks.length} remaining</span>
        </div>

        {/* ── MEMBERS (flex-2, hover) ───────────────────────────────────── */}
        <div className={cls('flex-[2]', 'hover:bg-gray-50/60 transition-colors cursor-default')}
          onMouseEnter={e => open('members', e)} onMouseLeave={startClose}>
          {title('Members')}
          <span className="text-[24px] font-black text-blue-600 leading-none mt-auto mb-0.5">{members.length}</span>
          <span className="text-[10px] text-gray-400 leading-none">collaborators</span>
        </div>

        {/* ── RECENT ACTIVITIES (flex-3, hover) ────────────────────────── */}
        <div className={cls('flex-[3]', 'hover:bg-gray-50/60 transition-colors cursor-default')}
          onMouseEnter={e => open('recent', e)} onMouseLeave={startClose}>
          {title('Recent Activities')}
          <div className="flex flex-col gap-[5px] mt-auto">
            {recentTasks.length === 0 ? (
              <span className="text-[10px] text-gray-300">No tasks yet</span>
            ) : recentTasks.slice(0, 3).map(t => (
              <div key={t.id} className="flex items-center gap-1.5 min-w-0">
                <span className="w-1.5 h-1.5 rounded-full flex-shrink-0" style={{ background: STATUS_DOT[t.status] ?? '#9ca3af' }} />
                <span className="text-[10px] text-gray-600 truncate leading-none">{t.title}</span>
              </div>
            ))}
          </div>
        </div>

        {/* ── DUE SOON (flex-3, hover) ─────────────────────────────────── */}
        <div className={cls('flex-[3]', 'hover:bg-gray-50/60 transition-colors cursor-default rounded-r-xl')}
          onMouseEnter={e => open('due', e)} onMouseLeave={startClose}>
          {title('Due Soon')}
          <div className="flex flex-col gap-[5px] mt-auto">
            {dueSoonTasks.length === 0 ? (
              <span className="text-[10px] text-gray-300">Nothing due this week</span>
            ) : dueSoonTasks.slice(0, 3).map(t => {
              const d = parseDue(t.dueDate)!;
              const isOverdue = d < new Date();
              return (
                <div key={t.id} className="flex items-center gap-1.5 min-w-0">
                  <span className={`text-[9px] font-bold px-1 rounded leading-[14px] flex-shrink-0 ${isOverdue ? 'bg-red-50 text-red-500' : 'bg-amber-50 text-amber-600'}`}>
                    {dueLabelShort(d)}
                  </span>
                  <span className="text-[10px] text-gray-600 truncate leading-none">{t.title}</span>
                </div>
              );
            })}
          </div>
        </div>

      </div>

      {popover()}
    </>
  );
}
