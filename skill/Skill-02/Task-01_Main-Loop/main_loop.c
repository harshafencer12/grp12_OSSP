#include <stdio.h>
#include <string.h>

int main()
{
    char input[100];

    while (1)
    {
        printf("Shellforge> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Shellforge...\n");
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }

        printf("Command entered: %s\n", input);
    }

    return 0;
}
