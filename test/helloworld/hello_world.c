#include <unistd.h>

int main(void)
{
	char *s = "Hello world\n";
	write(1, s, 12);
}
