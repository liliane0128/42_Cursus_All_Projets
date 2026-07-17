import Logo42 from "../components/Logo42";
import { useState, FormEvent } from "react";
import { useNavigate } from "react-router-dom";
import { Zap, AlertCircle, CheckCircle } from "lucide-react";

export default function ForgotPassword() {
  const navigate = useNavigate();
  const [email, setEmail]     = useState("");
  const [loading, setLoading] = useState(false);
  const [error, setError]     = useState("");
  const [sent, setSent]       = useState(false);

  async function handleSubmit(e: FormEvent) {
    e.preventDefault();
    setError("");
    setLoading(true);
    try {
      await fetch("/api/forgot-password", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ email }),
      });
      setSent(true);
    } catch {
      setError("Something went wrong. Please try again.");
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
            <h1 className="text-[15px] font-semibold text-gray-900">Forgot password</h1>
            <p className="text-[11px] text-gray-400 mt-0.5">
              Enter your email and we'll send you a reset link
            </p>
          </div>
          {sent ? (
            <div className="flex items-start gap-2 px-3 py-3 rounded-lg bg-green-50 border border-green-100">
              <CheckCircle size={13} className="text-green-500 flex-shrink-0 mt-0.5" />
              <p className="text-[12px] text-green-600">
                If this email is registered, you'll receive a reset link shortly.
              </p>
            </div>
          ) : (
            <form onSubmit={handleSubmit} className="space-y-3">
              <div>
                <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Email</label>
                <input
                  type="email"
                  value={email}
                  onChange={e => setEmail(e.target.value)}
                  placeholder="you@student.42.fr"
                  className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                    focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent
                    placeholder:text-gray-300 transition-all"
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
                disabled={loading || !email}
                className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                  hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors"
              >
                {loading ? "Sending…" : "Send reset link"}
              </button>
            </form>
          )}
        </div>
        <p className="text-center text-[11px] text-gray-400 mt-4">
          <button onClick={() => navigate("/login")} className="text-gray-900 font-medium hover:underline">
            Back to sign in
          </button>
        </p>
      </div>
    </div>
  );
}
