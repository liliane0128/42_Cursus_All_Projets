# DEV_DOC.md

## 1. Overview

This document explains how a developer can set up, build, and manage the Inception project using Docker and Docker Compose.

The project consists of three services:

* NGINX (HTTPS web server)
* WordPress (CMS)
* MariaDB (database)

---

## 2. Prerequisites

Before starting, ensure the following are installed:

* Docker
* Docker Compose
* Make
* A Linux-based system

---

## 3. Configuration Files and Secrets

### Environment file

The project uses a `.env` file located in the `srcs/` directory.

It contains sensitive configuration values such as:

* MYSQL_ROOT_PASSWORD
* MYSQL_USER
* MYSQL_PASSWORD
* MYSQL_DATABASE

⚠️ These values are used during container initialization and must be set before running the project.

---

## 4. Build and Launch the Project

### Using Makefile (recommended)

From the root directory:

```sh
make up
```

This command will:

* Create required directories
* Build all Docker images
* Start all containers in detached mode

---

### Using Docker Compose directly

```sh
cd srcs
docker compose up --build -d
```

---

## 5. Container Management Commands

### View running containers

```sh
docker compose ps
```

### Stop the project

```sh
make down
```

or

```sh
docker compose down
```

### Rebuild the project

```sh
make re
```

### Clean system resources

```sh
make clean
```

### Full cleanup (including volumes)

```sh
make fclean
```

---

## 6. Docker Volumes and Data Persistence

The project uses Docker volumes to ensure data persistence.

### Data storage locations:

* MariaDB data: `/home/lilwang/data/mariadb`
* WordPress files: `/home/lilwang/data/wordpress`

These directories are mounted as volumes inside containers.

---

## 7. How Data Persists

Even after restarting or rebooting the system:

* MariaDB data remains stored in the volume directory
* WordPress content and uploads are preserved

This ensures that no data is lost when containers are recreated.

---

## 8. Useful Developer Commands

### Enter a container

```sh
docker exec -it <container_name> bash
```

### Access database

```sh
mysql -u root -p
```

### View logs

```sh
docker logs <container_name>
```

---

## 9. Notes

* NGINX only exposes port 443 (HTTPS)
* MariaDB is not exposed externally for security reasons
* WordPress communicates with MariaDB through the Docker network
* All services are isolated using a custom Docker bridge network
