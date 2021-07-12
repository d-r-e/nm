NAME=a.out
SRC=src/main.c

$(NAME): $(SRC)
	gcc -Wall -Wextra -Werror $(SRC) -o $(NAME)

x: $(NAME)
	@./$(NAME)