# SIGNALS

## Introduction

* signals are software interrupts, which provide a way to deal with asynchronous events, i.e.
  * a user typing the interrupt key
  * the next program in a pipeline terminating

## Signal concepts

* every signal has a name and starts with SIG
  * SIGABRT is the abort signal that is generated when the process calls the `abort` function
  * SIGALRM is the alarm signal that is generated when the timer set by the `alarm` function goes off
  * there are a lot of different signals (30+) which are defined in `<signal.h>`
* each signal is defined by a positive integer, no signal is associated to 0 (null signal)
* a signal may be caused by
  * terminal signals, like SIGINT
  * hardware exceptions, like division by 0, invalid memory reference (SIGSEGV)
  * the `kill` function allows a process to send any signal to another process or process group
  * the `kill` command allows us to send signals to other processes, just an interface
  * software conditions can generate signals when certain conditions happen, SIGPIPE (when a process writes to a pipe after the reader of the pipe has terminated)
* signals are classic examples of asynchronous events
* we can tell the kernel to do one of three things when a signal occurs:
  * ignore the signal. This works for most of the signals except for SIGKILL and SIGSTOP, since the kernel must maintain a way to kill processes
  * catch the signal. To do this, we must tell the kernel to call a function when a signal occurs:
    * we may want to do this to allow users to press Control-C in a shell
    * or to catch SIGCHLD and handle the termination of a child process
    * or to catch SIGTERM to clean all temporary files before terminating
  * let the defaults apply. Every signal has a default behaviour; generally it is to terminate the process
* some signals may cause the creation of a core file, which can be opened by a debugger to inspect the cause of the signal
* most important signals
  * `SIGABRT`: `abort` function, the process terminates abnormally
  * `SIGALRM`: generated when a timer set with the `alarm` function expires (also `setitimer` causes this)
  * `SIGCHLD`: when a process terminates or stops, this is sent to its parent; by default, this signal is ignored
    * usually inside the signal handler wait is called to fetch the child termination status
