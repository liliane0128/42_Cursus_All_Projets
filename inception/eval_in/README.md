This project has been created as part of the 42 curriculum by lilwang

# Inception (42 Project)

## Description

This project is part of the 42 curriculum and aims to build a small infrastructure using Docker.  
The goal is to learn how to orchestrate multiple services using **Docker Compose**, while respecting strict constraints:

- Each service runs in a dedicated container
- No pre-built “ready-to-use” images from Docker Hub
- Services must communicate through a Docker network
- Data must persist using volumes
- The infrastructure must include:
  - NGINX (with TLS)
  - WordPress (PHP-FPM)
  - MariaDB (database)

The final result is a fully working WordPress website accessible via HTTPS, running inside a secure and isolated containerized environment.

---

## Project Overview

The architecture is composed of three main services:

### 1. NGINX
- Acts as a reverse proxy
- Handles HTTPS (TLS v1.2 / v1.3)
- Forwards PHP requests to WordPress container via FastCGI

### 2. WordPress (PHP-FPM)
- Runs PHP application logic
- Handles WordPress installation automatically via `wp-cli`
- Connects to MariaDB database
- Generates `wp-config.php` at runtime

### 3. MariaDB
- Stores WordPress data
- Initializes database, user, and privileges via custom script
- Uses persistent volume to store data

---

## Instructions

### 1. Clone the project

```bash
git clone <repo_url>
cd inception
```

### 2. Configure environment variables

Create a `.env` file in the `srcs/` directory:

```env
LOGIN=your_login
DOMAIN_NAME=your_domain

MYSQL_DATABASE=wordpress
MYSQL_USER=your_db_user
MYSQL_PASSWORD=your_db_password
MYSQL_ROOT_PASSWORD=your_root_password

WP_ADMIN_USER=your_admin_user
WP_ADMIN_PASSWORD=your_admin_password
WP_ADMIN_EMAIL=your_admin_email

WP_USER=your_user
WP_USER_EMAIL=your_email
WP_USER_PASSWORD=your_user_password

### 3. Build and run containers
```bash
make up
```

### 4. Access the website

Open browser:

https://lilwang.42.fr


### 5. Stop containers
make down


## AI Usage

Used for:

- Understanding Docker architecture and container orchestration concepts
- Clarifying the role and structure of Dockerfiles (build steps, base images, entrypoints)
- Understanding Docker Compose service communication (networking, dependencies, ports)
- Debugging Dockerfiles, entrypoint scripts, and docker-compose configuration issues
- Learning how NGINX, PHP-FPM, MariaDB, and WordPress interact in a multi-container setup
- Understanding TLS/SSL configuration for HTTPS with NGINX
- Resolving runtime issues such as volume mounting, permissions, and container startup order

## Documentation

https://www.devopssec.fr/category/apprendre-docker
https://www.ibm.com/fr-fr/think/topics/docker
https://dyma.fr/formations/docker?campaignId=21779107196&device=c&utm_source=google&gad_source=1&gad_campaignid=21779107196&gclid=CjwKCAjw5NvPBhAoEiwA_2egfixezJlVup9w8235_5onI-kFye9G33na-SWBM1vM8vpAIf2j3YnBcRoCVvkQAvD_BwE
https://docs.docker.com/
https://mariadb.com/kb/en/
https://wordpress.org/support/article/how-to-install-wordpress/
https://nginx.org/en/docs/
https://www.php.net/manual/en/
