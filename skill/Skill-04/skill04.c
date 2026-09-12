#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_TOKENS 64
#define MAX_CHILDREN 32

/* ==========================================
   TOKEN DEFINITIONS
   ========================================== */

typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_INPUT_REDIRECT,
    TOKEN_OUTPUT_REDIRECT,
    TOKEN_APPEND_REDIRECT,
    TOKEN_END
} TokenType;

typedef struct
{
    TokenType type;
    char value[MAX_INPUT];
} Token;

/* ==========================================
   PARSE TREE NODE
   ========================================== */

typedef struct ParseNode
{
    char value[MAX_INPUT];
    struct ParseNode *children[MAX_CHILDREN];
    int child_count;
} ParseNode;

/* ==========================================
   EXECUTION STRUCTURE
   ========================================== */

typedef struct
{
    char *command;
    char *arguments[MAX_TOKENS];
    int argument_count;

    char *input_file;
    char *output_file;

    int append_output;
} ExecutionStructure;

/* ==========================================
   TOKEN TYPE TO STRING
   ========================================== */

const char *token_type_string(TokenType type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_INPUT_REDIRECT:
            return "INPUT_REDIRECT";

        case TOKEN_OUTPUT_REDIRECT:
            return "OUTPUT_REDIRECT";

        case TOKEN_APPEND_REDIRECT:
            return "APPEND_REDIRECT";

        case TOKEN_END:
            return "END";

        default:
            return "UNKNOWN";
    }
}

/* ==========================================
   ADD TOKEN
   ========================================== */

void add_token(Token tokens[], int *count,
               TokenType type, const char *value)
{
    if (*count >= MAX_TOKENS - 1)
        return;

    tokens[*count].type = type;

    strncpy(tokens[*count].value,
            value,
            MAX_INPUT - 1);

    tokens[*count].value[MAX_INPUT - 1] = '\0';

    (*count)++;
}

/* ==========================================
   TOKENIZER
   ========================================== */

int tokenize(const char *input, Token tokens[])
{
    int count = 0;
    int i = 0;

    while (input[i] != '\0')
    {
        /* Ignore whitespace */
        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|')
        {
            add_token(tokens, &count,
                      TOKEN_PIPE, "|");

            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<')
        {
            add_token(tokens, &count,
                      TOKEN_INPUT_REDIRECT, "<");

            i++;
            continue;
        }

        /* Output redirection / append */
        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                add_token(tokens, &count,
                          TOKEN_APPEND_REDIRECT,
                          ">>");

                i += 2;
            }
            else
            {
                add_token(tokens, &count,
                          TOKEN_OUTPUT_REDIRECT,
                          ">");

                i++;
            }

            continue;
        }

        /* Normal word */
        char word[MAX_INPUT];
        int j = 0;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>')
        {
            if (j < MAX_INPUT - 1)
            {
                word[j++] = input[i];
            }

            i++;
        }

        word[j] = '\0';

        if (j > 0)
        {
            add_token(tokens, &count,
                      TOKEN_WORD, word);
        }
    }

    add_token(tokens, &count,
              TOKEN_END, "END");

    return count;
}

/* ==========================================
   DEBUG TOKEN OUTPUT
   ========================================== */

void print_tokens(Token tokens[], int count)
{
    printf("\n========== TOKEN DEBUG OUTPUT ==========\n");

    for (int i = 0; i < count; i++)
    {
        printf("Token %d: %-18s Value: \"%s\"\n",
               i + 1,
               token_type_string(tokens[i].type),
               tokens[i].value);

        if (tokens[i].type == TOKEN_END)
            break;
    }

    printf("========================================\n");
}

/* ==========================================
   VALIDATE TOKEN STREAM
   ========================================== */

int validate_tokens(Token tokens[], int count)
{
    int expecting_command = 1;

    for (int i = 0; i < count - 1; i++)
    {
        TokenType type = tokens[i].type;

        /* Empty command */
        if (type == TOKEN_END && expecting_command)
        {
            return 0;
        }

        /* Command/word */
        if (type == TOKEN_WORD)
        {
            expecting_command = 0;
            continue;
        }

        /* Pipe */
        if (type == TOKEN_PIPE)
        {
            if (expecting_command)
            {
                printf("Syntax Error: Invalid pipe placement.\n");
                return 0;
            }

            expecting_command = 1;
            continue;
        }

        /* Redirection */
        if (type == TOKEN_INPUT_REDIRECT ||
            type == TOKEN_OUTPUT_REDIRECT ||
            type == TOKEN_APPEND_REDIRECT)
        {
            if (expecting_command)
            {
                printf("Syntax Error: Redirection without command.\n");
                return 0;
            }

            if (i + 1 >= count - 1 ||
                tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Syntax Error: Redirection requires a file.\n");
                return 0;
            }

            i++;
            continue;
        }
    }

    if (expecting_command)
    {
        printf("Syntax Error: Command expected after pipe.\n");
        return 0;
    }

    return 1;
}

/* ==========================================
   CREATE PARSE TREE NODE
   ========================================== */

ParseNode *create_node(const char *value)
{
    ParseNode *node = malloc(sizeof(ParseNode));

    if (node == NULL)
    {
        perror("malloc");
        exit(1);
    }

    strncpy(node->value,
            value,
            MAX_INPUT - 1);

    node->value[MAX_INPUT - 1] = '\0';

    node->child_count = 0;

    for (int i = 0; i < MAX_CHILDREN; i++)
        node->children[i] = NULL;

    return node;
}

/* ==========================================
   ADD CHILD
   ========================================== */

void add_child(ParseNode *parent, ParseNode *child)
{
    if (parent->child_count < MAX_CHILDREN)
    {
        parent->children[parent->child_count++] = child;
    }
}

/* ==========================================
   GENERATE PARSE TREE
   ========================================== */

ParseNode *generate_parse_tree(Token tokens[], int count)
{
    ParseNode *root = create_node("COMMAND");

    ParseNode *current_command = create_node("COMMAND_SEGMENT");

    add_child(root, current_command);

    for (int i = 0; i < count - 1; i++)
    {
        if (tokens[i].type == TOKEN_PIPE)
        {
            current_command = create_node("COMMAND_SEGMENT");

            add_child(root, current_command);
        }
        else
        {
            ParseNode *node =
                create_node(tokens[i].value);

            add_child(current_command, node);
        }
    }

    return root;
}

/* ==========================================
   PRINT PARSE TREE
   ========================================== */

void print_tree(ParseNode *node, int level)
{
    for (int i = 0; i < level; i++)
        printf("  ");

    printf("|-- %s\n", node->value);

    for (int i = 0; i < node->child_count; i++)
    {
        print_tree(node->children[i],
                   level + 1);
    }
}

/* ==========================================
   FREE PARSE TREE
   ========================================== */

void free_tree(ParseNode *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < node->child_count; i++)
    {
        free_tree(node->children[i]);
    }

    free(node);
}

/* ==========================================
   CREATE EXECUTION STRUCTURE
   ========================================== */

void create_execution_structure(
    Token tokens[],
    int count,
    ExecutionStructure *execution)
{
    execution->command = NULL;
    execution->argument_count = 0;
    execution->input_file = NULL;
    execution->output_file = NULL;
    execution->append_output = 0;

    for (int i = 0; i < MAX_TOKENS; i++)
        execution->arguments[i] = NULL;

    for (int i = 0; i < count - 1; i++)
    {
        if (tokens[i].type == TOKEN_WORD)
        {
            if (execution->command == NULL)
            {
                execution->command =
                    strdup(tokens[i].value);
            }

            if (execution->argument_count <
                MAX_TOKENS - 1)
            {
                execution->arguments[
                    execution->argument_count++] =
                    strdup(tokens[i].value);
            }
        }

        else if (tokens[i].type ==
                 TOKEN_INPUT_REDIRECT)
        {
            if (i + 1 < count &&
                tokens[i + 1].type == TOKEN_WORD)
            {
                execution->input_file =
                    strdup(tokens[++i].value);
            }
        }

        else if (tokens[i].type ==
                 TOKEN_OUTPUT_REDIRECT)
        {
            if (i + 1 < count &&
                tokens[i + 1].type == TOKEN_WORD)
            {
                execution->output_file =
                    strdup(tokens[++i].value);
            }

            execution->append_output = 0;
        }

        else if (tokens[i].type ==
                 TOKEN_APPEND_REDIRECT)
        {
            if (i + 1 < count &&
                tokens[i + 1].type == TOKEN_WORD)
            {
                execution->output_file =
                    strdup(tokens[++i].value);
            }

            execution->append_output = 1;
        }
    }
}

/* ==========================================
   PRINT EXECUTION STRUCTURE
   ========================================== */

void print_execution_structure(
    ExecutionStructure *execution)
{
    printf("\n======= EXECUTION STRUCTURE =======\n");

    printf("Command: ");

    if (execution->command != NULL)
        printf("%s\n", execution->command);
    else
        printf("NONE\n");

    printf("Arguments: ");

    for (int i = 0;
         i < execution->argument_count;
         i++)
    {
        printf("[%s] ",
               execution->arguments[i]);
    }

    printf("\n");

    printf("Input file: ");

    if (execution->input_file != NULL)
        printf("%s\n", execution->input_file);
    else
        printf("NONE\n");

    printf("Output file: ");

    if (execution->output_file != NULL)
        printf("%s\n", execution->output_file);
    else
        printf("NONE\n");

    printf("Append mode: %s\n",
           execution->append_output ?
           "YES" : "NO");

    printf("===================================\n");
}

/* ==========================================
   FREE EXECUTION STRUCTURE
   ========================================== */

void free_execution_structure(
    ExecutionStructure *execution)
{
    if (execution->command != NULL)
        free(execution->command);

    for (int i = 0;
         i < execution->argument_count;
         i++)
    {
        free(execution->arguments[i]);
    }

    if (execution->input_file != NULL)
        free(execution->input_file);

    if (execution->output_file != NULL)
        free(execution->output_file);
}

/* ==========================================
   MAIN
   ========================================== */

int main()
{
    char input[MAX_INPUT];

    printf("============================================\n");
    printf("          SKILL 04 PARSER PROGRAM\n");
    printf("============================================\n");

    printf("\nThis program demonstrates:\n");
    printf("1. Tokenization\n");
    printf("2. Delimiter identification\n");
    printf("3. Whitespace handling\n");
    printf("4. Token validation\n");
    printf("5. Parse tree generation\n");
    printf("6. Syntax validation\n");
    printf("7. Error detection\n");
    printf("8. Empty command handling\n");
    printf("9. Execution structure generation\n");

    printf("\nEnter a command.\n");
    printf("Examples:\n");
    printf("  ls -l\n");
    printf("  cat file.txt | grep hello\n");
    printf("  cat < input.txt > output.txt\n");
    printf("  ls -l >> output.txt\n");
    printf("\nType 'exit' to quit.\n");

    while (1)
    {
        printf("\nskill04> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        /* Empty command */
        if (strlen(input) == 0)
        {
            printf("Empty command: nothing to parse.\n");
            continue;
        }

        /* Exit */
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Skill 04...\n");
            break;
        }

        Token tokens[MAX_TOKENS];

        int token_count =
            tokenize(input, tokens);

        /* Debug tokenizer */
        print_tokens(tokens, token_count);

        /* Validate token stream */
        if (!validate_tokens(tokens, token_count))
        {
            printf("Parsing failed due to syntax error.\n");
            continue;
        }

        printf("\nToken stream is valid.\n");

        /* Parse tree */
        ParseNode *root =
            generate_parse_tree(tokens,
                                token_count);

        printf("\n========== PARSE TREE ==========\n");

        print_tree(root, 0);

        printf("================================\n");

        /* Execution structure */
        ExecutionStructure execution;

        create_execution_structure(
            tokens,
            token_count,
            &execution);

        print_execution_structure(&execution);

        free_execution_structure(&execution);

        free_tree(root);

        printf("\nParsing completed successfully.\n");
    }

    return 0;
}
