# Process relationships

## Introduction

* every process has a parent process
  * a parent is notified whenever a child terminates and can get its termination status
  * waitpid can specify to wait for a child of a certain process group
* process groups and sessions
* login process

## Terminal logins

* the system administrator creates a file `/etc/ttys` that has one line for each terminal device
  * each line specifies the name of the device and other params to be passed to the `getty` command
* when the system is bootstrapped
  * the kernel creates process ID 1, the `init` process
  * the system is brought to multiuser mode
  * `/etc/ttys` is read and for each terminal, a fork is performed followed by an exec to `getty`
* the `getty` process
  * opens the terminal devices and sets up the file descriptors 0, 1 and 2
  * prints "login: " and waits for the login username to be inserted
  * then execs to the `login` program passing the username
* all these processes since init have superuser priviledges
* the `login` program
  * fetches our password entry with `getpwnam`
  * then calls `getpass` to print "password: " and waits for our password
  * then it encrypts the inserted password using the `crypt` command and confronts it with the `pw_passwd` entry
  * if login fails a number of times, `login` calls exit(1)
* if we login correctly
  * chdir to our home directly
  * chown the terminal device
  * change permission to terminal device so that we can read from and write to it
  * set our group ID and additional group IDs
  * initialize the environment variables (HOME, SHELL, USER, PATH)
  * setuid to our ID (since it is set by a superuser all three user IDs are set)
  * execs to our login shell (the login shell is invoked prepended with a minus to tell that it is being invoked as a login shell)
  * many other stuff...
* our login shell is running and
  * its parent is init, so if we logout a SIGCHLD is sent and a new getty process is spawned
* on Linux
  * `/etc/inittab` is read

## Network logins

* a pseudo-terminal is used to simulate a real terminal
* in BSD
  * a single process waits for most network connections, the `inetd` process (Internet superserver)
  * as a part of system startup, `init` invokes a shell which executes `/etc/rc`; one of these daemons is inetd
  * inetd waits for TCP/IP connection requests; then it forks and execs the correct program
  * if the program supports login (telnet, ssh) it forks and execs `login`
* the important thing is that whether we are logged in from a terminal or from network
  * the login shell has stdin, stdout and stderr connected to a terminal device or a pseudo terminal device
* in Linux
  * instead of inetd, xinetd is used

## Process groups

* each process not only has a process ID, but has a process group aswell
* a process group is a collection of processes, generally associated to the same job
* `pid_t getprgrp(void)` returns the process group ID
* `pid_t getpgid(pid_t pid)` returns the process group ID of process with ID pid
* each process group may have a group leader, which process ID == process group ID
  * even if the group leader terminates, the process group goes on until the last process terminates
  * a child inherits the process group ID of the parent
* `int setpgid(pid_t pid, pid_t pgid)` can be used to join an existing group or creating a new one

## Sessions

* a session is a collection of one or more process groups
* example of session
  * proc1 and proc2 are in the same background process group
  * proc3 and proc4 are in the same foreground process group

  ```bash
  proc1 | proc2 &
  proc 3 | proc4
  ```
* `pid_t setsid(void)` can be used to establish a new session, if called by a process which is not a process group leader
  * the process becomes the session leader; it is the only process in the session
  * the process becomes the group leader of a new process group
  * the process has no controlling terminal
* if setsid is called by a process group leader, it terminates with an error
  * to avoid this, we can call fork and let the parent terminate:
    * we are sure the child inherits the process group ID of the parent
    * its process ID != its process group ID
* there is no session ID, setsid returns the process group ID of the session leader
* `pid_t getsid(pid_t pid)` returns the process group ID of the process 'pid' session leader

## Controlling terminals

* a session can have a single controlling terminal; this is the terminal device (or pseudo terminal) on which we login
* the session leader that connects to the controlling terminal is the controlling process
* the process groups in a session can be
  * a single foreground process group
  * many background process groups
* when we type the terminal interrupt key (Control-C) an interrupt signal is sent to all processes in the foregroud process group
* when we type the terminal quit key (Control-backslash) a quit signal is sent to all processes in the foregroud process group
* when the network disconnects a hangup signal is sent to the controlling process
* sometimes a process might want to talk directly to the controlling terminal:
  * it can do so by opening the `/dev/tty` file, which is a kernel synonym for the controlling terminal
  * this is useful when the stardard input is already being used or with the `getpass` function

## tcgetpgrp, tcsetpgrp and tcgetsid functions

* these methods tell the kernel which is the foreground process group, so that
  * the terminal device driver knows where to send the terminal input and the terminal generated signals
* these functions are normally called by job-control shells

## Job Control

* job control allows us to
  * start multiple processes from a single terminal
  * control which process goes to foreground and which to background
* job control requires
  * a shell that supports job control
  * a terminal driver in the kernel that supports job control
  * the kernel must support some job-control signals
* when we start a background job, the shell assigns to it a job identifier and prints a job ID
* when the background job has finished, we have to press enter and see the exit status:
  * we have to press enter to re-render the prompt, since the shell doesn't print the status of background jobs at any time
  * if it did, it could print stuff while we are typing
* we can bring a foreground process to background with the suspend key (Control-Z):
  * the terminal driver sends SIGTSTP to all processes in the foreground group
* the terminal driver looks for three special characters:
  * interrupt character (DELETE or Control-c) that generates SIGINT
  * quit character (Control-backslash) that generates SIGQUIT
  * suspend character (Control-z) that generates SIGTSTP
* only the foreground process group receives input from the terminal
* when a background process wants to read from the terminal, the terminal driver sends it a special signal (SIGTTIN)
  * the background process generally stops and we are notified by the shell

## Shell execution of programs

* in a shell which doesn't support job control
  * background jobs are not put in their own process groups
  * the controlling terminal isn't taken away from the background job
  * when executing multiple processes from a pipe
    * the last program is the direct child of the shell
    * the other programs are children of the last program
  * if we try to put into background a process which might want to read from stdin
    * unknown things may happen, two processes are trying to read from the same device
* in a shell which supports job control
  * launching ps from bash

    ```bash
    arkham@code ~ $ ps -o pid,ppid,pgrp,session,tpgid,comm
      PID  PPID  PGRP  SESS TPGID COMMAND
    14884 14883 14884 14884 14938 bash
    14938 14884 14938 14884 14938 ps
    ```

    * the shell puts the new process in a new process group (with PGRP ID equal to PID)
    * this new process is the foreground process group, since its PGRP ID is equal to TPGID
    * both bash and ps belong to the same session
  * if we put ps into the background

    ```bash
    arkham@code ~ $ ps -o pid,ppid,pgrp,session,tpgid,comm &
    [1] 15421
      PID  PPID  PGRP  SESS TPGID COMMAND
    14884 14883 14884 14884 14884 bash
    15421 14884 15421 14884 14884 ps
    ```

    * ps is still placed in a new process group, but the PGRP ID is no longer the foreground PGRP
    * ps in background, while bash is in foreground
  * if we use a pipe

    ```bash
    arkham@code ~ $ ps -o pid,ppid,pgrp,session,tpgid,comm | cat
      PID  PPID  PGRP  SESS TPGID COMMAND
    14884 14883 14884 14884 15817 bash
    15817 14884 15817 14884 15817 ps
    15818 14884 15817 14884 15817 cat
    ```

    * ps and cat belong to the same PGRP, which is the foreground PGRP
    * both programs are spawned by bash
  * if we use a pipe in the background

    ```bash
    arkham@code ~ $ ps -o pid,ppid,pgrp,session,tpgid,comm | cat &
    [1] 15863
      PID  PPID  PGRP  SESS TPGID COMMAND
    14884 14883 14884 14884 14884 bash
    15862 14884 15862 14884 14884 ps
    15863 14884 15862 14884 14884 cat
    ```

    * ps and cat still belong to the sam PGRP, but are in background
