NAME = philo_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=address

SRCS = $(shell find ./src -iname "*.c")
OBJS = ${SRCS:.c=.o}
DEPS = $(SRCS:.c=.d)

$(NAME): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re