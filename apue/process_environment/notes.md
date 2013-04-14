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
  * command line arguments and environment variables
* `size` shows a program size in bytes of text, initialized data and bss segments.

## Shared libraries

* shared libraries remove common library routines from the process, and instead reference to a single copy of the routine
* reduces the program size, but can add some overhead
* moreover, when the library is updated (and the API hasn't changed) we don't have to relink the program
* shared:
  * gcc shared.c -Wall
  * ls -la a.out
      -rwxr-xr-x  1 arkham users 4.9K Apr 14 12:16 a.out*
* shared disabled:
  * gcc shared.c -static -Wall
  * ls -la a.out
      -rwxr-xr-x 1 arkham users 753K Apr 14 12:16 a.out*

## Memory allocation

* `malloc(size_t size)` allocates size bytes, with undefined initial value
* `calloc(size_t nobj, size_t size)` allocates nobj * size bytes, initialized to 0
* `realloc(void *ptr, size_t newsize)` increases/decreases the previously allocated area
  * when the size increases, this may involve moving the previously allocated area somewhere else
  * also, the newly allocated space is not initialized
* `free(void *ptr)` the space pointed by ptr is deallocated
* the pointer returned by the alloc functions is guaranteed to be aligned correctly
* since they return void * pointers we dont' have to manually cast them
* allocation routines are implemented with the `sbrk` system call, which expands the heap of the process
* even when memory is freed, generally the program keeps it in its memory pool
* potential problems:
  * when writing past the end of allocated area:
    * we could overwrite the data that is used for internal bookeeping
    * we could overwrite other dynamic data
  * freeing an already freed object
  * freeing a not malloced pointer
  * malloc with no free, *memory leak*

## Alternate memory allocators

* libmalloc
* vmalloc
* quick-fit
* alloca

## Environment variables

* environment strings are in the form `name=value`
* `char *getenv(const char *name)`
* `HOME, LANG, LC_ALL, SHELL, TERM`
* we cannot usually affect the environment variables of the parent process
* `int putenv(char *str)` takes a "name=value" string and saves it
* `int setenv(const char *name, const char *value, int rewrite)` sets name to value, rewriting if necessary
* `int unsetenv(const char *name)` removes the definition for name
* generally if we want to add new variables
  * since the environment is generally the last element and the stack grows from higher address to lower
  * we have to allocate space on the heap copy the pointers in the environment location

## setjmp and longjmp functions

* in C, we can't goto a label in another function
* setjmp and longjmp are used to handle error conditions in deeply nested function calls
* they use a shared `mp_buf` envobject, usually a global variable
* `int setjmp(jmp_buf env)` is called wherever you want to jump
* `void longjmp(longjmp env, int val)` is called to jump to the location env, making setjmp return val
* `longjmp` makes the stack go back and discard all stack frames
* when going back, the automatic variables may or may not be recovered
  * using `volatile` keyword makes the compiler leaves alone variables

## Potential automatic variable problems

* given this function:
    FILE *open_data
    {
      FILE *fp;
      char buf[BUFSIZE];

      if ( (fp = fopen("spam.txt", "r")) == NULL)
        return NULL;

      if (setvbuf(fp, buf, _IOLBF, BUFSIZE) != 0)
        return NULL;

      return fp;
    }
* when the function returns, the space it uses for the buffer gets thrown away, because its stack frame is discarded
* so either:
  * use extern or static keywords
  * use dynamic allocation

## getrlimit and setrlimit functions

* `int getrlimit(int resource, struct rlimit *rlptr)`
* `int setrlimit(int resource, const struct rlimit *rlptr)`
* each process has a set of resource limits, described by structures like
      struct rlimit {
        rlim_t rlim_cur; /* soft limit: current value */
        rlim_t rlim_max; /* hard limit: maximum value */
      }
* you can change soft limit <= hard limit
* you can change hard limit >= soft limit; irreversible for normal users
* superuser can raise hard limit
* limit can be infinite `RLIMIT_INIFINITY`
* limit examples: available memory, file size, cpu time, data, locks, number of files, child processes, sockets, stack size.
