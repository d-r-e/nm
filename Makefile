NAME = ft_nm
SRC = src/nm.c src/symbols.c
INC = -I inc
OBJ = $(SRC:.c=.o)
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3
LIBFT = libft/libft.a
MAKE = make -C
LIBC = libc.so
LIBC_VERSION=2.38
LIBC_TAR = test/glibc/glibc-$(LIBC_VERSION).tar.gz


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

$(LIBC_TAR):
	mkdir -p test/glibc
	wget https://ftp.gnu.org/gnu/libc/glibc-2.38.tar.gz -O $(LIBC_TAR)

$(LIBC): $(LIBC_TAR)
	# tar -xzf $(LIBC_TAR) -C test/glibc
	cd test/glibc/glibc-$(LIBC_VERSION) && mkdir -p build && cd build &&  .././configure --prefix=$(shell pwd)/test/glibc/glibc-$(LIBC_VERSION)/build && make -j4 && make install
	cd test/glibc/glibc-$(LIBC_VERSION) && make -j4
	cp test/glibc/glibc-$(LIBC_VERSION)/build/lib/libc.so.6 $(LIBC)

.PHONY: test re all clean fclean