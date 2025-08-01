NAME = minishell
SANITIZE = #-fsanitize=address
CC = cc
CFLAG = -Wall -Werror -Wextra -g
# hado lflags dreadline library 3ndi flmac
O_READLINE_DIR := readline/install
O_LDFLAGS += -L$(O_READLINE_DIR)/lib -lreadline -lhistory -lncurses
O_CFLAG = -I$(O_READLINE_DIR)/include -Ilibft # zdt readline library (MAC) #-Wall -Wextra -Werror
#------------------------------------------------------------------------------
LIBFT_SRC = ./libft/helpers1.c ./libft/helpers2.c ./libft/helpers3.c ./libft/helpers4.c ./libft/helpers5.c ./libft/helpers6.c 
LIBFT_OBJ = $(LIBFT_SRC:.c=.o)
READLINE_INC =# -I$(shell brew --prefix readline)/include
READLINE_LIB =# -L$(shell brew --prefix readline)/lib -lreadline

SRC = ./builtins/builtins_utils.c ./builtins/echo.c ./builtins/env.c ./builtins/pwd.c ./builtins/exit.c\
		./builtins/cd.c ./builtins/unset.c ./builtins/export.c ./builtins/export_helpers1.c ./builtins/export_helpers2.c\
		./builtins/ft_malloc.c ./builtins/ft_malloc_utils.c ./builtins/builtins.c ./execution/exec.c ./execution/exec1.c\
		./execution/exec2.c ./execution/exec_helper.c ./execution/heredoc1.c ./execution/heredoc2.c ./execution/redirections.c\
		./parsing/lexer.c ./parsing/parser.c ./parsing/shell_utils.c ./parsing/syntax.c ./parsing/minishell.c ./parsing/expand.c
OBJ = $(SRC:.c=.o)
ALL_OBJ = $(OBJ) $(LIBFT_OBJ)

all: $(NAME)

$(NAME): $(ALL_OBJ)
	$(CC) $(ALL_OBJ) $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(O_LDFLAGS) -lreadline -o $(NAME) $(READLINE_LIB)

%.o: %.c minishell.h Makefile
	$(CC) $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(READLINE_INC) -c $< -o $@

libft/%.o: libft/%.c libft/libft.h Makefile
	$(CC) $(CFLAGS) -Ilibft -c $< -o $@

clean:
	$(RM) $(ALL_OBJ)

fclean: clean
	$(RM) $(NAME) 

re: fclean all 
# .PHONY: 
# .SILENT:
