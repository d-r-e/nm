NAME = ft_nm
SRC = src/nm.c src/symbols.c src/sort.c
INC = -I inc
OBJ = $(SRC:.c=.o)
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3
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
	$(RM) $(OBJ) $(LIBC_TAR)

fclean: clean
	$(MAKE) libft fclean
	$(RM) $(NAME)
re: fclean all

x: all
	./$(NAME) $(NAME)
test: $(NAME)
	./test/test.sh


.PHONY: test re all clean fclean