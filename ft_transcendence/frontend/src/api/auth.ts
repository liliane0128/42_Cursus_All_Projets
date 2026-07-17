import { User } from "../context/AuthContext";

const BASE = "/api";

function authHeaders(token: string) {
  return {
    "Content-Type": "application/json",
    Authorization: `Bearer ${token}`,
  };
}

// ── Auth ──────────────────────────────────────────────────────────────────────

export async function apiLogin(
  login: string,
  password: string
): Promise<{ token: string; user: User }> {
  const res = await fetch(`${BASE}/auth/login`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ login, password }),
  });
  if (!res.ok) {
    const err = await res.json().catch(() => ({}));
    throw new Error(err.message ?? "Login failed");
  }
  return res.json();
}

export async function apiRegister(
  login: string,
  email: string,
  password: string
): Promise<{ token: string; user: User }> {
  const res = await fetch(`${BASE}/auth/register`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ login, email, password }),
  });
  if (!res.ok) {
    const err = await res.json().catch(() => ({}));
    throw new Error(err.message ?? "Registration failed");
  }
  return res.json();
}

// Returns the URL to redirect the browser to for 42 OAuth
export function get42OAuthUrl(): string {
  return `${BASE}/auth/42`;
}

// Returns the URL for GitHub OAuth
export function getGithubOAuthUrl(): string {
  return `${BASE}/auth/github`;
}

// ── User / Profile ────────────────────────────────────────────────────────────

export async function apiGetMe(token: string): Promise<User> {
  const res = await fetch(`${BASE}/auth/me`, {
    headers: authHeaders(token),
  });
  if (!res.ok) throw new Error("Failed to fetch profile");
  return res.json();
}

export async function apiUpdateProfile(
  token: string,
  updates: Partial<Pick<User, "displayName" | "bio" | "status" | "email">>
): Promise<User> {
  const res = await fetch(`${BASE}/users/me`, {
    method: "PATCH",
    headers: authHeaders(token),
    body: JSON.stringify(updates),
  });
  if (!res.ok) throw new Error("Failed to update profile");
  return res.json();
}

export async function apiUploadAvatar(
  token: string,
  file: File
): Promise<{ avatarUrl: string }> {
  const form = new FormData();
  form.append("file", file);
  const res = await fetch(`${BASE}/users/me/avatar`, {
    method: "POST",
    headers: { Authorization: `Bearer ${token}` },
    body: form,
  });
  if (!res.ok) throw new Error("Failed to upload avatar");
  return res.json();
}

// ── GDPR ──────────────────────────────────────────────────────────────────────

export async function apiExportMyData(token: string): Promise<object> {
  const res = await fetch(`${BASE}/auth/me/export`, {
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${token}`,
    },
  });
  if (!res.ok) throw new Error("Failed to export data");
  return res.json();
}

export async function apiDeleteMyAccount(token: string): Promise<void> {
  const res = await fetch(`${BASE}/auth/me`, {
    method: "DELETE",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${token}`,
    },
  });
  if (!res.ok) throw new Error("Failed to delete account");
}
