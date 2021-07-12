#include "../include/nm.h"

int nm(const char *path)
{
	int fd;
	
	fd = open(path, O_RDONLY, NULL);
	if (fd < 0)
	{
		ft_putstr("ft_nm: \'");
		ft_putstr(path);
		ft_puts("\': No such file");
		return (-1);
	}
	close(fd);
	return(0);
}



int main(int argc, char **argv)
{
	if (argc == 1)
		nm("a.out");
	else if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
			nm(argv[i]);
	}
}