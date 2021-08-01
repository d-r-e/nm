#include <unistd.h>

int main()
{
    char *s="Hello World\n";

    write(1, s, 12);
}