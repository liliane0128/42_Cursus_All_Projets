import Logo42 from "../components/Logo42";
import { useState, useEffect, FormEvent } from "react";
import { useNavigate } from "react-router-dom";
import { Zap, Eye, EyeOff, AlertCircle } from "lucide-react";
import { useAuth } from "../context/AuthContext";
import { apiLogin, apiRegister, get42OAuthUrl, getGithubOAuthUrl } from "../api/auth";


function IconGithub() {
  return (
    <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor">
      <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0 0 24 12c0-6.63-5.37-12-12-12z" />
    </svg>
  );
}

function Field({ label, type = "text", value, onChange, placeholder, autoComplete }: {
  label: string; type?: string; value: string;
  onChange: (v: string) => void; placeholder?: string; autoComplete?: string;
}) {
  const [show, setShow] = useState(false);
  const isPassword = type === "password";
  return (
    <div>
      <label className="block text-[11px] font-medium text-gray-500 mb-1.5">{label}</label>
      <div className="relative">
        <input
          type={isPassword && show ? "text" : type}
          value={value}
          onChange={e => onChange(e.target.value)}
          placeholder={placeholder}
          autoComplete={autoComplete}
          className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
            focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent
            placeholder:text-gray-300 transition-all"
        />
        {isPassword && (
          <button type="button" onClick={() => setShow(s => !s)}
            className="absolute right-2.5 top-1/2 -translate-y-1/2 text-gray-300 hover:text-gray-500 transition-colors">
            {show ? <EyeOff size={13} /> : <Eye size={13} />}
          </button>
        )}
      </div>
    </div>
  );
}

export default function Login() {
  const navigate        = useNavigate();
  const { login }       = useAuth();
  const [mode, setMode] = useState<"login" | "register">("login");
  const [error, setError]   = useState("");
  const [loading, setLoading] = useState(false);
  const [loginField, setLoginField] = useState("");
  const [email, setEmail]           = useState("");
  const [password, setPassword]     = useState("");
  const [confirmPass, setConfirmPass] = useState("");
  const [requires2FA, setRequires2FA] = useState(false);
  const [twoFAUserId, setTwoFAUserId] = useState<number | null>(null);
  const [twoFACode, setTwoFACode]     = useState("");
  const [twoFAError, setTwoFAError]   = useState("");
  const [agreedToTerms, setAgreedToTerms] = useState(false);

  useEffect(() => {
    const params = new URLSearchParams(window.location.search);
    if (params.get("requires_2fa") === "true") {
      const userId = params.get("user_id");
      if (userId) {
        setRequires2FA(true);
        setTwoFAUserId(Number(userId));
      }
    }
  }, []);

  function switchMode() {
    setMode(m => m === "login" ? "register" : "login");
    setError("");
    setPassword("");
    setConfirmPass("");
  }

  async function handleSubmit(e: FormEvent) {
    e.preventDefault();
    setError("");
    if (mode === "register" && password !== confirmPass) {
      setError("Passwords do not match");
      return;
    }
    if (password.length < 8) {
      setError("Password must be at least 8 characters");
      return;
    }
    setLoading(true);
    try {
      const result = mode === "login"
        ? await apiLogin(loginField, password)
        : await apiRegister(loginField, email, password);
      if (result.requires_2fa) {
        setRequires2FA(true);
        setTwoFAUserId(result.user_id);
        setLoading(false);
        return;
      }
      login(result.token, result.user);
      navigate("/dashboard");
    } catch (err: unknown) {
      setError(err instanceof Error ? err.message : "Something went wrong");
    } finally {
      setLoading(false);
    }
  }

  async function handle2FASubmit(e: FormEvent) {
    e.preventDefault();
    setTwoFAError("");
    setLoading(true);
    try {
      const res = await fetch("/api/auth/2fa/login", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ user_id: twoFAUserId, code: twoFACode }),
      });
      if (!res.ok) { setTwoFAError("Invalid code. Please try again."); return; }
      const result = await res.json();
      login(result.token, result.user);
      navigate("/dashboard");
    } catch {
      setTwoFAError("Something went wrong");
    } finally {
      setLoading(false);
    }
  }

  if (requires2FA) {
    return (
      <div className="min-h-screen bg-[#f5f5f3] flex items-center justify-center p-4 font-sans">
        <div className="w-full max-w-[340px]">
          <div className="flex items-center gap-2.5 mb-8">
            <div className="w-[26px] h-[26px] rounded-[6px] bg-gray-900 flex items-center justify-center flex-shrink-0">
              <Zap size={12} className="text-white" fill="white" />
            </div>
            <span className="text-[13px] font-semibold tracking-tight">42 Workspace</span>
          </div>
          <div className="bg-white border border-gray-100 rounded-xl p-6">
            <div className="mb-5">
              <h1 className="text-[15px] font-semibold text-gray-900">Two-Factor Authentication</h1>
              <p className="text-[11px] text-gray-400 mt-0.5">Enter the 6-digit code from your authenticator app.</p>
            </div>
            <form onSubmit={handle2FASubmit} className="space-y-3">
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Authentication Code</label>
                <input
                  type="text"
                  value={twoFACode}
                  onChange={e => setTwoFACode(e.target.value)}
                  placeholder="000000"
                  maxLength={6}
                  autoComplete="one-time-code"
                  className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                    focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent
                    placeholder:text-gray-300 transition-all tracking-widest"
                />
              </div>
              {twoFAError && (
                <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-red-50 border border-red-100">
                  <AlertCircle size={12} className="text-red-400 flex-shrink-0" />
                  <p className="text-[11px] text-red-500">{twoFAError}</p>
                </div>
              )}
              <button type="submit" disabled={loading || twoFACode.length !== 6}
                className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                  hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors mt-1">
                {loading ? "Verifying…" : "Verify"}
              </button>
            </form>
          </div>
          <p className="text-center text-[11px] text-gray-400 mt-4">
            <button onClick={() => setRequires2FA(false)} className="text-gray-900 font-medium hover:underline">
              Back to login
            </button>
          </p>
        </div>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-[#f5f5f3] flex items-center justify-center p-4 font-sans">
      <div className="w-full max-w-[340px]">
        <div className="flex items-center gap-2.5 mb-8">
          <div className="w-[26px] h-[26px] rounded-[6px] bg-gray-900 flex items-center justify-center flex-shrink-0">
            <Logo42 size={12} />
          </div>
          <span className="text-[13px] font-semibold tracking-tight">Collab42</span>
        </div>

        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <div className="mb-5">
            <h1 className="text-[15px] font-semibold text-gray-900">
              {mode === "login" ? "Welcome back" : "Create account"}
            </h1>
            <p className="text-[11px] text-gray-400 mt-0.5">
              {mode === "login" ? "Sign in to your workspace" : "Start your workspace journey"}
            </p>
          </div>

          <div className="grid grid-cols-2 gap-2 mb-4">
            <a href={get42OAuthUrl()}
              className="flex items-center justify-center gap-2 px-3 py-2 rounded-lg border border-gray-200
                bg-gray-50 hover:bg-gray-100 text-[12px] font-medium text-gray-700 transition-colors">
              <Logo42 size={16} color="#111827" />42 School
            </a>
            <a href={getGithubOAuthUrl()}
              className="flex items-center justify-center gap-2 px-3 py-2 rounded-lg border border-gray-200
                bg-gray-50 hover:bg-gray-100 text-[12px] font-medium text-gray-700 transition-colors">
              <IconGithub />GitHub
            </a>
          </div>

          <div className="flex items-center gap-2 mb-4">
            <div className="flex-1 h-px bg-gray-100" />
            <span className="text-[10px] text-gray-300 font-medium">or continue with</span>
            <div className="flex-1 h-px bg-gray-100" />
          </div>

          <form onSubmit={handleSubmit} className="space-y-3">
            <Field label="Login / Username" value={loginField} onChange={setLoginField}
              placeholder="mlorenzi" autoComplete="username" />
            {mode === "register" && (
              <Field label="Email" type="email" value={email} onChange={setEmail}
                placeholder="you@student.42.fr" autoComplete="email" required />
            )}
            <Field label="Password" type="password" value={password} onChange={setPassword}
              placeholder="••••••••" autoComplete={mode === "login" ? "current-password" : "new-password"} />
            {mode === "register" && (
              <Field label="Confirm Password" type="password" value={confirmPass} onChange={setConfirmPass}
                placeholder="••••••••" autoComplete="new-password" />
            )}
            {error && (
              <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-red-50 border border-red-100">
                <AlertCircle size={12} className="text-red-400 flex-shrink-0" />
                <p className="text-[11px] text-red-500">{error}</p>
              </div>
            )}
            {mode === "register" && (
              <label className="flex items-start gap-2 text-[11px] text-gray-500 cursor-pointer">
                <input
                  type="checkbox"
                  checked={agreedToTerms}
                  onChange={e => setAgreedToTerms(e.target.checked)}
                  className="mt-0.5 accent-indigo-500"
                />
                <span>
                  I agree to the{" "}
                  <a href="/terms" target="_blank" className="text-indigo-500 hover:underline">
                    Terms of Service
                  </a>{" "}
                  and{" "}
                  <a href="/privacy" target="_blank" className="text-indigo-500 hover:underline">
                    Privacy Policy
                  </a>
                </span>
              </label>
            )}
            <button type="submit" disabled={mode === "register" && !agreedToTerms ? true : loading || !loginField || !password || (mode === "register" && !email)}
              className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors mt-1">
              {loading ? "Please wait…" : mode === "login" ? "Sign in" : "Create account"}
            </button>
          </form>
        </div>

        <p className="text-center text-[11px] text-gray-400 mt-4">
          {mode === "login" ? "Don't have an account?" : "Already have an account?"}{" "}
          <button onClick={switchMode} className="text-gray-900 font-medium hover:underline transition-all">
            {mode === "login" ? "Sign up" : "Sign in"}
          </button>
        </p>
        {mode === "login" && (
          <p className="text-center text-[11px] text-gray-400 mt-2">
            <button onClick={() => navigate("/forgot-password")}
              className="text-gray-900 font-medium hover:underline">
              Forgot password?
            </button>
          </p>
        )}
      </div>
    </div>
  );
}
