# USER_DOC.md

## 1. Overview

This project is a Docker-based infrastructure composed of three services:

* **NGINX**: HTTPS web server that handles incoming requests.
* **WordPress**: Content Management System (CMS) for the website.
* **MariaDB**: Database service used by WordPress.

All services are containerized and communicate through a dedicated Docker network.

---

## 2. Project Architecture

* NGINX exposes the website via **HTTPS (port 443 only)**.
* WordPress runs with PHP-FPM internally.
* MariaDB stores all WordPress data persistently using Docker volumes.

---

## 3. Requirements

* Docker
* Docker Compose
* A configured `.env` file containing credentials

---

## 4. How to Start the Project

From the root directory of the project:

```sh
make up
```

or

```sh
cd srcs
docker compose up -d
```

This will build and start all containers.

---

## 5. How to Stop the Project

```sh
make down
```

or

```sh
docker compose down
```

This stops and removes all running containers.

---

## 6. Access the Website

Once the project is running:

* Website URL:

```
https://lilwang.42.fr
```
* WordPress user login:

```
https://lilwang.42.fr/wp-login.php
```

* WordPress Admin Panel:

```
https://lilwang.42.fr/wp-admin
```

Login using the WordPress user defined in the `.env` file.

---

## 7. Check the SSL certificate

```
openssl s_client -connect lilwang.42.fr:443
```

---

## 8. Credentials Management

All credentials are stored in the `.env` file located at the root of the project.

It includes:

* MYSQL_ROOT_PASSWORD
* MYSQL_USER
* MYSQL_PASSWORD
* MYSQL_DATABASE

⚠️ These credentials are used during container initialization.

---

## 9. Check if Services are Running

To verify that all services are running correctly:

```sh
cd srcs
docker compose ps
```

You should see the following services:

* nginx
* wordpress
* mariadb


---

## 10. Verify Functionality

### Check website access:

```sh
curl -k https://lilwang.42.fr
```

### Check database:

```sh
docker exec -it mariadb bash
mysql -u root -p
SHOW DATABASES;
USE wordpress;
SHOW TABLES;
```

---

## 11. Notes

* Only **port 443 (HTTPS)** is exposed for NGINX.
* MariaDB is not exposed externally for security reasons.
* Data is persisted using Docker volumes, so it survives reboot.
