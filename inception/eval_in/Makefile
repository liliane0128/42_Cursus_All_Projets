all:
	@mkdir -p /home/lilwang/data/mariadb
	@mkdir -p /home/lilwang/data/wordpress
	@docker compose -f srcs/docker-compose.yml up --build -d

up: all

down:
	@docker compose -f srcs/docker-compose.yml down

clean: down
	@docker system prune -af

fclean: clean
	@sudo rm -rf /home/lilwang/data/mariadb
	@sudo rm -rf /home/lilwang/data/wordpress
	@docker volume ls -q | xargs -r docker volume rm

re: fclean all

.PHONY: all up down clean fclean re
