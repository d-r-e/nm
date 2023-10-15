NAME = ft_nm
SRC = src/nm.c
INC = -I inc
OBJ = $(SRC:.c=.o)
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -O2 -g3
LIBFT = libft/libft.a
MAKE = make -C

$(NAME): $(LIBFT) $(SRC) $(OBJ)
	gcc -o $(NAME) $(OBJ) -L libft -lft

$(LIBFT):
	$(MAKE) libft

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -I libft -c $< -o $@

clean: 
	$(MAKE) libft clean
	$(RM) $(OBJ)

fclean: clean
	$(MAKE) libft fclean
	$(RM) $(NAME)
re: fclean all

test: $(NAME)
	./test/test.sh
