NAME=ft_nm

SRC=src/main.c src/string.c 
OBJ=obj/main.o obj/string.o


$(NAME): $(OBJ)
	gcc -Wall -Wextra -Werror $(OBJ) -o $(NAME)

$(OBJ): $(SRC)
	gcc -Wall -Wextra -Werror -c $(SRC)
	mv *.o obj

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

commit: re fclean
	@git commit -am "auto commit by Darodrig `date +%d-%m-%Y` from $(`uname -n`)"

push: commit
	git push

x: $(NAME)
	@./$(NAME)