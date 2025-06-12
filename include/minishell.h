/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:33:48 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/04/11 20:17:59 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h> //O_WRONLY O_APPEND O_CREAT
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>    //PATH_MAX
# include <string.h>    //strcmp
# include <sys/stat.h>  //struct stat	st; stat function
# include <sys/types.h> //pid_t
# include <sys/wait.h>  //waitpid
# include <termios.h>
# include <unistd.h> //getcwd

# define DELIMITERS " \t\n"

typedef struct s_minishell		t_minishell;
typedef struct s_command		t_command;

// RubiFont
// ▗▖  ▗▖ ▗▄▖ ▗▄▄▄▖▗▖  ▗▖
// ▐▛▚▞▜▌▐▌ ▐▌  █  ▐▛▚▖▐▌
// ▐▌  ▐▌▐▛▀▜▌  █  ▐▌ ▝▜▌
// ▐▌  ▐▌▐▌ ▐▌▗▄█▄▖▐▌  ▐▌

typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

// ▗▄▄▄▖      ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▖   ▗▖
//   █        ▐▛▚▞▜▌  █  ▐▛▚▖▐▌  █  ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌
//   █        ▐▌  ▐▌  █  ▐▌ ▝▜▌  █   ▝▀▚▖▐▛▀▜▌▐▛▀▀▘▐▌   ▐▌
//   █  ▗▄▄▄▄ ▐▌  ▐▌▗▄█▄▖▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖▐▙▄▄▖

typedef struct s_minishell
{
	t_env						*env;
	char						*input;
	t_command					*commands;
	int							exit_status;
	int							original_stdin;
	int							original_stdout;
}								t_minishell;

/* main.c */
void							init_shell_struct(t_minishell *shell,
									char **envp);
/*main_init.c*/
bool							decide_input_fd(t_minishell *shell, int argc,
									char **argv);
// ▗▄▄▖ ▗▄▄▖  ▗▄▖ ▗▖  ▗▖▗▄▄▄▖
// ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▛▚▞▜▌  █
// ▐▛▀▘ ▐▛▀▚▖▐▌ ▐▌▐▌  ▐▌  █
// ▐▌   ▐▌ ▐▌▝▚▄▞▘▐▌  ▐▌  █

char							*get_input(t_minishell *shell,
									bool interactive_mode);

// ▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌  █     ▗▞▘▐▌   ▐▌ ▐▌
//   █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌  █   ▗▞▘  ▐▛▀▀▘▐▛▀▚▖
//   █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌▗▄█▄▖▐▙▄▄▄▖▐▙▄▄▖▐▌ ▐▌

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

/*token_utils.c*/
t_token							*create_token(t_minishell *shell,
									t_token_type type, const char *value);
bool							add_token(t_token **head, t_token *new_node);
/*token_word.c*/
bool							add_word_token(t_minishell *shell, size_t *i,
									t_token **tokens);
/* tokenization.c */
t_token							*tokenizer(t_minishell *shell);

// ▗▄▄▖  ▗▄▖ ▗▄▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌
// ▐▛▀▘ ▐▛▀▜▌▐▛▀▚▖ ▝▀▚▖▐▛▀▀▘▐▛▀▚▖
// ▐▌   ▐▌ ▐▌▐▌ ▐▌▗▄▄▞▘▐▙▄▄▖▐▌ ▐▌

typedef struct s_redirection
{
	char						*filename;
	t_token_type				type;
}								t_redirection;

typedef struct s_command
{
	char						**args;
	t_redirection				*infiles;
	size_t						infile_count;
	t_redirection				*outfiles;
	size_t						outfile_count;
	struct s_command			*next;
}								t_command;
/*check_syntax.c*/
bool							is_syntax_error(t_minishell *shell,
									t_token *tokens);
/*create_command.c*/
t_command						*convert_token_into_cmd(t_minishell *shell,
									t_token *tokens);
/*parser.c*/
t_command						*parser(t_minishell *shell, t_token *tokens);
/*set_command_data.c*/
bool							is_redirection_type(t_token_type type);
bool							set_redirection(t_minishell *shell,
									t_command *cmd, t_token *tokens);
bool							add_argument(t_minishell *shell, t_command *cmd,
									char *new_arg);

// ▗▄▄▖ ▗▄▄▄▖▗▄▄▄ ▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄▖ ▗▄▖ ▗▖  ▗▖
// ▐▌ ▐▌▐▌   ▐▌  █  █  ▐▌ ▐▌▐▌   ▐▌     █    █  ▐▌ ▐▌▐▛▚▖▐▌
// ▐▛▀▚▖▐▛▀▀▘▐▌  █  █  ▐▛▀▚▖▐▛▀▀▘▐▌     █    █  ▐▌ ▐▌▐▌ ▝▜▌
// ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▀▗▄█▄▖▐▌ ▐▌▐▙▄▄▖▝▚▄▄▖  █  ▗▄█▄▖▝▚▄▞▘▐▌  ▐▌

bool							input_redirection(t_minishell *shell,
									t_command *cmd);
bool							output_redirection(t_minishell *shell,
									t_command *cmd);

void							clean_heredoc_tmpfile(t_minishell *shell);

// ▗▄▄▄▖▗▖  ▗▖▗▄▄▖  ▗▄▖ ▗▖  ▗▖▗▄▄▄
// ▐▌    ▝▚▞▘ ▐▌ ▐▌▐▌ ▐▌▐▛▚▖▐▌▐▌  █
// ▐▛▀▀▘  ▐▌  ▐▛▀▘ ▐▛▀▜▌▐▌ ▝▜▌▐▌  █
// ▐▙▄▄▖▗▞▘▝▚▖▐▌   ▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀

typedef struct s_expanded_str
{
	char						*buffer;
	size_t						size;
	size_t						index;
	bool						in_single_quote;
	bool						in_double_quote;
}								t_expanded_str;

bool							expand_all_cmd_args(t_minishell *shell);
char							*get_expanded_str(t_minishell *shell,
									const char *src_input);
bool							append_to_buffer(t_minishell *shell,
									t_expanded_str *expanded_str,
									const char *src, size_t src_len);
bool							handle_dollar(t_minishell *shell,
									t_expanded_str *expanded_str,
									const char *src_input, size_t *i);
bool							quote_removal_args(t_minishell *shell,
									char **args);
char							**word_splitting(t_minishell *shell, char *str);
char							*expand_filename(t_minishell *shell,
									t_redirection file);

// ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▖ ▗▄▄▖
// ▐▌    ▝▚▞▘ ▐▌   ▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▌ ▐▌
// ▐▛▀▀▘  ▐▌  ▐▛▀▀▘▐▌   ▐▌ ▐▌  █ ▐▌ ▐▌▐▛▀▚▖
// ▐▙▄▄▖▗▞▘▝▚▖▐▙▄▄▖▝▚▄▄▖▝▚▄▞▘  █ ▝▚▄▞▘▐▌ ▐▌

typedef enum e_builtin_index
{
	FT_ECHO,
	FT_CD,
	FT_PWD,
	FT_EXPORT,
	FT_UNSET,
	FT_ENV,
	FT_EXIT,
	NOT_BUILTIN,
}								t_builtin_id;

# define NUM_BUILTINS 7

typedef struct s_exec
{
	int							input_fd;
	int							pipe_fds[2];
	t_builtin_id				builtin_id;
	int							(*builtins[NUM_BUILTINS])(t_minishell *);
	pid_t						*pid_array;
}								t_exec;

/*child_process.c*/
void							cleanup_and_exit_child(t_minishell *shell,
									t_exec *exec_info, int exit_status);
void							execute_child_process(t_minishell *shell,
									t_exec *exec_info, t_command *cmd);
char							*search_command_in_path(t_minishell *shell,
									t_exec *exec_info, const char *command);
/*execute_cmd.c*/
void							execute_external_command(t_minishell *shell,
									t_exec *exec_info, t_command *cmd);
/*executor.c*/
t_builtin_id					is_builtin(char *command_str);
void							cmd_executor(t_minishell *shell);

/*handle_heredoc.c*/
void							child_heredoc(t_minishell *shell,
									char *filename, char *eof_name);
bool							handle_heredoc(t_minishell *shell);

/*run_commands_in_child*/
void							run_commands_in_child(t_minishell *shell,
									t_exec *exec_info);

// ▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖ ▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖
// ▐▌ ▐▌▐▌ ▐▌  █  ▐▌   █    █  ▐▛▚▖▐▌▐▌
// ▐▛▀▚▖▐▌ ▐▌  █  ▐▌   █    █  ▐▌ ▝▜▌ ▝▀▚▖
// ▐▙▄▞▘▝▚▄▞▘▗▄█▄▖▐▙▄▄▖█  ▗▄█▄▖▐▌  ▐▌▗▄▄▞▘

int								ft_echo(t_minishell *shell);
int								ft_cd(t_minishell *shell);
int								ft_pwd(t_minishell *shell);
int								ft_unset(t_minishell *shell);
int								ft_env(t_minishell *shell);
int								ft_exit(t_minishell *shell);
int								ft_export(t_minishell *shell);
int								sort_and_print_env(t_env **copied_env);
int								set_env_value(t_env *copied_env,
									const char *new_name, const char *new_value,
									bool append);

//  ▗▄▄▖▗▄▄▄▖ ▗▄▄▖▗▖  ▗▖ ▗▄▖ ▗▖
// ▐▌     █  ▐▌   ▐▛▚▖▐▌▐▌ ▐▌▐▌
//  ▝▀▚▖  █  ▐▌▝▜▌▐▌ ▝▜▌▐▛▀▜▌▐▌
// ▗▄▄▞▘▗▄█▄▖▝▚▄▞▘▐▌  ▐▌▐▌ ▐▌▐▙▄▄▖

extern volatile sig_atomic_t	g_signal;

void							setup_signals_child(void);
void							setup_signals_parent(void);
void							setup_signals_heredoc(void);
void							sig_handler_heredoc(int sig);
void							ignore_sigint(struct sigaction *original);
void							restore_sigint(struct sigaction *original);

// ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖    ▗▄▄▖
// ▐▌ ▐▌  █    █  ▐▌   ▐▌
// ▐▌ ▐▌  █    █  ▐▌    ▝▀▚▖
// ▝▚▄▞▘  █  ▗▄█▄▖▐▙▄▄▖▗▄▄▞▘

size_t							count_env_util(t_env *env);
void							set_exit_failure_util(t_minishell *shell);
t_env							*create_new_env_util(const char *new_name,
									const char *new_value, t_env *new_next);
void							env_add_back_util(t_env **copied_env,
									t_env *new_env);
bool							get_env_value_util(t_minishell *shell,
									const char *name, char **result);
char							**split_quoted_words_util(char const *s,
									const char *delimiters);
char							*remove_quotes_util(const char *input);

// ▗▄▄▄ ▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖ ▗▄▄▖    ▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖    ▗▄▄▖
// ▐▌  █▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌       ▐▌ ▐▌  █    █  ▐▌   ▐▌
// ▐▌  █▐▛▀▀▘▐▛▀▚▖▐▌ ▐▌▐▌▝▜▌    ▐▌ ▐▌  █    █  ▐▌    ▝▀▚▖
// ▐▙▄▄▀▐▙▄▄▖▐▙▄▞▘▝▚▄▞▘▝▚▄▞▘    ▝▚▄▞▘  █  ▗▄█▄▖▐▙▄▄▖▗▄▄▞▘

void							print_cmds(t_command *head);
void							print_tokens(t_token *tokens);
size_t							ft_array_count_str(char **array);

// ▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖▗▄▄▄▖
// ▐▌   ▐▌ ▐▌▐▌   ▐▌
// ▐▛▀▀▘▐▛▀▚▖▐▛▀▀▘▐▛▀▀▘
// ▐▌   ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖

void							free_all_cmds(t_command *head);
void							free_shell(t_minishell *shell);
int								get_exit_status(int err);
void							free_tokens(t_token *tokens);
void							free_copied_env(t_env *env);

#endif
