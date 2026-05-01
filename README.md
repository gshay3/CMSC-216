📘 CMSC-216 Systems Programming Projects Collection

This repository contains a series of C and assembly programming projects completed for CMSC 216 (Introduction to Computer Systems) at the University of Maryland.

Each project demonstrates low-level programming concepts including memory management, data structures, assembly language, process control, multithreading, synchronization, and systems-level design. <br>
 <br> <br>
📂 Projects Overview

🔹 Project 1: Intro – ASCII to Binary Converter
A foundational C program that converts text input into its binary (ASCII) representation.

Features:

Reads user input as a full string <br>
Converts each character into an 8-bit binary value <br>
Formats output for readability <br>
Handles spacing and line formatting <br>

Concepts Covered:

ASCII encoding <br>
String processing <br>
Loops and arrays <br>
Basic input/output (fgets, printf) <br>
 <br> <br>
 🔹 Project 2: Translate (Custom Language Interpreter)
A program that parses and translates input from a custom high-level language into a lower-level representation.

Features:

Reads and interprets structured input commands <br>
Tokenizes and parses input into meaningful components <br>
Translates commands into an output format or intermediate representation <br>
Handles syntax validation and error reporting <br>
Supports multiple instruction types and control flow constructs <br>

Concepts Covered:

Parsing and tokenization <br>
Interpreters and translation systems <br>
String manipulation <br>
Control flow and logic handling <br>
Modular program design <br>
 <br> <br>
🔹 Project 3: Dungeon Game (Graph-Based Adventure)
An interactive dungeon exploration game using graph structures and pathfinding algorithms.

Features:

Models the dungeon as a graph with rooms and connections <br>
Uses adjacency lists for dynamic structure representation <br>
Implements shortest path logic for gameplay mechanics <br>
Tracks both player and dragon movement <br>
Includes interactive commands and multiple endings <br>
Provides distance-based hints (e.g., warmer/colder system) <br>

Concepts Covered:

Graph data structures <br>
Dynamic memory allocation (malloc, free) <br>
Shortest path algorithms <br>
Structs and pointers <br>
Game state management <br>
 <br> <br>
🔹 Project 4: Reverse Polish Notation (RPN) Evaluator
A Y86-64 assembly program that evaluates postfix expressions using a stack-based approach.

Features:

Parses and evaluates RPN expressions <br>
Supports arithmetic operations (+, -, *, /, %) <br>
Implements stack behavior manually in assembly <br>
Includes helper routines for string handling <br>
Handles edge cases such as invalid input or division by zero <br>

Concepts Covered:

Assembly programming (Y86-64) <br>
Stack-based computation <br>
Low-level arithmetic operations <br>
Function calls and memory usage <br>
 <br> <br>
🔹 Project 5: Tiny Shell with Job Control
A Unix-style shell implemented in C that supports job control and process management.

Features:

Executes commands using fork and exec <br>
Supports foreground and background execution <br>
Implements built-in commands (quit, jobs, bg, fg) <br>
Handles signals (SIGINT, SIGTSTP, SIGCHLD) <br>
Maintains a job list with process states <br>
Prevents race conditions using signal blocking <br>

Concepts Covered:

Process control and system calls <br>
Signal handling <br>
Job scheduling and management <br>
Concurrency and synchronization <br>
Unix/Linux systems programming <br>
 <br> <br>
🔹 Project 6: Thread-Safe Data Structures
A multithreaded program that ensures safe concurrent access to shared data structures.

Features:

Implements a shared data structure (e.g., linked list or buffer) <br>
Supports concurrent read and write operations <br>
Uses mutexes and/or read-write locks for synchronization <br>
Prevents race conditions and deadlocks <br>
Includes thread creation, execution, and cleanup <br>

Concepts Covered:

Multithreading (pthreads) <br>
Synchronization primitives (mutexes, locks) <br>
Thread safety <br>
Concurrent programming design <br>
