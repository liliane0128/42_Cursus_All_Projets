import { useState, useRef, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { LogOut } from "lucide-react";
import { useAuth } from "../context/AuthContext";
import { mediaUrl } from "../api";

function statusDot(status: string) {
  if (status === "online")  return "bg-green-400";
  
  return "bg-gray-400";
}

export default function UserMenu() {
  const { user, logout } = useAuth();
  const navigate = useNavigate();
  const [open, setOpen] = useState(false);
  const ref = useRef<HTMLDivElement>(null);

  useEffect(() => {
    function handler(e: MouseEvent) {
      if (ref.current && !ref.current.contains(e.target as Node)) setOpen(false);
    }
    document.addEventListener("mousedown", handler);
    return () => document.removeEventListener("mousedown", handler);
  }, []);

  const initials = user?.displayName?.slice(0, 1).toUpperCase() ?? "?";
  const avatarUrl = mediaUrl(user?.avatar);

  return (
    <div ref={ref} className="relative ml-1">
      <button
        onClick={() => setOpen(o => !o)}
        className="relative w-[26px] h-[26px] flex-shrink-0">
        <div className="w-[26px] h-[26px] rounded-full overflow-hidden flex items-center justify-center text-[10px] font-bold text-white hover:opacity-80 transition-opacity">
          {avatarUrl ? (
            <img src={avatarUrl} alt="avatar" className="w-full h-full object-cover" />
          ) : (
            <div className="w-full h-full bg-gray-900 flex items-center justify-center">
              {initials}
            </div>
          )}
        </div>
        <span className={`absolute bottom-0 right-0 w-2 h-2 rounded-full border border-white ${statusDot(user?.status ?? "offline")}`} />
      </button>
      {open && (
        <div className="absolute right-0 top-full mt-2 w-48 bg-white border border-gray-100 rounded-xl shadow-lg z-50 overflow-hidden">
          <div className="px-4 py-3 border-b border-gray-100">
            <p className="text-[12px] font-semibold text-gray-900">{user?.displayName}</p>
            <p className="text-[10px] text-gray-400 mt-0.5">{user?.email}</p>
          </div>
          <div className="py-1">
            <button
              onClick={() => { setOpen(false); navigate("/profile"); }}
              className="w-full flex items-center gap-2.5 px-4 py-2 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
              Profile
            </button>
            <button
              onClick={() => { setOpen(false); logout(); navigate("/login"); }}
              className="w-full flex items-center gap-2.5 px-4 py-2 text-[12px] text-gray-600 hover:bg-gray-50 transition-colors">
              Log out <LogOut size={13} />
            </button>
          </div>
        </div>
      )}
    </div>
  );
}
