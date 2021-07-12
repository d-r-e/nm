#include "../inc/nm.h"

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
			puts(argv[i]);
	}
}