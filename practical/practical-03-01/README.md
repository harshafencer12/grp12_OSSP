# Practical 03

## Aim

Develop a C program using fork() that creates a parent and child process. Display the Process ID (PID), Parent Process ID (PPID), and process states at different stages of execution.

## Files

- practical_03.c
- README.md

## Compilation

gcc practical_03.c -o practical_03

## Execution

./practical_03

## Commands to Observe Process States

ps -ef | grep practical_03

or

ps -o pid,ppid,state,cmd

## Result

The program successfully creates a parent and child process using fork(). The PID, PPID, and process states are observed during execution.
