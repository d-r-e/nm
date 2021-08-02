#include <unistd.h>
#include <string.h>
int main()
{
    char *s="Hello World\n";

    write(1, s, strlen(s));
}