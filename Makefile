NAME=ft_nm

SRC=src/main.c src/string.c src/output.c src/mem.c
OBJ=obj/main.o obj/string.o obj/output.o obj/mem.o


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
	@git add $(SRC) Makefile include/nm.h
	@git commit -am "auto commit by Darodrig `date +%d-%m-%Y` from `uname -n`"

push: commit
	@git push

x: $(NAME)
	PATH=${PATH}:${PWD}
	@./$(NAME)