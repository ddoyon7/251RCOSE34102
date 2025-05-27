## CPU Scheduling Simulator

This project is a **CPU Scheduling Simulator** developed as part of the **Operating Systems course(	COSE341 - 02 ) at Korea University**.  
It demonstrates the behavior of multiple CPU scheduling algorithms through a command-line interface (CLI), designed for educational use and performance analysis.

## File Structure
├── main.c # Entry point, user interface

├── process.c # Process creation and management

├── queue.c # Queue operations (ready/waiting)

├── scheduler.c # Scheduler implementations

├── process.h # Process structure definition

├── queue.h # Queue structure and prototypes

├── scheduler.h # Scheduling algorithm prototypes

├── Makefile # Compilation instructions

## Scheduling Algorithms Implemented

The simulator supports the following scheduling algorithms:

| Type            | Algorithm                       |
|-----------------|---------------------------------|
| Non-preemptive  | FCFS (First-Come, First-Served) |
|                 | SJF (Shortest Job First)        |
|                 | Priority Scheduling             |
| Preemptive      | Preemptive SJF                  |
|                 | Preemptive Priority Scheduling  |
| Time-sliced     | Round-Robin                     |
| Real-time       | Rate-Monotonic                  |
|                 | EDF (Earliest Deadline First)   |

Each algorithm (FCFS, SJF, Priority, Preemptive SJF, Preemptive Priority, Round-Robin) simulates context switches, waiting times, turnaround times and process execution order.
The real-time scheduling algorithm (Rate-Monotonic, EDF) schedules only two processes, both of which arrive at time 0. It is assumed that there are no I/O requests. The output only indicates whether the scheduling was successful.

## Build & Run Instructions

To build and run the simulator, run:

```bash
git clone https://github.com/ddoyon7/251RCOSE34102.git
cd 251RCOSE34102
make
./simulator
