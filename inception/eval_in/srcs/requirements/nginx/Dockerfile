FROM debian:bookworm

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y nginx openssl && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir -p /etc/nginx/ssl

RUN openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /etc/nginx/ssl/nginx.key \
    -out /etc/nginx/ssl/nginx.crt \
    -subj "/C=FR/ST=Paris/L=Paris/O=42/OU=student/CN=lilwang.42.fr"

COPY conf/nginx.conf /etc/nginx/nginx.conf

COPY conf/default.conf /etc/nginx/conf.d/default.conf

RUN mkdir -p /run/nginx

RUN chmod 755 /var/www/html || true

EXPOSE 443

CMD ["nginx", "-g", "daemon off;"]