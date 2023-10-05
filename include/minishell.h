/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:59:33 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 15:11:43 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define INIT_FAIL 254
# define FAILURE -1
# define SUCCESS 0
# define FALSE 0
# define TRUE 1

/* CODES CONVENTION BASH */
# define TOO_MANY_ARG 1
# define WRONG_ARG 1
# define NO_FILE_DIR_PATH 1
# define SYNTAX_ERROR 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127

/* CODES PERSONNALISES */
# define PROCESS_FAIL 0
# define MALLOC_FAIL 12
# define FORK_FAIL 2
# define EXECVE_FAIL 3
# define PIPE_FAIL 4
# define OPEN_FAIL 5
# define SIGNALS_FAIL 6
# define NO_ENV_SET 9
# define BASH_SIGPIPE 13

# define MAX_CHILDREN 1024

# include "../libft/libft.h"
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <ctype.h>
# include <string.h>
# include <fcntl.h>
# include <stdarg.h>
# include <pthread.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>

typedef enum e_token
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC,
	PIPE,
	NONE,
	SINGLE_QUO,
	DOUBLE_QUO,
	HANDLED,
}	t_token;

typedef struct s_lexer
{
	char			*str;
	t_token			token_type;
	int				expanded;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_file
{
	char			*file_name;
	int				token_type;
	struct s_file	*next;
}	t_file;

typedef struct s_cmd
{
	char			**cmd_args;
	char			*full_path;
	int				fd_input;
	int				fd_output;
	pid_t			id_process;
	struct s_file	*file_in;
	struct s_file	*file_out;
	int				heredoc;
}			t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}		t_env;

typedef struct s_data
{
	t_env			*envg;
	char			**env_export;
	t_lexer			*line_lexered;
	t_cmd			*cmds;
	int				nb_cmd;
	pid_t			pid;
	pid_t			pids_child[MAX_CHILDREN];
	int				count_childpids;
	int				flag_pars;
	int				fd_tmp;
	int				fd_pipe[2];
	int				mini_exit_code;
}	t_data;

extern int			g_signal_status;

/* ENV */
int			setup_lauching(t_data **data, int ac, char **av, char **env);
void		free_env(t_data *data);
int			ft_lstsize_env(t_env *envg);
t_env		*save_env(t_data *data, char **env);
t_env		*ft_add_back_env(t_env **lst, t_env *new);
t_env		*ft_last_env(t_env *lst);
t_env		*ft_new_env(t_data *data, char *str);
void		ft_init_mini(t_data *data, char **env);

/* LEXER TOKENS */
int			ft_lexer_parse(t_data *data, char *str);
int			ft_lexer_parse_msg(t_lexer *lex_copy, t_data *data);
t_lexer		*ft_lexer(char *str);
t_lexer		*ft_add_token(char *str1, t_token token);
void		ft_add_back_token(t_lexer **lst, t_lexer *new);
int			ft_tokenize(t_lexer **lexer, char *str);
int			ft_tokenize_redir(t_lexer **lexer, char *str);
int			ft_tokenize_quote(t_lexer **lexer, char *str);
int			ft_tokenize_none(t_lexer **lexer, char *str);

/* EXPANDER */
char		*ft_check_remove_quotes(char *str, t_data *data);
int			ft_check_open_quotes(char *str);
void		ft_remove_quotes(char *str, char *str_replace);
int			ft_count_quotes(char *str);
bool		ft_expand(t_data *data);
void		ft_check_expansion(char **str, t_lexer *lex, t_data *data);
int			ft_expand_var(char **str, int start_pos, int quote_type,
				t_data *data);
int			ft_get_var_val(char *str, char **var_env, int quote_type,
				t_data *data);
int			ft_get_var_len(char *str, int quoted);
char		*ft_get_env_val(char *var_name, t_env *envg);
void		ft_split_args(char *str, t_lexer *lex);
void		ft_split_add_token(char **str, t_lexer *lex, int i);

/* DISPLAY */
void		display_token(t_lexer **lexer);
void		print_block_cmd(t_data *data);

/* PARSING */
int			ft_count_cmds(t_lexer *lexer);
int			ft_check_redir(t_lexer *lexer, t_cmd *cmd, t_data *data);
int			ft_nb_of_args(t_lexer *lexer);
bool		ft_save_redir(t_file **files, char *name, int type, t_data *data);
void		ft_parse_cmd(t_data *data);
void		ft_init_cmd(t_data *data);
bool		ft_parse_fds(t_data *data);
bool		ft_parser(t_data *data);
t_lexer		*ft_create_cmd(t_lexer *lexer, t_cmd *cmd, t_data *data);
t_lexer		*ft_get_strings(t_lexer *lexer, t_cmd *cmd, t_data *data);
t_lexer		*ft_save_string(t_lexer *lexer, t_cmd *cmd, t_data *data);
t_lexer		*ft_get_redir(t_lexer *lexer, t_cmd *cmd, t_data *data);
void		ft_parse_fds_in(t_file *file_out, t_cmd *cmd, t_data *data);
void		ft_parse_fds_out(t_file *file_out, t_cmd *cmd, t_data *data);
int			ft_check_particular(t_data *data);

/* ERRORS & FREE */
void		free_cmd_table(t_data *data);
void		free_lexer(t_lexer *lexer);
void		free_cmd_iofile(t_file *file);
void		free_cmd_table_child(t_cmd *cmds);
void		error_message(char *msg, char *cmd, int code, t_data *data);
void		non_existing_command(t_data *data, t_cmd cmd);
void		no_such_file(t_data *data, t_cmd cmd);
void		function_failed(t_data *data, int ret, int type_fail,
				char *error_message);

/* PARSING UTILS */
int			ft_is_space(int c);
int			ft_is_sep(int c);
int			ft_is_redir(t_token token);
int			ft_is_string(t_token token);
t_token		ft_which_quote(char c);
int			ft_which_quote2(char c);

/* UTILS EXEC */
int			prompt_loop(char *prompt, t_data *data);
int			array_len(char **tab);
void		free_tabs(char **tab);
void		ft_swap_strings(char **str1, char **str2);
char		**ft_strdup_tab(char **tab, int size);
char		**ft_sort_tab(t_data *data, int size, char **tab);
char		**copy_lst_in_tab2(t_data *data);
int			process_copy_lst2(t_env *current, char **env_tab);
char		**copy_lst_in_tab(t_data *data);
int			process_copy_lst(t_env *current, char **env_tab);
void		close_all_fds(t_data *data, int flag, int i);
void		close_next_fds(t_data *data, int i);
void		free_all_children(t_data *data);

/* EXECUTOR */
void		close_fd(int fd);
char		*get_real_path(t_cmd cmd, t_data *data);
char		**get_path_env(t_data *data, t_cmd cmd);
int			manage_single_cmd_fds(t_cmd cmd);
void		manage_pipe_fds(t_data *data, t_cmd cmd, int i);
void		manage_redir_fds(t_data *data, t_cmd cmd);
void		parent_single_cmd(t_data *data, t_cmd cmd);
void		execute_single_cmd(t_cmd cmd, t_data *data);
void		execute_pipeline(t_cmd *cmd, t_data *data);
void		wait_children_pids(t_data *data);
void		fork_and_exec_function(t_cmd cmd, t_data *data);
void		exec_function(t_cmd cmd, t_data *data);
void		execution_loop(t_data *data, t_cmd *cmds, int i);
void		executor(t_data *data);
void		parent_process(t_data *data, int pipe);
void		child_process(t_cmd *cmds, t_data *data, int i);

/* HEREDOCS */
void		check_heredoc(t_data *data);
int			create_heredoc(char *delim, t_data *data);
char		*expand_heredoc(char *prompt, t_data *data);
void		heredoc_loop(char *prompt, t_data *data, int fd);

/* SIGNALS */
void		child_sigint(int signal);
void		manage_sigint(int smultiple_cmdignum);
void		signals_monitoring(void);

/* BUILTINS */
int			test_builtin(t_cmd cmd);
int			exec_builtin(t_cmd cmd, t_data *data);
int			ft_is_valid_numb(const char *nptr);
int			is_valid_var_name(t_data *data, char *cmd_var);
int			existing_var_name(const char *var_arg, t_data *data);
int			existing_var_name_export(const char *var_arg, t_data *data);
void		create_new_var(char *args, t_data *data);
void		get_new_direc(t_data *data, t_cmd cmd, char *old);
void		get_new_path_direc(char *cmd_path, char **add_new_path, char *old);
int			mini_env(t_data *data);
int			mini_exit(t_cmd cmd, t_data *data);
void		mini_exit_code(t_cmd cmd, t_data *data);
void		mini_exit_args(char **cmd_args);
int			mini_echo(t_data *data, t_cmd cmd);
void		print_echo(char **str);
int			mini_cd(t_cmd cmd, t_data *data);
void		cd_error(t_data *data, char *old_path, char *cmd, int flag);
void		update_pwd(char *path, char *old_path, t_data *data);
int			mini_pwd(t_data *data);
void		unset_var_env(char *cmd_var, t_data *data);
void		unset_var_export(char *cmd_var, t_data *data);
int			mini_unset(t_cmd cmd, t_data *data);
void		add_var(char *var_name, char *var_value, t_data *data);
char		*ft_add_new_value(char *name, char *val, t_data *data);
int			add_var_in_export(char *var_name, char *var_value,
				t_data *data, int flag);
int			sort_and_print_var_export(t_cmd cmd, t_data *data);
int			mini_export(t_cmd cmd, t_data *data);
void		replace_var_in_export(char *var_name, char *var_value,
				t_data *data);

#endif
