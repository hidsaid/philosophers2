NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = main.c parsing_utils.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf
all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
