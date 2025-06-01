NAME = minishell
CC = cc
<<<<<<< HEAD:parsing/Makefile
CFLAGS = -Wall -Wextra -Werror
SRC = lexer.c parser.c shell_utils.c syntax.c testing.c utils.c
=======
CFLAG = -Wall -Werror -Wextra
SRC = ./builtins/builtins_utils.c ./builtins/echo.c ./builtins/env.c  ./builtins/pwd.c ./builtins/exit.c ./builtins/cd.c ./builtins/split.c ./builtins/unset.c ./builtins/export.c ./builtins/utils.c ./execution/exec.c ./builtins/builtins.c parssing.c #main.c
>>>>>>> 956ebfc297fde9d21e0a44ab44ff93b8b6ef9eb2:Makefile
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