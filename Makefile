CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = parssing.c
OBJ = $(SRC:.c=.o)
NAME = minishell
HEADERS = minishell.h
MFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(MFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all