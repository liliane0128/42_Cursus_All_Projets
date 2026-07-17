FROM node:20

WORKDIR /app

COPY package.json package-lock.json* ./

RUN npm install

COPY . .

ENV VITE_API_TARGET=http://transcendence_backend:8080

EXPOSE 5173

CMD ["npm", "run", "dev", "--", "--host"]