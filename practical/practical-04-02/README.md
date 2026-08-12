


## Objective

To create a scenario where a child process becomes a zombie process, investigate the process table, and modify the program to eliminate zombie processes using proper synchronization techniques.

## Part 1 - Creating a Zombie Process

The program `zombie.c` creates a child process using `fork()`.

The child process terminates using `exit(0)`, while the parent process does not call `wait()` or `waitpid()`.

The parent sleeps for 120 seconds, allowing the terminated child to remain in the process table as a zombie.

### Compilation

```bash
gcc zombie.c -o zombie
./zombie

Process Table Investigation

The process table was investigated using the ps command.

Command:ps -p 599 -o pid,ppid,state,cmd

Observed output:

PID    PPID S CMD
599    598 Z [zombie] <defunct>

Observation

PID 599 is the child process and PID 598 is its parent process.

The state Z indicates that the child process is in the zombie state.

The <defunct> status indicates that the child process has terminated but its parent has not yet collected its exit status.

Thus, a zombie process was successfully created and observed.



Part 2: Eliminating the Zombie Process

To eliminate the zombie process, the program was modified to use the waitpid() system call.

The parent process uses the following statement:
waitpid(pid, &status, 0);
This allows the parent to wait for the child process and collect its termination status.
Compilation

gcc zombie_fixed.c -o zombie_fixed
./zombie_fixed

Parent process:
PID = <parent PID>
Child PID = <child PID>
Parent waiting for child...

Child process:
PID = <child PID>
PPID = <parent PID>
Child exiting...

Child has been successfully reaped.
Parent exiting...

Verification

The process table was checked using:

ps -o pid,ppid,state,cmd | grep '[z]ombie_fixed'

No zombie process remained after the child process was reaped.

Conclusion

A zombie process was successfully created by allowing a child process to terminate without the parent collecting its exit status.

The zombie process was identified using the ps command, where its state was shown as Z and it was displayed as <defunct>.

The program was then modified to use waitpid(). The parent process properly synchronized with the child and collected its termination status.

Therefore, the zombie process was successfully eliminated using proper parent-child synchronization.
