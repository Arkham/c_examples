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

