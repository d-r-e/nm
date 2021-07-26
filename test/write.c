#include <unistd.h>
#include <string.h>

int main(void)
{
    const char *s = "TEST\n";

    write(1, s, 5);
    return(strlen(s));
}