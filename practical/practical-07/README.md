# Practical 07 - Linux Process Memory Analysis

## Objective

This practical studies the Linux process address space and virtual memory mappings.

### Part 1

Write a C program that prints the addresses of:

- Code
- Global variables
- Static variables
- Heap
- Stack

### Part 2

Use `/proc/<PID>/maps` and memory-analysis tools such as `pmap` to study the virtual memory organization of a running process.

---

## Files

- `memory_layout.c` - Displays addresses of different memory segments.
- `memory_maps.c` - Reads and displays `/proc/<PID>/maps`.
- `README.md` - Practical documentation.
- `output.txt` - Sample output.

---

# Part 1 - Memory Layout

Compile:

```bash
gcc -Wall -Wextra -g memory_layout.c -o memory_layout

Run: ./memory_layout

The program displays the addresses of:

Code Segment

Contains the machine instructions of the program.

A function address is used to demonstrate the code segment.

Global Variables

Global variables have program-wide scope and are stored in the program's data-related memory area.

Static Variables

Static variables have static storage duration and are stored in the program's data-related memory area.

Heap

The heap is used for dynamic memory allocation.

The program uses: malloc()
to allocate heap memory and:

free() to release it.

Stack

The stack stores local variables and function call information.

A local variable inside main() is used to demonstrate the stack.

Part 2 - /proc/PID/maps

Linux exposes information about a running process through the /proc virtual filesystem.

The file:/proc/<PID>/maps

contains the virtual memory mappings of the process.

For example: cat /proc/<PID>/maps

The mapping format is: START-END PERMISSIONS OFFSET DEVICE INODE PATH

Permissions
r - Read
w - Write
x - Execute
p - Private
s - Shared
Important Mappings
Program Code

The executable code is normally mapped with read and execute permissions:

r-xp

Data

Writable program data is normally mapped with:

rw-p
Heap

The mapping marked:

[heap]

represents dynamically allocated process memory.

Stack

The mapping marked:

[stack]

represents the process stack.

Shared Libraries

Libraries such as libc.so.6 are mapped into the process's virtual address space.

VDSO

The [vdso] mapping is a kernel-provided virtual shared object that allows certain operations to be performed efficiently from user space.

pmap Analysis

The pmap command can be used to inspect a process's memory mappings.

pmap <PID>

For extended information:

pmap -x <PID>

This provides information such as:

Virtual memory size
Resident memory
Dirty memory
Permissions
Mapping names

Linux Process Address Space

A simplified process address space can be represented as:
Higher Addresses
+----------------------+
|        Stack         |
|          ↓           |
+----------------------+
|                      |
|   Shared Libraries   |
|                      |
+----------------------+
|          ↑           |
|         Heap         |
+----------------------+
|        BSS/Data      |
+----------------------+
|        Code          |
+----------------------+
Lower Addresses

The exact layout and addresses depend on the architecture, operating system configuration, executable type, and security mechanisms such as ASLR.

Virtual Memory

Each Linux process normally operates in its own virtual address space.

Virtual addresses are translated to physical memory by the operating system and hardware memory-management mechanisms.

Different processes can therefore use similar virtual address ranges while referring to different physical memory.

Memory mappings also provide protection by controlling whether regions can be read, written, or executed.

Conclusion

The practical demonstrates the organization of a Linux process's virtual address space.

The addresses of code, global/static variables, heap, and stack were observed using a C program.

The /proc/<PID>/maps file and pmap were used to inspect virtual memory mappings of a running process.

This provides practical understanding of Linux process memory organization and virtual memory.
