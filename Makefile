NAME = minishell
CC = cc
CFLAG = -Wall -Werror -Wextra
SRC = ./builtins/builtins_utils.c ./builtins/echo.c ./builtins/env.c  ./builtins/pwd.c ./builtins/exit.c ./builtins/cd.c ./builtins/split.c ./builtins/unset.c ./builtins/export.c ./builtins/utils.c ./execution/exec.c ./builtins/builtins.c parssing.c #main.c
OBJ = $(SRC:.c=.o)
HEAD = fractol.h

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAG) $(OBJ) -lreadline -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) 

re: fclean all  