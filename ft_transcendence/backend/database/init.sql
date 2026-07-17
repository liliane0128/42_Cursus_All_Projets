// to use this: psql -U postgres -f database/init.sql

CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE IF NOT EXISTS workspaces (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    owner_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT fk_workspace_owner
        FOREIGN KEY (owner_id)
        REFERENCES users(id)
        ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS workspace_members (
    id SERIAL PRIMARY KEY,

    workspace_id INT NOT NULL,
    user_id INT NOT NULL,

    role VARCHAR(20) DEFAULT 'member',  -- owner / admin / member
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT fk_member_workspace
        FOREIGN KEY (workspace_id)
        REFERENCES workspaces(id)
        ON DELETE CASCADE,

    CONSTRAINT fk_member_user
        FOREIGN KEY (user_id)
        REFERENCES users(id)
        ON DELETE CASCADE,

    CONSTRAINT unique_workspace_user
        UNIQUE (workspace_id, user_id)
);


CREATE INDEX IF NOT EXISTS idx_workspace_owner ON workspaces(owner_id);
CREATE INDEX IF NOT EXISTS idx_member_workspace ON workspace_members(workspace_id);
CREATE INDEX IF NOT EXISTS idx_member_user ON workspace_members(user_id);