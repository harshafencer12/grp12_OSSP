#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define INITIAL_BUFFER_SIZE 32
#define HISTORY_INITIAL_SIZE 4

/* ==============================
   TERMINAL RAW MODE
   ============================== */

struct termios original_terminal;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}

void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &original_terminal);
    atexit(disable_raw_mode);

    struct termios raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* ==============================
   LINKED LIST FOR COMMAND HISTORY
   ============================== */

typedef struct HistoryNode
{
    char *command;
    struct HistoryNode *next;
} HistoryNode;

/* Add command to linked list */
void add_history(HistoryNode **head, const char *command)
{
    if (command == NULL || strlen(command) == 0)
        return;

    HistoryNode *new_node = malloc(sizeof(HistoryNode));

    if (new_node == NULL)
    {
        perror("malloc");
        return;
    }

    new_node->command = malloc(strlen(command) + 1);

    if (new_node->command == NULL)
    {
        perror("malloc");
        free(new_node);
        return;
    }

    strcpy(new_node->command, command);
    new_node->next = NULL;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        HistoryNode *temp = *head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = new_node;
    }
}

/* Display command history */
void display_history(HistoryNode *head)
{
    int count = 1;

    printf("\n\n========== COMMAND HISTORY ==========\n");

    while (head != NULL)
    {
        printf("%d  %s\n", count, head->command);
        head = head->next;
        count++;
    }

    printf("=====================================\n");
}

/* Free linked list memory */
void free_history(HistoryNode *head)
{
    HistoryNode *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;

        free(temp->command);
        free(temp);
    }
}

/* ==============================
   DYNAMIC INPUT BUFFER
   ============================== */

typedef struct
{
    char *data;
    size_t size;
    size_t length;
} InputBuffer;

/* Initialize dynamic buffer */
int initialize_buffer(InputBuffer *buffer)
{
    buffer->data = malloc(INITIAL_BUFFER_SIZE);

    if (buffer->data == NULL)
    {
        perror("malloc");
        return 0;
    }

    buffer->size = INITIAL_BUFFER_SIZE;
    buffer->length = 0;
    buffer->data[0] = '\0';

    return 1;
}

/* Resize buffer when required */
int resize_buffer(InputBuffer *buffer)
{
    size_t new_size = buffer->size * 2;

    char *new_data = realloc(buffer->data, new_size);

    if (new_data == NULL)
    {
        perror("realloc");
        return 0;
    }

    buffer->data = new_data;
    buffer->size = new_size;

    return 1;
}

/* Set buffer contents */
int set_buffer(InputBuffer *buffer, const char *text)
{
    size_t required_size = strlen(text) + 1;

    while (required_size > buffer->size)
    {
        if (!resize_buffer(buffer))
            return 0;
    }

    strcpy(buffer->data, text);
    buffer->length = strlen(text);

    return 1;
}

/* Add one character to buffer */
int append_character(InputBuffer *buffer, char c)
{
    if (buffer->length + 1 >= buffer->size)
    {
        if (!resize_buffer(buffer))
            return 0;
    }

    buffer->data[buffer->length] = c;
    buffer->length++;

    buffer->data[buffer->length] = '\0';

    return 1;
}

/* Remove last character */
void remove_last_character(InputBuffer *buffer)
{
    if (buffer->length > 0)
    {
        buffer->length--;
        buffer->data[buffer->length] = '\0';
    }
}

/* ==============================
   DISPLAY BUFFER
   ============================== */

void redraw_input(InputBuffer *buffer)
{
    printf("\r\033[2K");
    printf("skill03> %s", buffer->data);
    fflush(stdout);
}

/* ==============================
   MAIN PROGRAM
   ============================== */

int main()
{
    InputBuffer buffer;

    if (!initialize_buffer(&buffer))
        return 1;

    HistoryNode *history = NULL;

    printf("============================================\n");
    printf("             SKILL 03 DEMONSTRATION\n");
    printf("============================================\n");

    printf("\nFeatures implemented:\n");
    printf("1. Escape sequence handling\n");
    printf("2. Command history\n");
    printf("3. Previous command navigation\n");
    printf("4. Next command navigation\n");
    printf("5. Dynamic input buffer\n");
    printf("6. Automatic buffer resizing\n");
    printf("7. Backspace handling\n");
    printf("8. Linked list memory management\n");
    printf("9. Memory release using free()\n");

    printf("\nControls:\n");
    printf("UP ARROW    -> Previous command\n");
    printf("DOWN ARROW  -> Next command\n");
    printf("BACKSPACE   -> Delete character\n");
    printf("ENTER       -> Execute/store command\n");
    printf("Ctrl+D      -> Exit\n");
    printf("Type 'history' to display command history.\n\n");

    enable_raw_mode();

    int history_position = 0;

    while (1)
    {
        set_buffer(&buffer, "");

        printf("skill03> ");
        fflush(stdout);

        history_position = 0;

        while (1)
        {
            char c;

            ssize_t bytes_read = read(STDIN_FILENO, &c, 1);

            if (bytes_read <= 0)
                break;

            /* Ctrl+D */
            if (c == 4)
            {
                disable_raw_mode();

                printf("\n\nExiting Skill 03...\n");

                free(buffer.data);
                free_history(history);

                printf("Memory released successfully.\n");

                return 0;
            }

            /* ENTER */
            if (c == '\n' || c == '\r')
            {
                printf("\n");

                if (buffer.length > 0)
                {
                    if (strcmp(buffer.data, "history") == 0)
                    {
                        add_history(&history, buffer.data);

                        disable_raw_mode();
                        display_history(history);
                        enable_raw_mode();
                    }
                    else
                    {
                        printf("Command executed: %s\n", buffer.data);

                        add_history(&history, buffer.data);
                    }
                }

                break;
            }

            /* BACKSPACE */
            if (c == 127 || c == 8)
            {
                if (buffer.length > 0)
                {
                    remove_last_character(&buffer);

                    printf("\b \b");
                    fflush(stdout);
                }

                continue;
            }

            /* ESCAPE SEQUENCE */
            if (c == 27)
            {
                char sequence[2];

                if (read(STDIN_FILENO, &sequence[0], 1) <= 0)
                    continue;

                if (sequence[0] != '[')
                    continue;

                if (read(STDIN_FILENO, &sequence[1], 1) <= 0)
                    continue;

                /* UP ARROW */
                if (sequence[1] == 'A')
                {
                    if (history_position == 0)
                    {
                        history_position = 1;
                    }
                    else
                    {
                        history_position++;
                    }

                    HistoryNode *temp = history;
                    int count = 1;

                    while (temp != NULL &&
                           count < history_position)
                    {
                        temp = temp->next;
                        count++;
                    }

                    if (temp != NULL)
                    {
                        set_buffer(&buffer, temp->command);
                        redraw_input(&buffer);
                    }
                }

                /* DOWN ARROW */
                else if (sequence[1] == 'B')
                {
                    if (history_position > 0)
                    {
                        history_position--;

                        if (history_position == 0)
                        {
                            set_buffer(&buffer, "");
                            redraw_input(&buffer);
                        }
                        else
                        {
                            HistoryNode *temp = history;
                            int count = 1;

                            while (temp != NULL &&
                                   count < history_position)
                            {
                                temp = temp->next;
                                count++;
                            }

                            if (temp != NULL)
                            {
                                set_buffer(&buffer, temp->command);
                                redraw_input(&buffer);
                            }
                        }
                    }
                }

                continue;
            }

            /* Normal character */
            if (c >= 32 && c <= 126)
            {
                if (append_character(&buffer, c))
                {
                    putchar(c);
                    fflush(stdout);
                }
            }
        }
    }

    free(buffer.data);
    free_history(history);

    return 0;
}
