NAME = minishell
NAME_BONUS = minishell_bonus
SANITIZE = #-fsanitize=address
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
O_READLINE_DIR = readline/install
O_LDFLAGS = -L$(O_READLINE_DIR)/lib -lreadline -lhistory -lncurses
O_CFLAG = -I$(O_READLINE_DIR)/include -Ilibft

LIBFT_SRC = libft/helpers1.c libft/helpers2.c libft/helpers3.c \
            libft/helpers4.c libft/helpers5.c libft/helpers6.c libft/helpers7.c 
LIBFT_OBJ = $(LIBFT_SRC:.c=.o)
SRC = builtins/builtins_utils.c builtins/echo.c builtins/env.c \
      builtins/pwd.c builtins/exit.c builtins/cd.c builtins/unset.c \
      builtins/export.c builtins/export_helpers1.c builtins/export_helpers2.c \
      builtins/ft_malloc.c builtins/ft_malloc_utils.c builtins/builtins.c \
      execution/exec.c execution/exec1.c execution/exec2.c \
      execution/exec_helper.c execution/heredoc1.c execution/heredoc2.c \
      execution/redirections.c execution/redirections2.c parsing/lexer1.c parsing/lexer2.c parsing/parser_utils1.c\
	  parsing/parser_utils2.c parsing/parser_utils3.c parsing/parser_utils4.c\
      parsing/shell_utils.c parsing/syntax.c parsing/syntax2.c main/shell_cleanup.c\
	  main/shell_execution.c main/shell_init_input.c\
      expand/args_split_utils.c expand/expand_buffer_utils.c \
	  expand/expand_command_utils.c expand/expand_command_utils2.c expand/expand_command_vars.c expand/expand_variable_handler.c \
	  expand/empty_vars/empty_list.c expand/empty_vars/empty_system.c


BNS_SRC = builtins/builtins_utils.c builtins/echo.c builtins/env.c \
		builtins/pwd.c builtins/exit.c builtins/cd.c builtins/unset.c \
		builtins/export.c builtins/export_helpers1.c builtins/export_helpers2.c \
		builtins/ft_malloc.c builtins/ft_malloc_utils.c builtins/builtins.c \
		execution/exec.c execution/exec1.c execution/exec2.c \
		execution/exec_helper.c execution/heredoc1.c execution/heredoc2.c \
		execution/redirections.c execution/redirections2.c parsing/lexer1.c parsing/lexer2.c parsing/parser_utils1.c\
		parsing/parser_utils2.c parsing/parser_utils3.c parsing/parser_utils4.c\
		parsing/shell_utils.c parsing/syntax.c parsing/syntax2.c main/shell_cleanup.c\
	    bonus/shell_execution_bonus.c main/shell_init_input.c\
		bonus/wild_card.c bonus/wild_card2.c expand/args_split_utils.c expand/expand_buffer_utils.c \
		expand/expand_command_utils.c expand/expand_command_utils2.c expand/expand_command_vars.c expand/expand_variable_handler.c \
		expand/empty_vars/empty_list.c expand/empty_vars/empty_system.c 


OBJ = $(SRC:.c=.o)
BNS_OBJ = $(BNS_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_OBJ)
	$(CC) $^ $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(O_LDFLAGS) -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(BNS_OBJ) $(LIBFT_OBJ)
	$(CC) $^ $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(O_LDFLAGS) -o $@

%.o: %.c minishell.h Makefile
	$(CC) $(CFLAGS) $(SANITIZE) $(O_CFLAG) -c $< -o $@

libft/%.o: libft/%.c libft/libft.h Makefile
	$(CC) $(CFLAGS) -Ilibft -c $< -o $@

clean:
	rm -f $(OBJ) $(BNS_OBJ) $(LIBFT_OBJ)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re bonus