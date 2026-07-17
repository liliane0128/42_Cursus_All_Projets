import Logo42 from "../components/Logo42";
import { useState, FormEvent } from "react";
import { useNavigate, useSearchParams } from "react-router-dom";
import { Zap, Eye, EyeOff, AlertCircle, CheckCircle } from "lucide-react";
import { useAuth } from "../context/AuthContext";

export default function ResetPassword() {
  const navigate = useNavigate();
  const { logout } = useAuth();
  const [params] = useSearchParams();
  const token = params.get("token") ?? "";
  const [password, setPassword] = useState("");
  const [confirm, setConfirm]   = useState("");
  const [show, setShow]         = useState(false);
  const [loading, setLoading]   = useState(false);
  const [error, setError]       = useState("");
  const [success, setSuccess]   = useState(false);

  async function handleSubmit(e: FormEvent) {
    e.preventDefault();
    setError("");
    if (password !== confirm) { setError("Passwords do not match"); return; }
    if (password.length < 8)  { setError("Password must be at least 8 characters"); return; }
    setLoading(true);
    try {
      const res = await fetch("/api/reset-password", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ token, password }),
      });
      if (!res.ok) {
        const data = await res.json();
        setError(data.detail ?? "Invalid or expired link");
      } else {
        localStorage.removeItem("jwt_token");
        localStorage.removeItem("user");
        setSuccess(true);
      }
    } catch {
      setError("Something went wrong.");
    } finally {
      setLoading(false);
    }
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
            <h1 className="text-[15px] font-semibold text-gray-900">Reset password</h1>
            <p className="text-[11px] text-gray-400 mt-0.5">Enter your new password</p>
          </div>
          {success ? (
            <div className="space-y-3">
              <div className="flex items-start gap-2 px-3 py-3 rounded-lg bg-green-50 border border-green-100">
                <CheckCircle size={13} className="text-green-500 flex-shrink-0 mt-0.5" />
                <p className="text-[12px] text-green-600">Password reset successfully!</p>
              </div>
              <button
                onClick={() => { logout(); navigate("/login"); }}
                className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                  hover:bg-gray-700 transition-colors"
              >
                Back to Login
              </button>
            </div>
          ) : (
            <form onSubmit={handleSubmit} className="space-y-3">
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">New password</label>
                <div className="relative">
                  <input
                    type={show ? "text" : "password"}
                    value={password}
                    onChange={e => setPassword(e.target.value)}
                    placeholder="••••••••"
                    className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                      focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all"
                  />
                  <button type="button" onClick={() => setShow(s => !s)}
                    className="absolute right-2.5 top-1/2 -translate-y-1/2 text-gray-300 hover:text-gray-500">
                    {show ? <EyeOff size={13} /> : <Eye size={13} />}
                  </button>
                </div>
              </div>
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Confirm password</label>
                <input
                  type={show ? "text" : "password"}
                  value={confirm}
                  onChange={e => setConfirm(e.target.value)}
                  placeholder="••••••••"
                  className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                    focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all"
                />
              </div>
              {error && (
                <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-red-50 border border-red-100">
                  <AlertCircle size={12} className="text-red-400 flex-shrink-0" />
                  <p className="text-[11px] text-red-500">{error}</p>
                </div>
              )}
              <button
                type="submit"
                disabled={loading || !password || !confirm}
                className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                  hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors"
              >
                {loading ? "Resetting…" : "Reset password"}
              </button>
            </form>
          )}
        </div>
      </div>
    </div>
  );
}
