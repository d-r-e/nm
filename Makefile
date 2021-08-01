FILES	= ft_nm ft_otool
NM		= ft_nm
OT		= ft_otool

all:	$(FILES)

$(NM):
	$(MAKE) -C nm
	mv nm/ft_nm .
$(OT):	
	$(MAKE) -C otool
	mv otool/ft_otool .

clean:
	$(MAKE) -C nm clean
	$(MAKE) -C otool clean
	

fclean:	clean
	$(MAKE) fclean -C nm 
	$(MAKE) fclean -C otool
	rm -f $(FILES)

re:		fclean all

test:	$(OT)
	./test.sh