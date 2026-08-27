# Scheduling Algorithms

## Objective

To implement and study different CPU scheduling algorithms in C.

## Algorithms Implemented

1. First Come First Serve (FCFS)
2. Shortest Job First (SJF) - Non-preemptive
3. Priority Scheduling
4. Round Robin

## 1. FCFS

First Come First Serve executes processes in the order in which they arrive.

The waiting time is calculated as:

WT = Previous Waiting Time + Previous Burst Time

Turnaround time:

TAT = WT + BT

Compile:

```bash
gcc fcfs.c -o fcfs

