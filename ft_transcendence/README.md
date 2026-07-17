*This project has been created as part of the 42 curriculum by [hguo], [lilwang], [lcao], [rxue].*

# Collab42 (ft_transcendence)

## Description

Collab42 is a real-time collaboration platform built for 42 students,
combining team chat, task management, and workspace organization with an
AI assistant (Meowlinette) built in. The goal of this project is to design
and implement a full-stack web application meeting the requirements of the
ft_transcendence common-core subject: a working frontend, backend, and
database, deployed via a containerized single-command setup, with a set of
chosen modules (see [Modules](#modules) below).

What makes it distinctive is the tight integration between its three pillars:
structured workspaces with Kanban task tracking, live multi-channel chat with
@mentions and image sharing, and an AI assistant reachable directly group 
conversations and its dedicated AI channel.

---

## Instructions

### Requirements

- Docker and Docker Compose

### Setup & Run

```bash
git clone <repo-url>
cd ft_transcendence
cp backend/.env.example backend/.env   # fill in required values
make up
```

The application will be available at `https://localhost:8443`.  
Grafana monitoring is at `http://localhost:3000`.

### Required `.env` variables

Copy `backend/.env.example` to `backend/.env` and fill in:

| Variable | Description |
|----------|-------------|
| `POSTGRES_USER` | PostgreSQL username |
| `POSTGRES_PASSWORD` | PostgreSQL password |
| `POSTGRES_DB` | PostgreSQL database name |
| `GF_SECURITY_ADMIN_USER` | Grafana admin username |
| `GF_SECURITY_ADMIN_PASSWORD` | Grafana admin password |
| `JWT_SECRET` | Secret key for signing JWT tokens (any long random string) |
| `GITHUB_CLIENT_ID` | GitHub OAuth App client ID |
| `GITHUB_CLIENT_SECRET` | GitHub OAuth App client secret |
| `GITHUB_REDIRECT_URI` | Must match the callback URL registered in the GitHub OAuth App |
| `FT_CLIENT_ID` | 42 OAuth App client ID |
| `FT_CLIENT_SECRET` | 42 OAuth App client secret |
| `FT_REDIRECT_URI` | Must match the callback URL registered in the 42 OAuth App |
| `FRONTEND_URL` | Base URL the backend redirects to after OAuth login |
| `GROQ_API_KEY` | Groq API key for the AI assistant (Meowlinette) |
| `GMAIL_USER` | Gmail address used for password reset emails |
| `GMAIL_PASSWORD` | Gmail App Password (not the account password) |


## Resources

### References

- FastAPI documentation — https://fastapi.tiangolo.com/
- React documentation — https://react.dev/
- LangChain documentation — https://python.langchain.com/
- Groq API documentation — https://console.groq.com/docs
- Tailwind CSS documentation — https://tailwindcss.com/docs
- SQLAlchemy documentation — https://docs.sqlalchemy.org/
- Nginx documentation — https://nginx.org/en/docs/
- Docker Compose documentation — https://docs.docker.com/compose/
- Prometheus documentation — https://prometheus.io/docs/
- Grafana documentation — https://grafana.com/docs/

### AI Usage


- Concept explanation & learning support
AI was used as a learning assistant to explain unfamiliar concepts and technologies encountered during development, including FastAPI, React, routing, REST APIs, authentication, database relationships, and frontend-backend communication. It helped us understand framework principles, read documentation more efficiently, and explore different technical approaches before implementation.

- Debugging & problem solving
AI was used as a support tool when the team encountered issues that were difficult to identify or understand. It helped us analyze error messages, explore possible causes, and suggest debugging directions. The team remained responsible for investigating, testing solutions, and validating the final fixes.

---

## Team

> One person may hold multiple roles.

| Name                | Role |
|---------------------|------|
| Huilin Guo (hguo)   | PO & Tech Lead & Developer — Chat, AI, Security & Infrastructure |
| Lili Wang (lilwang) | Project Manager & Full-Stack Developer — Workspace Management, Infrastructure & Deployment|
| Ruixian Xue (rxue)  | Full-Stack Developer — Dashboard, Task Management & GDPR Compliance |
| Luxuan Cao (lcao)   | Full-Stack Developer — User Authentication, OAuth Integration, Profile Management & UI/UX |

---

## Repository

[https://github.com/liliane0128/ft_transcendence](https://github.com/liliane0128/ft_transcendence)

---

## Project Management

Work was coordinated via **Discord**, with each team member working on a
dedicated Git branch (`hguo`, `lcao`, `lilwang`, `rxue`). Features were
split by domain: each member owned their modules end-to-end (frontend +
backend). Pull requests were reviewed and merged into `master` by the team,
with conflicts resolved collaboratively.

The team met every **Friday afternoon** to sync on progress, discuss
blockers, and align on priorities. Day-to-day communication and async
updates happened on Discord. Feature priorities and scope were decided
collectively as a team.

---

## Tech Stack

| Layer | Technology | Why |
|-------|-----------|-----|
| Frontend | React + TypeScript + Vite + Tailwind CSS | Component-based architecture with strong typing; Vite for fast HMR during development; Tailwind for rapid utility-first styling |
| Backend | FastAPI (Python) | High performance async framework; automatic API documentation; straightforward to learn and iterate on |
| Database | PostgreSQL + SQLAlchemy | Reliable relational storage with ACID guarantees; SQLAlchemy ORM keeps queries type-safe and prevents SQL injection |
| Real-time | WebSocket (custom Node.js/TypeScript chat service) | Enables instant messaging, presence, typing indicators, and live notifications |
| AI | LangChain + Groq (Llama 3.3 / 3.1) | Powers the AI assistant (Meowlinette) with tool calling and streaming |
| Auth | JWT + 42 OAuth + GitHub OAuth + TOTP 2FA | Supports multiple login paths (password, 42 Intra, GitHub) for maximum accessibility; JWT keeps the backend stateless; OAuth reduces friction for 42 students already authenticated with their school account |
| Reverse Proxy | Nginx | Single entry point for all traffic: SSL termination, HTTP→HTTPS redirect, and WebSocket (`wss://`) proxying for chat, API, and file uploads |
| Monitoring | Prometheus + Grafana | Industry-standard open-source stack; easy Docker integration; provides real-time metrics and custom dashboards |
| Containerization | Docker / Docker Compose | One-command setup (`make up`) for all services; isolated environments ensure reproducibility across machines and teammates |

---

## Architecture

```
┌──────────┐      ┌──────────────┐      ┌──────────────┐
│ Browser  │ ───▶ │    Nginx     │ ───▶ │   Backend    │
│          │ ◀─── │ (SSL / proxy)│ ◀─── │  (FastAPI)   │
└──────────┘      └──────┬───────┘      └──────┬───────┘
   │                      │
                          │ /ws                  ▼
                          ▼               ┌─────────────┐
                   ┌──────────────┐       │  PostgreSQL │
                   │ Chat Service │       └─────────────┘
                   │  (Node WS)   │
                   └──────────────┘

                   ┌──────────────┐      ┌─────────────┐
                   │  Prometheus  │ ───▶ │   Grafana   │
                   │  (metrics)   │      │ (dashboards)│
                   └──────────────┘      └─────────────┘
```

---

## Database Schema

- `users` — id, login, email, display_name, password (hashed), avatar, bio,
  status, ft_id, github_id, totp_secret, totp_enabled, is_deleted
- `friendships` — requester_id, addressee_id, status (pending/accepted)
- `dm_channels` — user1_id, user2_id
- `messages` — id, chat_id, sender_id, sender_name, content, type, created_at, edited
- `workspaces` — id, name, description, color, deadline, is_completed, created_at
- `workspace_members` — id, workspace_id, user_id, role (owner/admin/member)
- `workspace_invites` — id, workspace_id, inviter_id, invitee_id, role, status (pending/accepted/rejected), created_at
- `workspace_notifications` — id, user_id, workspace_id, type, message, is_read, created_at
- `workspace_files` — id, workspace_id, uploader_id, name, size_str, file_path, created_at
- `tasks` — id, title, status (todo/in progress/done), priority, workspace_id, due_date, created_at
- `links` — per-channel shared links
- `reset_tokens` — password reset tokens
- 
---

## Features

| Feature | Description | Implemented by |
|---------|-------------|----------------|
| Real-time chat | WebSocket-based DMs and group channels with presence, typing indicators, message editing/deletion, image upload | hguo |
| AI assistant | LangChain + Groq agent with streaming responses, tool calling, @mention summon | hguo |
| Friend & account system | Friend requests, DMs, 2FA, password reset, account deactivation (soft delete) | hguo |
| Notification center | Unified panel for chat messages, friend requests, time-sorted | hguo |
| Content moderation | Banned-word filter with word-boundary matching on frontend and backend | hguo |
| Monitoring | Prometheus metrics endpoint + Grafana dashboards with custom panels and alert rules | hguo |
| PWA | Installable progressive web app with offline support via service worker | hguo |
| Workspace management | Create/manage/delete workspaces; member invitation, role-based access (owner/member), ownership transfer, leave-workspace guard | lilwang |
| Shared files | File upload and listing scoped per workspace, served via nginx `/uploads/` proxy | lilwang |
| HTTPS & infrastructure | Nginx reverse proxy with self-signed TLS, HTTP→HTTPS redirect, WebSocket proxying; Docker Compose orchestration for all services | lilwang |
| Dashboard | Stat cards with hover popovers (workspaces, tasks, team members), deadline-based progress tracking | rxue |
| Task management | Kanban board with todo/in-progress/done columns, task creation, editing, assignment, priority levels, deadline tracking, drag-and-drop reordering | rxue |
| GDPR compliance | Data export & account deletion endpoints, email comfirmation | rxue |
| Authentication | JWT-based login and registration with secure token handling | lcao |
| OAuth integration | 42 School and GitHub OAuth login flow with token-based session management | lcao |
| Avatar upload | User profile picture upload with server-side storage and live preview | lcao |
| Frontend routing | React Router-based SPA navigation with protected routes and auth guards | lcao |
| Frontend UI | Responsive login/register page design and overall layout styling | lcao |
| API integration | Frontend-backend wiring with Vite proxy, error handling, and typed API calls | lcao |
| Database schema | SQLite user table design with OAuth ID fields, avatar, and status support | lcao |

---

## Modules

### Major modules (2 pts each)

| Module | Status | Points | Justification |
|--------|--------|--------|---------------|
| Framework (frontend + backend) | ✅ | 2 | Structured, maintainable codebase with React (component-based UI) and FastAPI (auto docs, async) |
| Real-time features (WebSocket) | ✅ | 2 | Core requirement for a chat platform; enables instant messaging and live presence without polling |
| User interaction | ✅ | 2 | Social foundation of the app — friends, DMs, and profiles make collaboration meaningful |
| Public API | ✅ | 2 | Exposes platform data securely with authentication, rate limiting, and 5+ documented endpoints |
| Standard user management & authentication | ✅ | 2 | Secure access control with multiple auth methods (OAuth, 2FA) for a production-grade experience |
| Organization system | ✅ | 2 | Enables structured team collaboration through workspaces with role-based permissions |
| LLM system interface | ✅ | 2 | Adds AI-powered assistance directly in the workspace via @mention, enhancing productivity |
| Monitoring system | ✅ | 2 | Provides real-time visibility into app health and performance with Prometheus + Grafana |
| Backend as microservices | ✅ | 2 | Separates concerns between API (FastAPI) and real-time (WebSocket service) for scalability |

**Total Major: 9 × 2 = 18 pts**

### Minor modules (1 pt each)

| Module | Status | Points | Justification |
|--------|--------|--------|---------------|
| ORM for database | ✅ | 1 | SQLAlchemy reduces boilerplate and prevents SQL injection through parameterized queries |
| Notification system | ✅ | 1 | Keeps users informed of all activity (messages, requests, workspace events) in one unified panel |
| Real-time collaborative features | ✅ | 1 | Live presence and shared workspaces enable genuine team collaboration |
| PWA | ✅ | 1 | Makes the app installable and usable offline, improving accessibility and mobile experience |
| OAuth 2.0 | ✅ | 1 | Allows login via existing 42/GitHub accounts, reducing friction for new users |
| 2FA | ✅ | 1 | Adds a critical security layer to protect user accounts from unauthorized access |
| Content moderation AI | ✅ | 1 | Automatically filters inappropriate content to maintain a safe and respectful environment |
| Sentiment analysis | ✅ | 1 | AI-based emotion detection adds context-aware responses, enhancing communication quality |
| GDPR compliance | ✅ | 1 | Ensures user data rights (export, deletion) in compliance with European privacy regulations |

**Total Minor: 9 × 1 = 9 pts**

**Grand total: 27 pts / minimum 14**

---

## Individual Contributions

### Huilin Guo (hguo)

- **Real-time chat system**: DM and group channels, message editing/deletion,
  image upload, emoji picker, @mentions, typing indicators, message search.
- **AI assistant Meowlinette**: LangChain + Groq integration, streaming
  responses, tool calling, @mention summon.
- **Friend & account system**: friend requests, 2FA (TOTP), email-based
  password reset, soft-delete account deactivation with consistent UI handling
  across friends list, DMs, and group messages.
- **Notification system**: unified `NotificationPanel` across all pages,
  supporting chat messages, friend requests (inline accept/ignore), and
  workspace events, time-sorted.
- **Content moderation**: banned-word filter with word-boundary regex,
  synchronized across frontend and backend.
- **Monitoring**: Prometheus metrics endpoint + Grafana dashboards.
- **PWA**: service worker setup, installability.
- **Code quality & architecture**: modular component design, shared `Avatar` 
  and `NotificationPanel` components, large-scale refactoring for maintainability.

### Lili Wang (lilwang)

- **Workspace system**: full workspace lifecycle — create, manage, and delete
  workspaces; member invitation with role-based access (owner / admin /member),
  ownership transfer, and leave-workspace guard when the user is the sole member.
- **Workspace Task management**: Kanban board with todo / in-progress / done columns;
  task creation, editing, assignment, priority levels, deadline tracking, and
  drag-and-drop reordering within and across columns.
- **Shared files**: file upload and in-workspace file listing, served through
  a dedicated `/uploads/` proxy route.
- **Workspace dashboard**: overview panel aggregating member avatars, task
  progress, and upcoming deadlines at a glance.
- **Workspace notifications**: backend notification model with frontend polling;
  per-user `localStorage` cache keyed by user ID to prevent cross-user
  data leakage on shared devices.
- **Infrastructure & HTTPS**: Docker Compose orchestration for all services;
  nginx reverse proxy with auto-generated self-signed TLS, HTTP → HTTPS
  redirect, and `wss://` WebSocket proxying — enabling teammates on the same
  network to share a single backend without any hardcoded `localhost` URLs.

### Luxuan Cao (lcao)

- **User authentication system**: registration and login with JWT token management, secure password hashing with bcrypt, email format validation on both frontend and backend.
- **OAuth 2.0**: GitHub and 42 Intra OAuth login flows, including automatic account creation on first login and token-based session redirect.
- **Profile page**: editable display name, email, and bio; avatar upload with in-browser cropping (react-easy-crop); online/offline status dot on all page headers.
- **Account management**: logout from any page via unified UserMenu component; account deletion with username confirmation and full database cleanup.
- **UserMenu component**: shared header component across Dashboard, Tasks, Chats, and Workspace pages, displaying user avatar with live status indicator.
- **UI/UX consistency**: Collab42 rebrand (replacing legacy "42 Workspace" naming), Logo42 SVG component using official 42 logo across all pages, unified color and icon system.
- **Form validation**: client-side and server-side email format validation using regex on registration and profile update.

### Ruixian Xue (rxue)

- **Dashboard stat cards**: hover popovers for active workspaces, open
  tasks, completed tasks, and team members — the latter deduplicated across
  every workspace via polling (rather than the less-reliable WebSocket
  presence list), excluding the current user and showing live profile
  avatars.
- **Workspace overview & My Tasks panels**: deadline-based progress bars per
  workspace, completed workspaces sorted to the bottom, and a quick-glance
  list of open tasks with one-click navigation into Tasks or Workspace.
- **Create Workspace flow**: "Create Workspace" on the Dashboard opens the
  create-workspace modal directly (instead of redirecting into whatever
  workspace was last viewed), landing the user inside the newly created
  workspace once saved.
- **Kanban task board**: dedicated `/tasks` page with To Do / In Progress /
  In Review / Completed columns, horizontal scrolling, and drag-and-drop
  status updates between columns.
- **Task detail, creation & filtering**: slide-in side panel for editing
  title/status/priority/due date, Add Task modal with duplicate-name
  validation, and live search + priority filtering across the board.
- **GDPR compliance**: data export (`/auth/me/export`) and account deletion
  (`/auth/me`) endpoints, each triggering a confirmation email — deletion
  soft-deletes the user and transfers or cleans up workspace memberships as
  needed.
