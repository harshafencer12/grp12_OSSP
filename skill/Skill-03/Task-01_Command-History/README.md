# Skill-03 Task-01: Command History

## Objective

To implement command history functionality by applying escape sequences, storing commands, navigating previous and next commands, updating the input buffer, and testing command recall.

## Features

- Accept keyboard input
- Detect Enter key
- Handle Backspace
- Detect escape sequences
- Store commands in history
- Navigate previous commands using the Up arrow
- Navigate next commands using the Down arrow
- Update the input buffer
- Support multiple commands
- Exit using `exit` or Ctrl+D

## Escape Sequences

The terminal sends escape sequences for special keys.

The Up arrow is represented by:

```text
ESC [ A
