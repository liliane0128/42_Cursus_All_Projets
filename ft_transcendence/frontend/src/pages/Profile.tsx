import Logo42 from "../components/Logo42";
import { useState, useRef, useEffect, FormEvent } from "react";
import { useNavigate } from "react-router-dom";
import { useAuth } from "../context/AuthContext";
import { apiUpdateProfile, apiUploadAvatar, apiExportMyData, apiDeleteMyAccount } from "../api/auth";
import { mediaUrl } from "../api";
import AvatarCropper from "../components/AvatarCropper";
import { Zap, ArrowLeft, Camera, AlertCircle, CheckCircle, Trash2, Download } from "lucide-react";

export default function Profile() {
  const navigate = useNavigate();
  const { user, token, updateUser, logout } = useAuth();

  const [displayName, setDisplayName] = useState(user?.displayName ?? "");
  const [email, setEmail]             = useState(user?.email ?? "");
  const [bio, setBio]                 = useState(user?.bio ?? "");
  const [saving, setSaving]           = useState(false);
  const [success, setSuccess]         = useState(false);
  const [error, setError]             = useState("");
  const [avatarLoading, setAvatarLoading] = useState(false);
  const [cropSrc, setCropSrc]         = useState<string | null>(null);
  const [deleteConfirm, setDeleteConfirm] = useState(false);
  const [deleteInput, setDeleteInput]     = useState("");
  const fileRef = useRef<HTMLInputElement>(null);

  const [exportLoading, setExportLoading] = useState(false);
  const [exportSuccess, setExportSuccess] = useState(false);

  const [twoFAEnabled, setTwoFAEnabled] = useState(false);
  const [showQR,       setShowQR]       = useState(false);
  const [qrCode,       setQrCode]       = useState("");
  const [twoFACode,    setTwoFACode]    = useState("");
  const [twoFAError,   setTwoFAError]   = useState("");

  useEffect(() => {
    if (!token) return;
    fetch("/api/users/me", { headers: { Authorization: `Bearer ${token}` } })
      .then(r => r.json())
      .then(data => { if (data.totp_enabled) setTwoFAEnabled(true); })
      .catch(() => {});
  }, [token]);

  async function setup2FA() {
    setTwoFAError("");
    try {
      const res = await fetch("/api/users/2fa/setup", {
        method: "POST",
        headers: { Authorization: `Bearer ${token}` },
      });
      const data = await res.json();
      setQrCode(data.qr_code);
      setShowQR(true);
      setTwoFACode("");
    } catch {
      setTwoFAError("Failed to set up 2FA");
    }
  }

  async function verify2FA() {
    setTwoFAError("");
    try {
      const res = await fetch("/api/users/2fa/verify", {
        method: "POST",
        headers: { "Content-Type": "application/json", Authorization: `Bearer ${token}` },
        body: JSON.stringify({ code: twoFACode }),
      });
      if (!res.ok) { setTwoFAError("Invalid code. Please try again."); return; }
      setTwoFAEnabled(true);
      setShowQR(false);
      setTwoFACode("");
    } catch {
      setTwoFAError("Something went wrong");
    }
  }

  async function disable2FA() {
    setTwoFAError("");
    try {
      await fetch("/api/users/2fa/disable", {
        method: "POST",
        headers: { Authorization: `Bearer ${token}` },
      });
      setTwoFAEnabled(false);
      setShowQR(false);
    } catch {
      setTwoFAError("Failed to disable 2FA");
    }
  }

  async function handleSave(e: FormEvent) {
    e.preventDefault();
    setError(""); setSuccess(false);
    setSaving(true);
    try {
      const updated = await apiUpdateProfile(token!, { displayName, bio, email });
      updateUser(updated);
      setSuccess(true);
      setTimeout(() => setSuccess(false), 3000);
    } catch (err: unknown) {
      setError(err instanceof Error ? err.message : "Failed to save");
    } finally {
      setSaving(false);
    }
  }

  async function handleAvatar(e: React.ChangeEvent<HTMLInputElement>) {
    const file = e.target.files?.[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onload = () => setCropSrc(reader.result as string);
    reader.readAsDataURL(file);
    e.target.value = "";
  }

  async function handleCrop(blob: Blob) {
    console.log("handleCrop called", blob);
    setCropSrc(null);
    setAvatarLoading(true);
    try {
      const file = new File([blob], "avatar.jpg", { type: "image/jpeg" });
      console.log("uploading...", token);
      const res = await apiUploadAvatar(token!, file);
      console.log("upload res:", res);
      console.log("updating user avatar:", res.avatarUrl);
      updateUser({ avatar: res.avatarUrl });
    } catch {
      setError("Failed to upload avatar");
    } finally {
      setAvatarLoading(false);
    }
  }

  async function handleExportData() {
    setExportLoading(true);
    setError("");
    try {
      const data = await apiExportMyData(token!);
      const blob = new Blob([JSON.stringify(data, null, 2)], { type: "application/json" });
      const url  = URL.createObjectURL(blob);
      const a    = document.createElement("a");
      a.href     = url;
      a.download = `collab42-data-${user?.login}.json`;
      a.click();
      URL.revokeObjectURL(url);
      setExportSuccess(true);
      setTimeout(() => setExportSuccess(false), 3000);
    } catch {
      setError("Failed to export data");
    } finally {
      setExportLoading(false);
    }
  }

  async function handleDelete() {
    if (deleteInput !== user?.login) {
      setError("Username doesn't match");
      return;
    }
    try {
      await apiDeleteMyAccount(token!);
      logout();
      navigate("/login");
    } catch {
      setError("Failed to delete account");
    }
  }

  const avatarUrl = mediaUrl(user?.avatar);

  if (cropSrc) return (
    <AvatarCropper
      imageSrc={cropSrc}
      onCancel={() => setCropSrc(null)}
      onCrop={handleCrop}
    />
  );

  return (
    <div className="min-h-screen bg-[#f5f5f3] font-sans">
      {/* Header */}
      <div className="bg-white border-b border-gray-100 px-6 py-4 flex items-center gap-3">
        <button onClick={() => navigate("/dashboard")}
          className="w-7 h-7 flex items-center justify-center rounded-lg text-gray-400 hover:text-gray-900 hover:bg-gray-100 transition-colors">
          <ArrowLeft size={14} />
        </button>
        <div className="flex items-center gap-2">
          <div className="w-[22px] h-[22px] rounded-[5px] bg-gray-900 flex items-center justify-center">
            <Logo42 size={10} />
          </div>
          <span className="text-[13px] font-semibold">Profile</span>
        </div>
      </div>

      <div className="max-w-lg mx-auto px-4 py-8 space-y-4">

        {/* Avatar */}
        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <h2 className="text-[13px] font-semibold mb-4">Avatar</h2>
          <div className="flex items-center gap-4">
            <div className="relative">
              {avatarUrl ? (
                <img src={avatarUrl} alt="avatar"
                  className="w-16 h-16 rounded-full object-cover border border-gray-100" />
              ) : (
                <div className="w-16 h-16 rounded-full bg-gray-900 flex items-center justify-center text-white text-xl font-bold">
                  {user?.displayName?.slice(0, 1).toUpperCase() ?? "?"}
                </div>
              )}
              <button onClick={() => fileRef.current?.click()}
                className="absolute -bottom-1 -right-1 w-6 h-6 rounded-full bg-white border border-gray-200
                  flex items-center justify-center text-gray-500 hover:bg-gray-50 transition-colors shadow-sm">
                <Camera size={11} />
              </button>
            </div>
            <div>
              <p className="text-[12px] font-medium text-gray-900">{user?.displayName}</p>
              <p className="text-[11px] text-gray-400 mt-0.5">@{user?.login}</p>
            </div>
            <input ref={fileRef} type="file" accept="image/*" className="hidden" onChange={handleAvatar} />
          </div>
        </div>

        {/* Profile info */}
        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <h2 className="text-[13px] font-semibold mb-4">Profile</h2>
          <form onSubmit={handleSave} className="space-y-3">
            <div>
              <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Username</label>
              <input value={user?.login ?? ""} disabled
                className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50 text-gray-400 cursor-not-allowed" />
              <p className="text-[10px] text-gray-400 mt-1">Username cannot be changed</p>
            </div>
            <div>
              <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Display Name</label>
              <input value={displayName} onChange={e => setDisplayName(e.target.value)}
                placeholder="Your display name"
                className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                  focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all" />
            </div>
            <div>
              <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Email</label>
              <input value={email} onChange={e => setEmail(e.target.value)}
                type="email" placeholder="your@email.com"
                className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                  focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all" />
            </div>
            <div>
              <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Bio</label>
              <textarea value={bio} onChange={e => setBio(e.target.value)}
                placeholder="Tell us about yourself…" rows={3}
                className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                  focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all resize-none" />
            </div>

            {error && (
              <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-red-50 border border-red-100">
                <AlertCircle size={12} className="text-red-400 flex-shrink-0" />
                <p className="text-[11px] text-red-500">{error}</p>
              </div>
            )}
            {success && (
              <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-green-50 border border-green-100">
                <CheckCircle size={12} className="text-green-500 flex-shrink-0" />
                <p className="text-[11px] text-green-600">Profile saved!</p>
              </div>
            )}
            <button type="submit" disabled={saving}
              className="w-full py-2 rounded-lg bg-gray-900 text-white text-[13px] font-medium
                hover:bg-gray-700 disabled:opacity-40 transition-colors">
              {saving ? "Saving…" : "Save changes"}
            </button>
          </form>
        </div>

        {/* Security */}
        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <h2 className="text-[13px] font-semibold mb-4">Security</h2>
          <div className="space-y-4">
            <div className="flex items-center justify-between">
              <div>
                <p className="text-[12px] font-medium text-gray-900">Two-Factor Authentication</p>
                <p className="text-[11px] text-gray-400 mt-0.5">Add an extra layer of security to your account.</p>
              </div>
              <span className={`text-[10px] font-semibold px-2 py-0.5 rounded-full ${twoFAEnabled ? "bg-green-50 text-green-600" : "bg-gray-100 text-gray-400"}`}>
                {twoFAEnabled ? "Enabled" : "Disabled"}
              </span>
            </div>

            {showQR && qrCode && (
              <div className="space-y-3 pt-2 border-t border-gray-100">
                <p className="text-[11px] text-gray-500">Scan this QR code with your authenticator app (Google Authenticator, Authy, etc.), then enter the 6-digit code to confirm.</p>
                <img src={qrCode} alt="2FA QR Code" className="w-40 h-40 rounded-lg border border-gray-100" />
                <div>
                  <label className="block text-[11px] font-medium text-gray-500 mb-1.5">Verification Code</label>
                  <input
                    type="text"
                    value={twoFACode}
                    onChange={e => setTwoFACode(e.target.value)}
                    placeholder="000000"
                    maxLength={6}
                    className="w-full px-3 py-2 text-[13px] border border-gray-200 rounded-lg bg-gray-50
                      focus:outline-none focus:ring-2 focus:ring-gray-900 placeholder:text-gray-300 transition-all tracking-widest"
                  />
                </div>
                <div className="flex gap-2">
                  <button onClick={() => { setShowQR(false); setTwoFACode(""); setTwoFAError(""); }}
                    className="flex-1 py-2 rounded-lg border border-gray-200 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
                    Cancel
                  </button>
                  <button onClick={verify2FA} disabled={twoFACode.length !== 6}
                    className="flex-1 py-2 rounded-lg bg-gray-900 text-white text-[12px] font-medium
                      hover:bg-gray-700 disabled:opacity-40 disabled:cursor-not-allowed transition-colors">
                    Verify & Enable
                  </button>
                </div>
              </div>
            )}

            {twoFAError && (
              <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-red-50 border border-red-100">
                <AlertCircle size={12} className="text-red-400 flex-shrink-0" />
                <p className="text-[11px] text-red-500">{twoFAError}</p>
              </div>
            )}

            {!showQR && !twoFAEnabled && (
              <button onClick={setup2FA}
                className="w-full py-2 rounded-lg border border-gray-200 text-[12px] text-gray-700 hover:bg-gray-50 transition-colors">
                Enable 2FA
              </button>
            )}
            {!showQR && twoFAEnabled && (
              <button onClick={disable2FA}
                className="w-full py-2 rounded-lg border border-red-200 text-[12px] text-red-500 hover:bg-red-50 transition-colors">
                Disable 2FA
              </button>
            )}
          </div>
        </div>

        {/* Privacy & Data */}
        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <h2 className="text-[13px] font-semibold mb-1">Privacy & Data</h2>
          <p className="text-[11px] text-gray-400 mb-4">You have the right to access and download your personal data.</p>
          <div className="space-y-3">
            <div className="flex items-start justify-between gap-4">
              <div>
                <p className="text-[12px] font-medium text-gray-900">Export your data</p>
                <p className="text-[11px] text-gray-400 mt-0.5">Download a copy of your account data and tasks as a JSON file. A confirmation email will be sent.</p>
              </div>
            </div>
            {exportSuccess && (
              <div className="flex items-center gap-2 px-3 py-2 rounded-lg bg-green-50 border border-green-100">
                <CheckCircle size={12} className="text-green-500 flex-shrink-0" />
                <p className="text-[11px] text-green-600">Data exported successfully!</p>
              </div>
            )}
            <button
              onClick={handleExportData}
              disabled={exportLoading}
              className="flex items-center gap-2 px-4 py-2 rounded-lg border border-gray-200 text-[12px] text-gray-700 hover:bg-gray-50 transition-colors disabled:opacity-40">
              <Download size={13} />
              {exportLoading ? "Exporting…" : "Export my data"}
            </button>
          </div>
        </div>

        {/* Account Management */}
        <div className="bg-white border border-gray-100 rounded-xl p-6">
          <h2 className="text-[13px] font-semibold mb-4">Account Management</h2>
          <div className="space-y-4">
            <button
              onClick={() => { logout(); navigate("/login"); }}
              className="w-full flex items-center gap-2.5 px-4 py-2 rounded-lg border border-gray-200 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
              Log out
            </button>
            <div className="border-t border-gray-100 pt-4">
              <p className="text-[11px] text-gray-400 mb-3">
                Deleting your account is permanent and cannot be undone. A confirmation email will be sent.
              </p>
              {!deleteConfirm ? (
                <button onClick={() => setDeleteConfirm(true)}
                  className="flex items-center gap-2 px-4 py-2 rounded-lg border border-red-200 text-[12px] text-red-500 hover:bg-red-50 transition-colors">
                  <Trash2 size={13} />
                  Delete account
                </button>
              ) : (
                <div className="space-y-3">
                  <p className="text-[12px] text-gray-600">
                    Type <span className="font-mono font-bold">{user?.login}</span> to confirm deletion:
                  </p>
                  <input value={deleteInput} onChange={e => setDeleteInput(e.target.value)}
                    placeholder={user?.login}
                    className="w-full px-3 py-2 text-[13px] border border-red-200 rounded-lg bg-red-50
                      focus:outline-none focus:ring-2 focus:ring-red-300 placeholder:text-gray-300 transition-all" />
                  <div className="flex gap-2">
                    <button onClick={() => { setDeleteConfirm(false); setDeleteInput(""); }}
                      className="flex-1 py-2 rounded-lg border border-gray-200 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
                      Cancel
                    </button>
                    <button onClick={handleDelete}
                      disabled={deleteInput !== user?.login}
                      className="flex-1 py-2 rounded-lg bg-red-500 text-white text-[12px] font-medium
                        hover:bg-red-600 disabled:opacity-40 disabled:cursor-not-allowed transition-colors">
                      Delete forever
                    </button>
                  </div>
                </div>
              )}
            </div>
          </div>
        </div>

      </div>
    </div>
  );
}