# Threads

## Introduction

* how we can use multiple threads of control to perform multiple tasks within a single process
* all threads within the same process can access to some process components, such as file descriptors and memory
* everytime multiple agents access to the same data, we have to deal with _concurrency_ and _synchronization_

## Thread concepts

* a typical UNIX process has a single thread of control; each process is doing only one thing at a time
* being able to do more than one thing at a time can be useful
  * we can simplify code that deals with asynchronous events
    * we assign a separate thread to deal with each event type
    * synchronous code is much simpler than asyncronous code
  * multiple processes have to use complex mechanisms to share memory and file descriptors
    * threads share this data automatically
  * some problems can be partitioned so that overall throughput is improved
    * this can be performed if tasks do not depend on each other
  * interactive programs can separate the user interface (input and ouput) from the other parts of the program
* generally we associate threaded-programming to multiprocessor systems
  * although, a threaded approach may bring benefits even on a single processor system
    * the program structure doesn't depend on a the hardware architecture
    * blocking events are handled by some threads, while others keep on running
* a thread is composed of
  * a thread ID, which is unique within the same process
  * a set of register values
  * a stack
  * a scheduling priority
  * a policy
  * a signal mask
  * an errno variable
  * thread-specific data
* everything is a process is shareable among the threads, including text segment, global and heap memory, the stacks and the file descriptors

## Threads identification

* a PID (`pid_t`) is represented by a non-negative integer
* a thread ID is represented by a `pthread_t` data structure
  * to make them portable, they can't be treated as integers
* `int pthread_equal(pthread_t tid1, pthread_t tid2)` tells if two threads are equal
* `pthread_t pthread_self(void)` returns the thread own ID
* there is no portable way to print a thread ID
* these functions can be used by threads to identify data structures associated to their own IDs:
  * for example a single master and multiple slaves configuration, where
    * the master assign jobs to the slaves
    * the slave recognize which jobs are assigned to it

## Thread creation

```c
int pthread_t pthread_create(pthread_t *restrict tidp,
                             const pthread_attr_t *restrict attr,
                             void *(*start_rtn)(void *), void *restrict arg)
```
* the memory location pointed by `tidp` is set to the new thread ID
* the `attr` argument can be used to customize thread attributes (or NULL to go with the defaults)
* the newly created thread starts running at the address of the `start_rtn` function, which starts using `arg` as argument
  * if we need to pass more data, we use store them in a structure and pass the address as `arg`
* as with fork, when the new thread is created we have no guarantee on which thread runs first, the newly created one or the calling one
  * the new thread inherits the process address space, and the calling thread floating-point env and signal mask
  * the set of pending signals is cleared though
* pthread functions return an error when they fail (they do not use `errno`)
* in the example `printid.c`
  * we have to sleep from the main thread to deal with concurrency issues
  * the new thread has to get its thread ID by calling `pthread_self`
    * it can't use the global variable `ntid` since it can't be sure that the main thread has returned from the `pthread_create`

## Thread termination

* if any thread calls `exit, _exit or _Exit` the whole process terminates
* when a signal which default action is terminate is sent to a thread, the whole process will terminate
* a single thread can exit in three ways without terminating the whole process:
  * the thread can simply return from the start routine
  * the thread may be canceled by another thread in the same process
  * the thread can call `pthread_exit`
* `void pthread_exit(void *rval_ptr)`
  * the typeless `rval_ptr` pointer is available for other threads in the process by calling the `pthread_join` function
* `int pthread_join(pthread_t thread, void **rval_ptr)` will make the calling thread block until the thread with ID `thread` exits
  * if the other thread returned from its start routine, we will find the return value in `rval_ptr`
  * if it was canceled, `rval_ptr` will point to `PTHREAD_CANCELED`
  * by calling this function, we will place the other thread in detached state so that its termination status can be recovered
    * if the other thread was already in detached state, `pthread_join` can fail
  * we can also set `rval_ptr` to NULL if we don't care about the termination status of the thread
* the pointer used by `pthread_create` and `pthread_exit` can be used to store more than a single value
  * we have to be careful that the memory has to be valid after the caller has completed
  * for example, if we allocated dynamically a structure and pass it to `pthread_exit`, the stack might have been modified by the time the caller of `pthread_join` uses it
  * see example `pthread_stack.c`
* `int pthread_cancel(pthread_t tid)` can be used by a thread to request the cancellation of another thread of the same process
  * the other thread behaves as if it called `pthread_exit`
  * however, the other thread can ignore or control how it is canceled
  * this function does not wait for the termination of the other thread, it merely makes the request
* similarly to the atexit handlers, there are thread cleanup handlers:
  * `void pthread_cleanup_push(void (*rtn)(void *), void *arg)`, the rtn is the cleanup function to be called with the argument arg
    * this handler is run when the thread calls `pthread_exit`
    * or responds to a cancellation request
    * or calls `pthread_cleanup_pop` with a non-zero argument
  * `void pthread_cleanup_pop(int execute)`
  * in the `pthread_cleanup.c` example
    * we can see that only the handlers for the second thread are called
      * the first thread cleanup handlers are not called since it returns from its start routine
    * note also that the cleanup handlers are called in reverse order
* we can see a lot of similarities between process and thread control
  * `fork` -> `pthread_create`
  * `exit` -> `pthread_exit`
  * `waitpid` -> `pthread_join`
  * `atexit` -> `pthread_cleanup_push`
  * `getpid` -> `pthread_self`
  * `abort` -> `pthread_cancel`
* a thread termination status is retained until `pthread_join` is called for that thread
  * a thread underlying storage can be immediately reclaimed if it is on detached status
  * but if it is already detached, `pthread_join` can't be used to wait for its exit status, and generally the function call fails
  * `int pthread_detach(pthread_t tid)` can be used to detach a thread

## Thread syncronization

* since multiple threads share the same memory, some kind of synchronization is necessary
  * if each thread only modifies its own data, no problems arise
  * in the same way, if all threads access data read-only, there is no consistency problem
  * when a thread modifies data that other threads modify or read, that's when we need to ensure that they don't use an invalid value
* this can be troublesome on architectures where the modification takes more than one cycle
  * a thread start modifying some data
  * another thread reads data while the modification is in progress
  * it reads an inconsistent value
* therefore, the threads need to use a lock to ensure that only one thread can access to the variable at a time
* another case when the use of a lock is necessary is when two threads try to modify the same variable
  * in order to increment a value, we must
    * read the the memory location into a register
    * increment the value in the register
    * write the new value back to the memory location
  * the inconsistencies may also be caused if we read a value and then make decisions depending on the value that we've read

### Mutexes

* we can protect our data and ensure access by only one thread using the pthreads mutual-exclusion interface
* a mutex is a lock that we
  * *lock*, set before accessing a shared resource
    * when a mutex is locked, all threads that try to access the shared resource will be blocked
  * *unlock*, release when we are done
    * when the mutex is unlocked, all blocked threads will be made runnable and the first one to run will be able to set the lock
* a mutex is represented by a `pthread_mutex_t` data type; before using it we must call
  * `PTHREAD_MUTEX_INITIALIZER` for statically allocated mutexes
  * `int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)` for dynamically allocated ones
    * if we want to use the default attributes, we can set `attr` as NULL
* if we have allocated a mutex dynamically, we must call this before freeing the memory:
  * `int pthread_mutex_destroy(pthread_mutex_t *mutex)`
* the following functions can be used to interact with a mutex
  * `int pthread_mutex_lock(pthread_mutex_t *mutex)`
  * `int pthread_mutex_trylock(pthread_mutex_t *mutex)`, which can be used if a thread doesn't want to block
    * if the mutex is unlocked, lock is acquired and 0 returned
    * if the lock can't be acquired, EBUSY is returned
  * `int pthread_mutex_unlock(pthread_mutex_t *mutex)`
