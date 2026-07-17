/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:21:49 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 14:50:51 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include "../libft/libft.h"

# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126
# define ERR_MSG "syntax error near unexpected token "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

extern volatile sig_atomic_t	g_last_code;

typedef enum e_quote_status
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_quote_status;

typedef enum e_token_type
{
	CMD_SIMPLE,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}		t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_status	status;
	struct s_token	*next;
}	t_token;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
	int				fd;
	int				heredoc_fd;
	t_quote_status	heredoc_quote;
}	t_redir;

typedef struct s_cmd
{
	char				**av;
	int					pipe_fd[2];
	struct s_cmd		*next;
	struct s_cmd		*prev;
	struct s_cmd		*head;
	t_token				*toklist;
	t_token_type		type;
	t_redir				*redirs;
}				t_cmd;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*subshell;
}	t_ast;

typedef struct s_data
{
	char	**envp;
	t_cmd	*cmd;
	pid_t	pid;
	int		last_status;
	t_ast	*ast;
	int		saved_stdin;
	int		saved_stdout;
}				t_data;

// builtin
char			*get_env_value(char **envp, const char *key);
int				cd_builtin(t_data *data, char **args);
int				echo_builtin(t_data *data, char **args);
int				env_builtin(t_data *data, char **args);
int				export_builtin(t_data *data, char **args);
int				pwd_builtin(t_data *data, char **args);
int				unset_builtin(t_data *data, char **args);
int				exit_builtin(t_data *data, char **args);

// env utils
char			**ft_envp_dup(char **envp);
char			**ft_sort_env(char **envp);
char			**ft_realloc_env(char **envp, const char *new_var);
bool			is_valid_identifier(const char *s);	
int				error_msg(const char *cmd, const char *arg, \
			const char *msg, int status);

// exec
bool			handle_redirs(t_cmd *cmd);
int				exec_ast(t_ast *node, t_data *data);
int				exec_pipe_ast(t_ast *node, t_data *data);
int				exec_builtin_in_child(t_cmd *cmd, t_data *data);
int				exec_cmd_node(t_ast *node, t_data *data);
int				exec_pipe_ast(t_ast *node, t_data *data);
bool			save_stdio(int *saved_stdin, int *saved_stdout);
bool			cmd_is_dir(char *cmd);
void			restore_stdio(int saved_stdin, int saved_stdout);
char			*find_command_path(char *cmd, char **envp);
void			close_redirs(t_redir *redirs);
void			handle_redirections_only(t_token *toklist, t_data *data);
bool			cmd_contains_slash(char *cmd);

//expander
void			expand_tokens(t_data *data, t_token *toklist);
char			*expand_variable_value(t_token *tok, int *i, t_data *data);
char			*expand_heredoc(t_data *data, char *line);
bool			is_var_end(char c);
char			*get_var_name(char *str);
void			update_quote_status(t_token *token, char c);
bool			has_wildcard(char *s);
void			expand_wildcards(t_token **tokens);
bool			is_valid_identifier_char(char c);
char			*expand_token_value(t_token *tok, t_data *data);
char			*str_append_str(char *s1, char *s2);
char			*str_append_char(char *s, char c);

// parsing
void			remove_empty_tokens(t_token **head);
void			free_redirs(t_redir *redir);
t_ast			*build_ast(t_token *tokens, t_data *data);
t_ast			*new_ast_node(t_node_type type, t_cmd *cmd);
void			free_ast(t_ast *node);
bool			is_cmd_end(t_token *token);
bool			parse_redirections(t_cmd *cmd, t_token **tokens, t_data *data);
t_node_type		get_logic_type(t_token *token);
t_token_type	get_operator_type(char *str);
t_cmd			*init_cmd(void);
bool			check_empty_pipe(t_token *tokens, t_data *data);
int				process_heredoc(char *delimiter,
					t_quote_status q_type, t_data *data);
int				count_args(t_token *tokens);
char			**extract_args(t_token **tokens);
t_cmd			*parse_simple_cmd(t_token **tokens, t_data *data);
t_ast			*parse_pipeline(t_token **tokens, t_data *data);
t_ast			*parse(char *input, t_data *data);
char			*remove_quotes(char *str);
bool			has_quotes(char *str);
int				is_operator_char(char c);
int				is_whitespace(char c);
int				skip_quotes(char *str, int i, char quote);
int				get_token_len(char *str);
t_token			*new_token(char *value, t_token_type type);
void			add_token(t_token **head, t_token *new);
t_token			*tokenize(char *input);
void			free_tokens(t_token *tokens);
char			*trim_quotes(char *str);
t_quote_status	get_heredoc_quote(char *s);

bool			preprocess_heredocs(t_ast *node, t_data *data);
t_ast			*parse_expr(t_token **tokens, t_data *data);
bool			check_syntax_tokens(t_token *tok, t_data *data);
bool			check_redir_tokens(t_token *tokens, t_data *data);
bool			is_heredoc_delim(t_token *token, t_token *prev);
void			remove_quotes_except_heredoc(t_token *tokens);
void			close_all_heredoc_fds(t_ast *node);

// ast
t_ast			*new_ast_node(t_node_type type, t_cmd *cmd);
void			free_ast(t_ast *node);

// signals
void			sigint_handler(int sig);
void			sigint_heredoc(int sig);
void			setup_signal(int signo, void (*handler)(int));
void			setup_parent_signals(void);
void			setup_child_signals(void);
void			setup_heredoc_signals(void);
void			ignore_signals(void);
void			sigint_handler_multiline(int sig);
void			restore_interactive(void);
int				ctrlc_event_hook(void);

// errors
int				errmsg_cmd(char *cmd, char *arg, char *msg, int status);
void			execve_error(t_data *data, char *path);
void			cleanup_and_exit(int status,
					t_data *data, int *fds, int fds_count);
int				errmsg_cmd_w_quote(char *cmd, char *arg, char *msg, int status);

// free
void			ft_free_array(char **arr);
void			free_single_cmd(t_cmd	*cmd);
void			free_cmds(t_cmd *cmd);
void			minishell_loop(t_data *data);
bool			is_builtin(const char *cmd);
void			cleanup(t_data *data);
int				check_syntax(char *line, t_data *data);

#endif
