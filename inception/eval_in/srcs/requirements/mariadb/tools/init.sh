#!/bin/sh

# exit on error
set -e

mkdir -p /run/mysqld

# change owener -R: recursive
# Allow MySQL to have its own runtime environment and database files
chown -R mysql:mysql /run/mysqld /var/lib/mysql

# it initializes MySQL only the first time, when the database directory doesn’t exist.
if [ ! -d "/var/lib/mysql/mysql" ]; then
    mysql_install_db --user=mysql --datadir=/var/lib/mysql
fi

# initialize MySQL by creating the database, users, and permissions during first setup.
mysqld --user=mysql --datadir=/var/lib/mysql --bootstrap << EOF
USE mysql;
FLUSH PRIVILEGES;
CREATE DATABASE IF NOT EXISTS ${MYSQL_DATABASE};
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';
GRANT ALL PRIVILEGES ON ${MYSQL_DATABASE}.* TO '${MYSQL_USER}'@'%';
ALTER USER 'root'@'localhost' IDENTIFIED BY '${MYSQL_ROOT_PASSWORD}';
FLUSH PRIVILEGES;
EOF

# launche MySQL as the main process using the specified data directory.
exec mysqld --user=mysql --datadir=/var/lib/mysql