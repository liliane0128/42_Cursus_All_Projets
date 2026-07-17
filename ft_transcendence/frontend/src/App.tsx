import { Routes, Route, Navigate } from 'react-router-dom'
import { NotificationProvider } from './context/NotificationContext'
import { ChatContextProvider } from './context/ChatContext'
import { useAuth } from './context/AuthContext'
import { useState } from 'react'
import Dashboard from './pages/Dashboard'
import Tasks from './pages/Tasks'
import Login from './pages/Login'
import Chats from './pages/Chats'
import Workspace from './pages/Workspace/WorkspacePage'
import ForgotPassword from './pages/ForgotPassword'
import ResetPassword from './pages/ResetPassword'
import Profile from './pages/Profile'
import PrivacyPolicy from './pages/PrivacyPolicy'
import TermsOfService from './pages/TermsOfService'

// ── Route guard ───────────────────────────────────────────────────────────────

function PrivateRoute({ children }: { children: React.ReactNode }) {
  const { user, isLoading } = useAuth()

  if (isLoading) {
    return (
      <div className="min-h-screen bg-[#f5f5f3] flex items-center justify-center">
        <div className="w-5 h-5 border-2 border-gray-900 border-t-transparent rounded-full animate-spin" />
      </div>
    )
  }

  return user ? <>{children}</> : <Navigate to="/login" replace />
}

// ── App ───────────────────────────────────────────────────────────────────────

export default function App() {
  const { user } = useAuth()

  return (
    <NotificationProvider>
      <ChatContextProvider currentUserId={user ? String(user.id) : ""} currentUserName={user?.displayName ?? ""}>
        <Routes>
          <Route path="/login" element={user ? <Navigate to="/dashboard" replace /> : <Login />} />
          <Route path="/forgot-password" element={<ForgotPassword />} />
          <Route path="/privacy" element={<PrivacyPolicy />} />
          <Route path="/terms"   element={<TermsOfService />} />
          <Route path="/reset-password" element={<ResetPassword />} />
          <Route path="/profile" element={<PrivateRoute><Profile /></PrivateRoute>} />
          <Route path="/dashboard" element={<PrivateRoute><Dashboard /></PrivateRoute>} />
          <Route path="/tasks"     element={<PrivateRoute><Tasks /></PrivateRoute>} />
          <Route path="/chats"     element={<PrivateRoute><Chats /></PrivateRoute>} />
          <Route path="/workspace" element={<PrivateRoute><Workspace /></PrivateRoute>} />
          <Route path="*"          element={<Navigate to="/dashboard" replace />} />
          
        </Routes>
      </ChatContextProvider>
    </NotificationProvider>
  )
}