# Skill 03 - Command History and Dynamic Memory Management

## Objective

To implement:

1. Escape sequence handling
2. Command history
3. Previous command navigation
4. Next command navigation
5. Input buffer updating
6. Command recall functionality
7. Dynamic buffer allocation
8. Dynamic buffer resizing
9. Buffer overflow prevention
10. Linked list management
11. Correct memory release
12. Memory verification using Valgrind

## Files

- `skill03.c` - Complete implementation
- `README.md` - Documentation
- `output.txt` - Sample execution and Valgrind output

## Compilation

```bash
gcc -Wall -Wextra -g skill03.c -o skill03

Execution

./skill03

Controls
UP ARROW - Recall previous command
DOWN ARROW - Move to next command
BACKSPACE - Delete character
ENTER - Execute/store command
Ctrl+D - Exit
history - Display stored command history
Dynamic Memory

The program uses:

malloc() for dynamic allocation
realloc() for increasing the input buffer
free() for releasing allocated memory

The input buffer starts at 32 bytes and doubles when more space is required.

Linked List

Command history is stored using a linked list.

Each node contains:

Dynamically allocated command string
Pointer to the next node

All nodes and command strings are released using free() before the program exits.

Escape Sequences

Arrow keys generate escape sequences.

For example:

UP arrow: ESC [ A
DOWN arrow: ESC [ B

The program reads these sequences and performs command navigation.

Buffer Overflow Prevention

Before adding a character, the program checks whether sufficient space is available.

If the buffer is full, realloc() is used to increase its size.

Therefore, the program does not write beyond the allocated buffer.

Valgrind

Memory leaks and invalid memory operations can be checked using:

valgrind --leak-check=full --show-leak-kinds=all ./skill03

A successful test should report:

All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors



