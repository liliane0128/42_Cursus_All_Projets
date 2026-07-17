import { createContext, useContext, useState, useEffect, ReactNode } from "react";

// ── Types ─────────────────────────────────────────────────────────────────────

export interface User {
  id: number;
  login: string;          // 42 login or username
  displayName: string;
  email: string;
  avatar: string | null;  // URL or null
  bio: string;
  status: "online" | "offline" | "in_game";
}

interface AuthContextType {
  user: User | null;
  token: string | null;
  isLoading: boolean;
  login: (token: string, user: User) => void;
  logout: () => void;
  updateUser: (updates: Partial<User>) => void;
}

// ── Context ───────────────────────────────────────────────────────────────────

const AuthContext = createContext<AuthContextType | null>(null);

export function AuthProvider({ children }: { children: ReactNode }) {
  const [user, setUser]       = useState<User | null>(null);
  const [token, setToken]     = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(true);

  // Restore session on mount
  useEffect(() => {
    const savedToken = localStorage.getItem("jwt_token");
    const savedUser  = localStorage.getItem("user");
    if (savedToken && savedUser) {
      try {
        setToken(savedToken);
        setUser(JSON.parse(savedUser));
      } catch {
        localStorage.removeItem("jwt_token");
        localStorage.removeItem("user");
      }
    }
    setIsLoading(false);
  }, []);

  // Handle 42 OAuth callback: ?token=xxx in URL
  useEffect(() => {
    const params = new URLSearchParams(window.location.search);
    const oauthToken = params.get("token");
    if (oauthToken) {
      // Fetch user info with this token
      fetch("/api/auth/me", {
        headers: { Authorization: `Bearer ${oauthToken}` },
      })
        .then(r => r.json())
        .then((u: User) => {
          login(oauthToken, u);
          // Clean URL
          window.history.replaceState({}, "", window.location.pathname);
        })
        .catch(() => {});
    }
  }, []);

  function login(newToken: string, newUser: User) {
    setToken(newToken);
    setUser({ ...newUser, status: "online" });
    localStorage.setItem("jwt_token", newToken);
    localStorage.setItem("user", JSON.stringify({ ...newUser, status: "online" }));
    fetch("/api/users/me", {
      method: "PATCH",
      headers: { "Content-Type": "application/json", Authorization: `Bearer ${newToken}` },
      body: JSON.stringify({ status: "online" }),
    });
  }

  function logout() {
    const token = localStorage.getItem("jwt_token");
    if (token) {
      fetch("/api/users/me", {
        method: "PATCH",
        headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
        body: JSON.stringify({ status: "offline" }),
      });
    }
    setToken(null);
    setUser(null);
    localStorage.removeItem("jwt_token");
    localStorage.removeItem("user");
  }

  function updateUser(updates: Partial<User>) {
    setUser(prev => {
      if (!prev) return null;
      const updated = { ...prev, ...updates };
      localStorage.setItem("user", JSON.stringify(updated));
      return updated;
    });
  }

  return (
    <AuthContext.Provider value={{ user, token, isLoading, login, logout, updateUser }}>
      {children}
    </AuthContext.Provider>
  );
}

export function useAuth() {
  const ctx = useContext(AuthContext);
  if (!ctx) throw new Error("useAuth must be used inside AuthProvider");
  return ctx;
}