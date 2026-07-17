# 42 Cursus – Core Projects

Ce dépôt regroupe l'ensemble des projets réalisés dans le cadre du **Common Core** du cursus 42. Chaque dossier correspond à un projet indépendant, avec ses propres consignes, contraintes et objectifs pédagogiques.

## Sommaire des projets

### Libft
Bibliothèque personnelle en C recodant des fonctions standard (`strlen`, `memcpy`, `atoi`, etc.) ainsi que des fonctions additionnelles (listes chaînées). Sert de base réutilisable pour tous les projets suivants.

### get_next_line
Fonction lisant une ligne à la fois depuis un descripteur de fichier, quel que soit le contenu ou la taille du buffer. Introduit la gestion des buffers statiques et des fuites mémoire.

### printf (ft_printf)
Réimplémentation de la fonction `printf` en gérant les principaux formats (`%s`, `%d`, `%x`, `%p`, etc.) via les fonctions variadiques (`va_list`).

### pipex
Simulation du comportement d'un pipe shell (`cmd1 | cmd2`) en C, en manipulant `fork`, `pipe`, `dup2` et `execve`.

### Philosophers
Simulation du problème des philosophes dînants. Introduit la programmation concurrente avec threads/mutex (ou processus), la gestion des race conditions et des deadlocks, ainsi que la précision du timing.

### minishell
Développement d'un mini shell Unix capable d'exécuter des commandes, gérer les pipes, redirections, variables d'environnement, quotes et signaux — une réplique simplifiée de bash.

### push_swap
Algorithme de tri optimisé utilisant deux piles (stack A et B) et un nombre limité d'instructions. L'objectif est de trier avec le moins d'opérations possible.

### fractol
Projet graphique affichant des fractales (Mandelbrot, Julia, etc.) avec la MinilibX, incluant zoom et déplacement interactifs.

### cub3D
Mini moteur de raycasting inspiré de Wolfenstein 3D : génération d'un rendu pseudo-3D à partir d'une carte 2D, gestion des textures, collisions et mouvements du joueur.

### CPPs (CPP Modules 00 → 09)
Suite de modules progressifs introduisant le C++ et la programmation orientée objet :
- **CPP00** : Classes, membres, fonctions membres, initialisation.
- **CPP01** : Mémoire (stack/heap), références, pointeurs.
- **CPP02** : Surcharge d'opérateurs (classe Fixed-point).
- **CPP03** : Héritage.
- **CPP04** : Polymorphisme, classes abstraites, interfaces.
- **CPP05** : Gestion des exceptions.
- **CPP06** : Cast (conversion de types) en C++.
- **CPP07** : Templates.
- **CPP08** : Containers de la STL (vector, map, etc.) et algorithmes.
- **CPP09** : Containers avancés et algorithmes complexes appliqués à un cas concret (ex. tri de données).

### irc
Implémentation d'un serveur IRC en C++ conforme à la RFC, gérant les connexions clients multiples via `poll()` (ou équivalent), l'authentification, les canaux, et les commandes de base du protocole IRC.

### ft_transcendence — Collab42
Projet final du tronc commun, réalisé en équipe de 4. Plutôt qu'un simple jeu de Pong, nous avons choisi de construire **Collab42**, une plateforme de collaboration en temps réel pour étudiants 42 : chat d'équipe, gestion de tâches et organisation en workspaces, avec un assistant IA intégré (Meowlinette).

- **Frontend** : React + TypeScript + Vite + Tailwind CSS
- **Backend** : FastAPI (Python) + PostgreSQL/SQLAlchemy, service de chat WebSocket dédié (Node.js/TypeScript)
- **Auth** : JWT, OAuth (42 Intra & GitHub), 2FA (TOTP)
- **IA** : LangChain + Groq (Llama 3.3/3.1) pour l'assistant et la modération de contenu
- **Infra** : Docker Compose, Nginx (reverse proxy + TLS), monitoring Prometheus/Grafana, PWA installable

27 points de modules validés (9 majeurs + 9 mineurs) pour un minimum requis de 14. Détails complets (architecture, schéma de base de données, répartition des tâches par membre, modules choisis) dans le [README du projet](ft_transcendence/README.md).

---

Chaque projet est évalué via des soutenances entre pairs (peer-evaluation) et doit respecter la norme de codage 42 (Norminette) ainsi que des contraintes strictes de gestion mémoire et de robustesse.
