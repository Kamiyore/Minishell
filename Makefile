# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 11:52:00 by otaniyuhi         #+#    #+#              #
#    Updated: 2025/04/12 12:31:51 by oyuhi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Readline library path for macOS
LIBS = -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -lreadline
LIBFT = libft/libft.a
LIBFT_REPO = https://github.com/yuhi-ootani/lifbt_all.git

LIBFT_DIR = libft
INC_DIR = includes
MAIN_DIR = srcs/a_main
PROMPT_DIR = srcs/b_prompt
TOKENIZATION_DIR = srcs/c_tokenization
PARSER_DIR = srcs/d_parser
EXPAND_DIR = srcs/e_expand
SIGNALS_DIR = srcs/signals
BUILTINS_DIR = srcs/g_builtins
UTILS_DIR = srcs/utils
MANAGE_ENV_DIR = srcs/manage_env
EXECUTOR_DIR = srcs/f_executor
FREE_DIR = srcs/h_free
	   
SRCS = $(MAIN_DIR)/main.c $(MAIN_DIR)/main_init.c  \
	   $(PROMPT_DIR)/prompt.c \
	   $(TOKENIZATION_DIR)/tokenization.c $(TOKENIZATION_DIR)/token_word.c $(TOKENIZATION_DIR)/token_utils.c \
	   $(PARSER_DIR)/parser.c $(PARSER_DIR)/check_syntax.c $(PARSER_DIR)/create_command.c $(PARSER_DIR)/set_command_data.c \
	   $(EXPAND_DIR)/expand.c $(EXPAND_DIR)/get_expanded_str.c $(EXPAND_DIR)/handle_dollar.c $(EXPAND_DIR)/split_and_quotes_removal.c $(EXPAND_DIR)/expand_filename.c \
       $(SIGNALS_DIR)/signals.c $(SIGNALS_DIR)/heredoc_signals.c\
	   $(BUILTINS_DIR)/ft_cd.c $(BUILTINS_DIR)/ft_echo.c $(BUILTINS_DIR)/ft_env.c $(BUILTINS_DIR)/ft_exit.c $(BUILTINS_DIR)/ft_export_main.c $(BUILTINS_DIR)/ft_export_set_env.c $(BUILTINS_DIR)/ft_export_sort_print.c $(BUILTINS_DIR)/ft_unset.c $(BUILTINS_DIR)/ft_pwd.c \
	   $(UTILS_DIR)/utils.c $(UTILS_DIR)/env_utils.c $(UTILS_DIR)/quotes_removal_utils.c $(UTILS_DIR)/split_quoted_words_utils.c \
	   $(EXECUTOR_DIR)/executor.c $(EXECUTOR_DIR)/redirection.c $(EXECUTOR_DIR)/run_commands_in_child.c \
	   $(EXECUTOR_DIR)/handle_heredoc_child.c $(EXECUTOR_DIR)/handle_heredoc.c \
	   $(EXECUTOR_DIR)/child_process_execute.c $(EXECUTOR_DIR)/child_process_setup.c $(EXECUTOR_DIR)/child_search_cmd_path.c \
	   $(FREE_DIR)/free.c $(FREE_DIR)/free_command.c \



OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME) 

$(LIBFT) :
	 @echo "🔍 Checking for libft..."
	 @if [ ! -d "$(LIBFT_DIR)" ]; then \
	 	echo "📥 Cloning libft..."; \
	 	git clone  --quiet $(LIBFT_REPO)  $(LIBFT_DIR); \
	 fi
	 @make -C $(LIBFT_DIR) --silent

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then \
		make clean -C $(LIBFT_DIR) --silent; \
	fi
	rm -f $(OBJS)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then \
		rm -rf $(LIBFT_DIR); \
		echo "🧹 Removing libft directory..."; \
	fi
	rm -f $(NAME)


re: fclean all


.PHONY: all clean fclean re 