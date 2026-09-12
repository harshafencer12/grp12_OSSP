# Skill 04 - Tokenization and Parser

## Objective

This skill implements a basic command parser.

### Part 1 - Tokenization

The program performs:

1. Input splitting into tokens
2. Delimiter identification
3. Whitespace handling
4. Token structure creation
5. Token stream validation
6. Debugging of parsing output

### Part 2 - Parser

The program performs:

1. Parser logic
2. Parse tree generation
3. Syntax validation
4. Syntax error detection
5. Empty command handling
6. Execution structure generation

## Files

- `skill04.c` - Complete C implementation
- `README.md` - Documentation
- `output.txt` - Sample output

## Compilation

```bash
gcc -Wall -Wextra -g skill04.c -o skill04


Execution

./skill04

Supported Tokens

| Token | Meaning                   |      |
| ----- | ------------------------- | ---- |
| WORD  | Command or argument       |      |
| `     | `                         | Pipe |
| `<`   | Input redirection         |      |
| `>`   | Output redirection        |      |
| `>>`  | Append output redirection |      |
| END   | End of token stream       |      |


Tokenization

The tokenizer scans the input character by character.

Whitespace is ignored.

Special characters such as:

|
<
>
>>

are identified as delimiters/operators.

Normal character sequences are stored as WORD tokens.

Token Validation

The token stream is checked for invalid syntax.

Examples:

| ls
ls |
ls >
These are detected as syntax errors.

Parse Tree

A parse tree is generated using dynamically allocated nodes.

A command is represented as:

COMMAND
 |
 +-- COMMAND_SEGMENT
       |
       +-- command
       +-- arguments

For a pipeline, multiple command segments are generated.

Example:cat file.txt | grep hello
produces two command segments.

Execution Structure

The parser produces:

Command
Arguments
Input file
Output file
Append mode

Example:cat < input.txt > output.txt

produces:

Command: cat
Arguments: [cat]
Input file: input.txt
Output file: output.txt
Append mode: NO


Empty Commands

If the user presses Enter without entering a command:

Empty command: nothing to parse.

The parser continues running.

Memory Management

The parse tree is dynamically allocated using malloc().

All parse tree nodes are released using free().

Execution structure strings are also dynamically allocated and released correctly.

Testing

Test cases:

ls -l
cat file.txt | grep hello
cat < input.txt > output.txt
ls -l >> output.txt

Error cases:

| ls
ls |
ls >

Empty command:

[Press Enter]


Conclusion

The program demonstrates lexical tokenization, delimiter recognition, token validation, syntax parsing, parse tree generation, error detection, and execution structure creation.
