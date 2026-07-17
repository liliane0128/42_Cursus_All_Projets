
DC = docker compose --env-file backend/.env

up:
	$(DC) up --build

down:
	$(DC) down

stop:
	$(DC) stop

start:
	$(DC) start

restart: down up


clean:
	$(DC) down -v

fclean: clean
	docker system prune -af


rebuild:
	$(DC) build backend

logs:
	$(DC) logs -f

backend-logs:
	$(DC) logs -f backend

db-logs:
	$(DC) logs -f db