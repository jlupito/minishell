NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3

#source
SRCS_DIR =	sources/

SRCS = main.c\
setup_launching.c\
env/env_main.c\
lexer/lexer.c lexer/tokenizer.c lexer/token_utils.c\
parser/parser.c parser/parser_redir.c parser/parser_string.c parser/parser_fds.c\
exec/exec_single.c exec/exec_pipeline.c exec/exec_pid_fds.c\
builtins/builtins.c  builtins/mini_cd.c builtins/mini_echo.c builtins/mini_env.c\
builtins/mini_exit.c builtins/mini_export_print.c builtins/mini_export.c\
builtins/mini_pwd.c builtins/mini_unset.c\
utils/utils_exec.c utils/clean_lists.c utils/errors.c utils/utils_lists.c\
utils/utils_display.c utils/utils_exec_close.c\
expander/expander.c expander/quotes.c expander/expander_utils.c\
exec/heredocs.c\
signals.c\

SRCS_PREFIXED = $(addprefix $(SRCS_DIR), $(SRCS))

#objsm
OBJS = $(SRCS_PREFIXED:.c=.o)

all : $(NAME)

%.o: %.c ./include/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@
$(NAME): $(OBJS)
	@$(MAKE) -C ./libft
	@$(CC) $(CFLAGS) $(OBJS) -L./libft -lft -lm -lreadline -o $(NAME)
	@echo $(NAME) is built

clean :
	@$(MAKE) clean -C ./libft
	@rm -rf $(OBJS)
	@echo cleaning

fclean : clean
	@$(MAKE) fclean -C ./libft
	@rm -rf $(NAME)
	@echo "full clean"

re : fclean all

.PHONY	: all clean fclean re help
