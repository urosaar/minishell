NAME = minishell
SANITIZE =# -fsanitize=address
CC = cc
CFLAG = -Wall -Werror -Wextra -g
# hado lflags dreadline library 3ndi flmac
O_READLINE_DIR := readline/install
O_LDFLAGS += -L$(O_READLINE_DIR)/lib -lreadline -lhistory -lncurses
O_CFLAG = -I$(O_READLINE_DIR)/include # zdt readline library (MAC) #-Wall -Wextra -Werror
#------------------------------------------------------------------------------

READLINE_INC =# -I$(shell brew --prefix readline)/include
READLINE_LIB =# -L$(shell brew --prefix readline)/lib -lreadline

SRC = ./builtins/builtins_utils.c ./builtins/echo.c ./builtins/env.c  ./builtins/pwd.c ./builtins/exit.c\
		./builtins/cd.c ./builtins/split.c ./builtins/unset.c ./builtins/export.c ./builtins/utils.c\
		./builtins/ft_malloc.c ./builtins/ft_malloc_utils.c ./execution/exec.c ./builtins/builtins.c\
		./parsing/lexer.c ./parsing/parser.c ./parsing/shell_utils.c ./parsing/syntax.c ./parsing/testing.c ./parsing/utils.c ./parsing/expand.c
OBJ = $(SRC:.c=.o)
HEAD = fractol.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(O_LDFLAGS) -lreadline -o $(NAME) $(READLINE_LIB)

%.o: %.c minishell.h Makefile
	$(CC) $(CFLAGS) $(SANITIZE) $(O_CFLAG) $(READLINE_INC) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) 

re: fclean all 
# .PHONY: 
# .SILENT: