#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

void demonstrate_dynamic_array()
{
    int size = 2;
    int count = 0;

    int *array = malloc(size * sizeof(int));

    if (array == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    printf("Dynamic Array:\n");

    for (int i = 1; i <= 5; i++)
    {
        if (count == size)
        {
            size *= 2;

            int *temp = realloc(array, size * sizeof(int));

            if (temp == NULL)
            {
                free(array);
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            array = temp;

            printf("Buffer resized to %d elements\n", size);
        }

        array[count] = i * 10;

        printf("Added: %d\n", array[count]);

        count++;
    }

    printf("Array contents: ");

    for (int i = 0; i < count; i++)
        printf("%d ", array[i]);

    printf("\n");

    free(array);
}

void demonstrate_linked_list()
{
    Node *head = NULL;

    printf("\nLinked List:\n");

    for (int i = 1; i <= 3; i++)
    {
        Node *new_node = malloc(sizeof(Node));

        if (new_node == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        new_node->value = i * 100;
        new_node->next = head;
        head = new_node;

        printf("Added node: %d\n", new_node->value);
    }

    printf("List contents: ");

    Node *current = head;

    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }

    printf("\n");

    /* Release linked-list memory */
    current = head;

    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    printf("Linked list memory released successfully.\n");
}

int main()
{
    printf("Dynamic Memory Management Demonstration\n");
    printf("========================================\n\n");

    demonstrate_dynamic_array();

    demonstrate_linked_list();

    printf("\nAll dynamically allocated memory was released.\n");

    return 0;
}
