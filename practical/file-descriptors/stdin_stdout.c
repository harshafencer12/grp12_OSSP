#include <stdio.h>
#include <unistd.h>

int main()
{
    char buffer[100];
    int n;

    printf("Enter something: ");
    fflush(stdout);

    n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        write(STDOUT_FILENO, buffer, n);
    }

    return 0;
}
