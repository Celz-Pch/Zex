##
## EPITECH PROJECT, 2025
## ~/epitech/delivery
## File description:
## Makefile
##

CC = clang

CFLAGS = -Wall -Iinclude -g

SRC = $(shell find src -type f -name "*.c")

OBJ = $(SRC:.c=.o)

NAME = zex

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lncurses
	rm -f $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f *.gcda *.gcno tests/*.gcda tests/*.gcno
	find . \( -name "*.gcda" -o -name "*.gcno" \) -delete

valgrind:
	valgrind --leak-check=full --log-file=valgrind.txt ./$(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all