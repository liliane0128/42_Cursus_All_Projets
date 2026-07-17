const BASE = "";

// Convert a backend path (e.g. "/api/files/avatar.jpg") to a usable URL.
// Absolute URLs (42 OAuth avatars, etc.) are returned as-is.
export function mediaUrl(path: string | null | undefined): string | null {
  if (!path) return null;
  if (path.startsWith("http")) return path;
  return path; // relative path → served via Vite proxy
}

function getToken(): string | null {
  return localStorage.getItem("jwt_token") || localStorage.getItem("token") || localStorage.getItem("access_token");
}

async function request<T>(url: string, options: RequestInit = {}): Promise<T> {
  const token = getToken();

  const headers = new Headers(options.headers);
  
  if (!headers.has("Content-Type")) {
    headers.set("Content-Type", "application/json");
  }
  
  if (token) {
    const authValue = token.startsWith("Bearer ") ? token : `Bearer ${token}`;
    headers.set("Authorization", authValue);
  } else {
    console.warn(`[API Control] No token found under 'jwt_token', 'token', or 'access_token' for ${url}`);
  }

  const response = await fetch(url, {
    ...options,
    headers,
    credentials: "include",
  });

  if (!response.ok) {
    const errorData = await response.json().catch(() => ({}));

    if (response.status === 401 && !url.includes("/api/messages")) {
      console.warn("Token expired! Clearing storage and redirecting...");

      localStorage.removeItem("jwt_token");
      localStorage.removeItem("token");
      localStorage.removeItem("access_token");

      window.location.href = "/login";
    }
    console.error(`[API Response Error] ${response.status}:`, errorData);
    throw new Error(errorData.detail || "API Error");
  }

  return response.json();
}

// ── Workspace Types & API ───────────────────────────────────────────────────

export interface Workspace {
  id: number;
  name: string;
  description: string;
  color: string;
  deadline: string | null;
  is_completed: boolean;
  created_at: string;
}

export async function getWorkspaces(): Promise<Workspace[]> {
  return request<Workspace[]>(`${BASE}/api/workspaces`);
}

export async function createWorkspace(data: { name: string; description?: string; color?: string; deadline?: string | null }): Promise<Workspace> {
  return request<Workspace>(`${BASE}/api/workspaces`, {
    method: "POST",
    body: JSON.stringify(data),
  });
}

export async function deleteWorkspace(id: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${id}`, { method: "DELETE" });
}

export async function updateWorkspaceApi(id: number, data: { name?: string; description?: string; color?: string; deadline?: string | null; is_completed?: boolean }): Promise<Workspace> {
  return request<Workspace>(`${BASE}/api/workspaces/${id}`, {
    method: "PATCH",
    body: JSON.stringify(data),
  });
}

export async function inviteMember(workspaceId: number, loginOrEmail: string, role: "member" | "admin" = "member"): Promise<{ message: string }> {
  return request<{ message: string }>(`${BASE}/api/workspaces/${workspaceId}/invite`, {
    method: "POST",
    body: JSON.stringify({ login_or_email: loginOrEmail, role }),
  });
}

export interface PendingInvite {
  id: number;
  workspace_id: number;
  workspace_name: string;
  inviter_login: string;
  inviter_display_name: string;
  role: "member" | "admin";
  created_at: string;
}

export async function getPendingInvites(): Promise<PendingInvite[]> {
  return request<PendingInvite[]>(`${BASE}/api/invites/pending`);
}

export async function respondToInvite(inviteId: number, action: "accept" | "reject"): Promise<{ message: string; workspace_id?: number }> {
  return request<{ message: string; workspace_id?: number }>(`${BASE}/api/invites/${inviteId}/respond`, {
    method: "POST",
    body: JSON.stringify({ action }),
  });
}

export interface WorkspaceMember {
  user_id: number;
  login: string;
  display_name: string;
  avatar: string | null;
  role: "owner" | "admin" | "member";
}

export async function getWorkspaceMembers(workspaceId: number): Promise<WorkspaceMember[]> {
  return request<WorkspaceMember[]>(`${BASE}/api/workspaces/${workspaceId}/members`);
}

export interface WorkspacePendingInvite {
  id: number;
  invitee_login: string;
  invitee_display_name: string;
  invitee_avatar: string | null;
  role: 'member' | 'admin';
  created_at: string;
}

export async function getWorkspacePendingInvites(workspaceId: number): Promise<WorkspacePendingInvite[]> {
  return request<WorkspacePendingInvite[]>(`${BASE}/api/workspaces/${workspaceId}/pending-invites`);
}

export async function cancelWorkspaceInvite(workspaceId: number, inviteId: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${workspaceId}/invites/${inviteId}`, { method: 'DELETE' });
}

export async function setMemberRole(workspaceId: number, userId: number, role: "admin" | "member"): Promise<{ message: string }> {
  return request<{ message: string }>(`${BASE}/api/workspaces/${workspaceId}/members/${userId}`, {
    method: "PATCH",
    body: JSON.stringify({ role }),
  });
}

export async function kickMember(workspaceId: number, userId: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${workspaceId}/members/${userId}`, { method: "DELETE" });
}

export async function leaveWorkspace(workspaceId: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${workspaceId}/leave`, { method: "DELETE" });
}

export async function transferOwnership(workspaceId: number, newOwnerId: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${workspaceId}/transfer`, {
    method: "PATCH",
    body: JSON.stringify({ new_owner_id: newOwnerId }),
  });
}

export interface WorkspaceFileItem {
  id: number;
  name: string;
  size: string;
  url: string;
  uploadedAt: string;
}

export async function getWorkspaceFiles(workspaceId: number): Promise<WorkspaceFileItem[]> {
  return request<WorkspaceFileItem[]>(`${BASE}/api/workspaces/${workspaceId}/files`);
}

export async function fetchBlobUrl(url: string): Promise<string> {
  const token = getToken();
  const res = await fetch(url, {
    headers: token ? { Authorization: `Bearer ${token}` } : {},
    credentials: "include",
  });
  if (!res.ok) throw new Error("Failed to load file");
  return URL.createObjectURL(await res.blob());
}

export async function downloadFile(url: string, filename: string): Promise<void> {
  const blobUrl = await fetchBlobUrl(url);
  const a = document.createElement("a");
  a.href = blobUrl;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  setTimeout(() => URL.revokeObjectURL(blobUrl), 1000);
}

export function uploadWorkspaceFile(
  workspaceId: number,
  file: File,
  onProgress?: (percent: number) => void,
): Promise<WorkspaceFileItem> {
  return new Promise((resolve, reject) => {
    const token = getToken();
    const form = new FormData();
    form.append("file", file);
    const xhr = new XMLHttpRequest();
    xhr.open("POST", `${BASE}/api/workspaces/${workspaceId}/files`);
    if (token) xhr.setRequestHeader("Authorization", `Bearer ${token}`);
    xhr.withCredentials = true;
    xhr.upload.onprogress = (e) => {
      if (e.lengthComputable && onProgress) onProgress(Math.round((e.loaded / e.total) * 100));
    };
    xhr.onload = () => {
      if (xhr.status >= 200 && xhr.status < 300) {
        try { resolve(JSON.parse(xhr.responseText)); } catch { reject(new Error("Invalid response")); }
      } else {
        try { reject(new Error(JSON.parse(xhr.responseText).detail || "Upload failed")); }
        catch { reject(new Error("Upload failed")); }
      }
    };
    xhr.onerror = () => reject(new Error("Network error"));
    xhr.send(form);
  });
}

export async function deleteWorkspaceFile(workspaceId: number, fileId: number): Promise<void> {
  await request(`${BASE}/api/workspaces/${workspaceId}/files/${fileId}`, { method: "DELETE" });
}

export interface WorkspaceNotif {
  id: number;
  workspace_id: number | null;
  type: string;
  message: string;
  is_read: boolean;
  created_at: string;
}

export async function getWsNotifications(): Promise<WorkspaceNotif[]> {
  return request<WorkspaceNotif[]>(`${BASE}/api/notifications`);
}

export async function markWsNotificationsRead(): Promise<void> {
  await request(`${BASE}/api/notifications/read-all`, { method: "POST" });
}

export async function getWsUnreadCount(): Promise<number> {
  const res = await request<{ count: number }>(`${BASE}/api/notifications/unread-count`);
  return res.count;
}

export async function clearWsNotifications(): Promise<void> {
  await request(`${BASE}/api/notifications`, { method: "DELETE" });
}

export function formatNotifTime(ts: string): string {
  if (!ts || ts === 'now()') return 'Just now';
  try {
    const d = new Date(ts);
    if (isNaN(d.getTime())) return 'Just now';
    const diff = Date.now() - d.getTime();
    if (diff < 60_000) return 'Just now';
    if (diff < 3_600_000) return `${Math.floor(diff / 60_000)}m ago`;
    if (diff < 86_400_000) return `${Math.floor(diff / 3_600_000)}h ago`;
    return d.toLocaleDateString(undefined, { month: 'short', day: 'numeric' });
  } catch {
    return 'Just now';
  }
}

export interface UserProfile {
  id: number; login: string; displayName: string; email: string; avatar: string;
}

export async function lookupUser(login: string): Promise<UserProfile | null> {
  try {
    return await request<UserProfile>(`${BASE}/api/users/${encodeURIComponent(login)}`);
  } catch {
    return null;
  }
}

// ── Task Types & API ─────────────────────────────────────────────────────────

export interface ApiTask {
  id: number;
  title: string;
  status: string;
  priority: string;
  workspace_id: number;
  due_date: string | null;
  created_at: string;
}

export async function getTasks(workspace_id?: number): Promise<ApiTask[]> {
  const url = workspace_id
    ? `${BASE}/api/tasks?workspace_id=${workspace_id}`
    : `${BASE}/api/tasks`;
  return request<ApiTask[]>(url);
}

export async function createTask(data: {
  title: string;
  workspace_id: number;
  status?: string;
  priority?: string;
  due_date?: string;
}): Promise<ApiTask> {
  const res = await fetch(`${BASE}/api/tasks`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(data),
  });
  return res.json();
}

export async function updateTask(id: number, data: {
  title?: string;
  status?: string;
  priority?: string;
  due_date?: string;
}): Promise<ApiTask> {
  const res = await fetch(`${BASE}/api/tasks/${id}`, {
    method: "PATCH",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(data),
  });
  return res.json();
}

export async function deleteTask(id: number): Promise<void> {
  await fetch(`${BASE}/api/tasks/${id}`, { method: "DELETE" });
}