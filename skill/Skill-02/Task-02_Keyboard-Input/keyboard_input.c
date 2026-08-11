#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

int main()
{
    struct termios oldt, newt;
    char buffer[BUFFER_SIZE];
    int position = 0;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Shellforge> ");
    fflush(stdout);

    while (1)
    {
        ch = getchar();

        if (ch == '\n')
        {
            buffer[position] = '\0';

            printf("\nCommand entered: %s\n", buffer);

            if (position == 4 &&
                buffer[0] == 'e' &&
                buffer[1] == 'x' &&
                buffer[2] == 'i' &&
                buffer[3] == 't')
            {
                break;
            }

            position = 0;

            printf("Shellforge> ");
            fflush(stdout);
        }
        else if (ch == 127 || ch == 8)
        {
            if (position > 0)
            {
                position--;

                printf("\b \b");
                fflush(stdout);
            }
        }
        else if (position < BUFFER_SIZE - 1)
        {
            buffer[position] = ch;
            position++;

            putchar(ch);
            fflush(stdout);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("Exiting Shellforge...\n");

    return 0;
}
