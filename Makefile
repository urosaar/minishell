NAME = minishell
CC = cc
CFLAG = -Wall -Werror -Wextra
SRC =   builtins_utils.c echo.c env.c  pwd.c exit.c cd.c split.c unset.c export.c utils.c parssing.c builtins.c exec.c #main.c
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