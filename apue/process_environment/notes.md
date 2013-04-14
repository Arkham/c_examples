# Process environment

## Introduction

* how the main function is called
* how command line arguments are passed
* typical memory layout
* how to allocate additional memory
* how the process can use environment variables
* many ways process can terminate
* `setjmp, longjmp`
* resource limits of a process

## main function

* `int main(int argc, char *argv[])`
* when a program is run by the kernel (using exec)
  * a special start-up procedure is called before main
  * the program sets this routine as the starting address for the program
  * all this behaviour is arranged by the linker
  * this routine takes the command line arguments, the environment variables and sets things up for the main function

## Process termination

* a process can terminate normally in five ways:
  * return from main
  * `exit`
  * `_exit` or `_Exit`
  * return of last thread from its start routine
  * `pthread_exit`
* a process can terminate abnormally in three ways:
  * `abort`
  * receipt of a signal
  * response of the last thread to a cancellation request
* the start-up routine calls `exit` when we return from the main function

## Exit functions

* the `exit` function closes all standard I/O streams, which causes buffers to flush
* exit functions expect a status integer
* returning a value from the main is the same as calling exit with that value

## atexit function

* a process can register up to 32 functions to be automatically called by exit (*exit handlers*)
* `int atexit(void (*func) (void))`
* exit calls these functions in reverse order
* if a function is registered twice, it will be called twice
* the only way for the kernel to execute a program is `exec`
* a program may terminate directly to the kernel voluntarily with `_exit` and `_Exit`

## Command line arguments

* when a program executes, the process which calls `exec` passes along command-line arguments
* argv[argc] is a NULL pointer

## Environment list

* each program is passed an environment list, an array of null-terminated strings
* `extern char **environ` contains the environment list

## Memory layout of a C program

* a C program is composed of:
  * a text segment, which are the instructions the CPU executes. Usually it is shareable and read-only
  * initialized data segment, containing data specifically initialized outside functions
  * uninitialized data segment (bss), containing data declared outside functions but not initialized
  * stack, where automatic variables are stored, along with function calls data.
    * each time a function is called, the return address is saved on the stack along with some register values
    * the called function allocates its variables on the stack
    * each time a function is called, its stack does not interfere with other instances of the same function (i.e. recursive calls)
  * heap, where dynamic memoty allocation takes place
* typically the arrangement is:
  * text
  * initialized data
  * uninitialized data
  * heap
  * stack (generally growing from high address to low address)
* `size` shows a program size in bytes of text, initialized data and bss segments.
