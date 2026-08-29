#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_HISTORY 100
#define BUFFER_SIZE 1024

char *history[MAX_HISTORY];
int history_count = 0;
int history_position = -1;

void disable_raw_mode(struct termios *original)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}

void enable_raw_mode(struct termios *original)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, original);
    raw = *original;

    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void add_history(const char *command)
{
    if (strlen(command) == 0)
        return;

    if (history_count == MAX_HISTORY)
    {
        free(history[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
            history[i - 1] = history[i];

        history_count--;
    }

    history[history_count] = strdup(command);
    history_count++;
}

void clear_line(int length)
{
    for (int i = 0; i < length; i++)
        write(STDOUT_FILENO, "\b \b", 3);
}

void display_history_command(char *buffer, int *length, int position)
{
    if (position >= 0 && position < history_count)
    {
        clear_line(*length);

        strcpy(buffer, history[position]);
        *length = strlen(buffer);

        write(STDOUT_FILENO, buffer, *length);
    }
}

int main()
{
    struct termios original;
    char buffer[BUFFER_SIZE];
    int length = 0;

    enable_raw_mode(&original);

    printf("Simple Command History\n");
    printf("Type commands. Use Up/Down arrows to navigate.\n");
    printf("Press Ctrl+D or type exit to quit.\n\n");

    while (1)
    {
        printf("\nshell> ");
        fflush(stdout);

        length = 0;
        buffer[0] = '\0';
        history_position = history_count;

        while (1)
        {
            char c;

            if (read(STDIN_FILENO, &c, 1) != 1)
                break;

            /* Enter key */
            if (c == '\n' || c == '\r')
            {
                buffer[length] = '\0';
                printf("\n");

                if (strcmp(buffer, "exit") == 0)
                {
                    disable_raw_mode(&original);

                    for (int i = 0; i < history_count; i++)
                        free(history[i]);

                    return 0;
                }

                if (length > 0)
                {
                    add_history(buffer);
                    printf("Command stored: %s\n", buffer);
                }

                break;
            }

            /* Ctrl+D */
            if (c == 4)
            {
                disable_raw_mode(&original);

                for (int i = 0; i < history_count; i++)
                    free(history[i]);

                printf("\nExiting...\n");
                return 0;
            }

            /* Backspace */
            if (c == 127 || c == 8)
            {
                if (length > 0)
                {
                    length--;
                    buffer[length] = '\0';
                    write(STDOUT_FILENO, "\b \b", 3);
                }

                continue;
            }

            /* Escape sequence */
            if (c == 27)
            {
                char seq[2];

                if (read(STDIN_FILENO, &seq[0], 1) != 1)
                    continue;

                if (read(STDIN_FILENO, &seq[1], 1) != 1)
                    continue;

                /* Up arrow */
                if (seq[0] == '[' && seq[1] == 'A')
                {
                    if (history_count > 0 && history_position > 0)
                    {
                        history_position--;
                        display_history_command(
                            buffer,
                            &length,
                            history_position
                        );
                    }

                    continue;
                }

                /* Down arrow */
                if (seq[0] == '[' && seq[1] == 'B')
                {
                    if (history_position < history_count - 1)
                    {
                        history_position++;
                        display_history_command(
                            buffer,
                            &length,
                            history_position
                        );
                    }
                    else
                    {
                        clear_line(length);
                        length = 0;
                        buffer[0] = '\0';
                        history_position = history_count;
                    }

                    continue;
                }

                continue;
            }

            /* Normal character */
            if (length < BUFFER_SIZE - 1)
            {
                buffer[length] = c;
                length++;

                write(STDOUT_FILENO, &c, 1);
            }
        }
    }

    disable_raw_mode(&original);

    return 0;
}
