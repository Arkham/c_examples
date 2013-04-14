# Process control

## Introduction

* process creation
* process execution
* process termination
* real, effective, saved IDs
* interpreter files and `system`
* process accounting on UNIX

## Process identifiers

* every process has an unique process ID, a non-negative integer
* although unique, process IDs can be reused:
  * whenever a program terminates, its ID can ben reused by another process
  * UNIX systems have some algorithms to delay reuse, so that a new process can't be mistaken with a freshly-closed one
* process ID 0 is usually the scheduler process (swapper), it's not a program on disk but it's a system process
* process ID 1 is usually the init process (`/sbin/init`), which is invoked by the kernel at the end of the bootstrap procedure
  * this process reads the initialization files, such as `/etc/rc*`, `/etc/inittab` and the files in `/etc/init.d/`
  * then brings up the system to a certain state, i.e. multiuser
  * this process never dies, and normally is a user process with superuser priviledges (unlike the swapper)
* useful functions:
  * `pid_t getpid(void)`
  * `pid_t getppid(void)`
  * `uid_t getuid(void)`
  * `uid_t geteuid(void)`
  * `gid_t getgid(void)`
  * `gid_t getegid(void)`

## fork function

* a process can create a new process with `pid_t fork(void)`
* the new process is called the *child* process
* this function is called once, but returns twice:
  * it returns 0 in the child
  * it returns the child pid in the parent process
* after fork, both processes continue executing:
  * the child gets a complete copy of the parent's data space, stack and heap
  * the child and the parent do not share any of those information
  * the only thing that is shared is the text segment
* generally the parent's data, stack and heap are not copied immediately after the fork, since many times it is followed by an `exec`
  * instead, copy-on-write is performed
  * those memory regions are marked as read-only by the kernel
  * if any of the processes tries to modify these regions, then the kernel performs the copy (generally, a new page)
* in the "fork.c" example
  * if we run it from the terminal, we only see "before fork" once
  * if we redirect its output to a file, wee see "before fork" twice
  * this is because the standard I/O is line buffered on the terminal, and fully buffered otherwise
  * the parent and the child both have the line in their I/O buffers, so when they are flushed, they print the string twice

## File sharing

* file descriptors open in the parent are duplicated in the child too
* after fork the both have a set of file descriptors pointing to the same entries in the kernel file table
* therefore, they share the same file offsets (it's much easier for both processes to write on the same files)
* while the output can be intermixed, generally after a fork:
  * the parent waits for the child to complete
  * both the parent and the child go their own ways, closing the descriptors they don't need.
  * In this way, they don't interfere with the other's open file descriptors
* other properties are inherited by the child
  * real user/group ID, effective user/group ID
  * supplementary group IDs
  * process group ID
  * session ID
  * terminal
  * set-user-ID and set-group-ID flgas
  * current working directory
  * root directory
  * umask
  * signal masks and dispositions
  * close-on-exec flag for any open file descriptor
  * environment
  * attached shared memory segments
  * memory mappings
  * resource limits
* what is different:
  * return values from fork()
  * process IDs
  * parent PIDs
  * file locks of the parent are not inherited
  * pending alarms are cleared for the child
  * the set of pending signals for the child is set to empty
* a fork can fail if:
  * too many processes are already in the system
  * total number of processes for this real user ID exceeds system's limit
* a fork is used when:
  * a process wants to duplicate itself so that the parent and the child can execute different sections of code at the same time
    * this is usual for network servers, the parent waits for connections; when it gets one, it forks and let the child handle the connection
  * a process wants to execute a different program.
    * this is common for shells, which use `exec` after forking
    * in some OS, the action fork+exec is called spawn

## vfork function

* vfork should be used when we want to exec right after forking
* child shares same address space as parent
* no copy is performed of the parent data structures
  * better than copy-on-write since no copying is better than a little copying
* vfork guarantee that the child will run first until it calls exec or exit

## exit functions

* a process can exit normally in five ways:
  * return from main
  * calling `exit`, launching all atexit handles and closing standard I/O streams
  * calling `_exit` or `_Exit`, ignoring all handlers
  * returning from the start routine of the last thread
  * calling `phtread_exit` from the last thread in the process
* a process can terminate abnormally in three ways:
  * calling `abort` (generates SIGABRT)
  * after receiving a signal
  * the last thread responds to a cancellation request
* regardlessly of how a process terminates, the kernel performs the same procedures:
  * it closes all open file descriptors
  * releases the memory used by the process and so on..
* parent must be notified of the child termination
  * when the process terminates normally, it returns an exit status
  * if it didn't, the kernel generates one to be sento to the parent process
* if the parent terminates before the child:
  * the child is inherited by init
* if the child terminates before the parent:
  * the kernel keeps some information about each terminating process, such as:
    * process ID, termination status, amount of CPU time
  * then the kernel can discard the process, it's open file des and free the memory
  * a child which parent has not yet waited for is called *zombie process*
* `ps` shows zombie processes as Z
* if a process inherited by init dies, it will never become a zombie because init always waits for its children

## wait and waitpid functions

* when a process terminates, the kernel notifies the parent by sending it the SIGCHLD signal
* the parent can choose to ignore this signal, which is the default, or can setup a signal handler to manage the event
* when a process calls `wait` or `waitpid` it can
  * block, if all its children are still running
  * return immediately with the termination status of a child, if a child has terminated and is waiting for its status to be fetched
  * return immediately with an error, if it doesn't have any child processes
* `pid_t wait(int *statloc)`
* `pid_t waitpid(pid_t pid, int *statloc, int options`
* wait can block the caller until a child process terminates, while waitpid has a non-blocking option
* waitpid function doesn't wait for the first child to terminate; it has a set of options to decide for which child wait for
* we can tell which child has terminated because its pid is returned by the wait functions
* the exit information of the child is saved in `statloc`, if it's not NULL
  * to read this info, four macros should be used
    * WIFEXITED(status);   if this is true, use WIFEXITSTATUS(status) to fetch the exit status
    * WIFSIGNALED(status); if this is true, use WTERMSIG(status) to fetch the signal
    * WIFSTOPPED(status);  if this is true, use WSTOPSIG(status) to fetch the signal which caused the child to stop
    * WIFCONTINUED(status)
* if we want for a specific child with wait:
  * we would have to wait for any child to terminate
  * if it's the one we're looking for, good job
  * otherwise, save the pid and status info in an array, and wait again
    * the next time we want to wait for a child, go through the list to see if the child has already been waited for
* the previous process can be simplified with `waitpid`:
  * if pid == -1, wait for any child
  * if pid > 0,   wait for child with process ID pid
  * if pid == 0,  wait for any child in same process group ID as calling process
  * if pid < -1,  wait for any child with process group ID abs(pid)
* waitpid supports some options:
  * WCONTINUED | WNOHANG | WUNTRACED
* waitpid is much more powerful than wait since:
  * it can wait for a specific child
  * it has a non blocking version
  * it supports job control
* to prevent zombie processes the trick is to call fork twice:
  * as soon as the first child is forked, fork again and exit
  * the second child will get inherited by init, so it will always be waited for

## waitid function

* `int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options)`

## wait3 and wait4 functions

* these functions allow to return a summary of all used resources by the child process, such as:
  * user CPU time
  * system CPU time
  * number of page faults
  * number of signals received

## Race conditions

* a race condition is when multiple processes try to do something with shared data and the result depends on the order in which they run
  * whenever we use fork, we are never sure if the child process or the parent process will run first
  * even if we sleep from inside a process, a high system load and the kernel scheduling algorithm could change the execution order
  * problems like this are difficult to debug because this code tend to work "most of the time"
* a process which wants to wait for its children to terminate must call one of the wait functions
* a process which wants to wait for its parent to terminate:
  * could try some sort of polling, such as:

    ```c
    while (waitpid() != pid) {
      sleep(1);
    }
    ```
  * this approach wastes CPU time and the caller is awakened every second to check
* otherwise a set of signals could be sent from the parent to the child and vice versa
  * so that the two processes can synchronize themselvers using a set of signals and waiting for them

## exec functions

* when a program calls one of the exec functions:
  * the existing program is completely replace by a new program
  * the new program starts executing its main function
  * the process ID does not change, since exec merely replaces the current process with new text, data, heap and stack segments
* six exec functions
  * `int execl(const char *pathname, const char *arg0, ..., /* (char *) 0 */ )`
  * `int execv(const char *pathname, char *const argv[])`
  * `int execle(const char *pathname, const char *arg0, ..., /* (char *) 0, char *const envp[] */`
  * `int execve(const char *pathname, char *const argv[], char *const envp[])`
  * `int execlp(const char *filename, const char *arg0, ..., /* (char *) 0 */)`
  * `int execvp(const char *filename, char *const argv[])`
* the first four take a path argument, while the latter two take a filename:
  * if the filename contains a slash, it is treated as a pathname
  * otherwise, we search the executable in the directories listed in the PATH environment variable
* if the filename passed to execlp and execvp is not executable, the function assumes it is a shell script and tries to invoke `/bin/sh` on it
* the execl functions require arguments to be passed separately (marking the end with a NULL pointer)
* the execp functions require arguments to be passed as a single array
* the execle and execve functions allow to the environment variables to be passed to the new program:
  * the other ones use the standard `environ` variable
* every system has a maximum number of command line arguments:
  * generally it is 4096 on POSIX.1 systems
  * we can acknowledge this limitation with shell expansion:
    * `grep spam /usr/share/*/*` can return "Argument list too long"
* if the file descriptor has the close-on-exec flags set, it is closed across an exec (generally it's left open)
* while the real user and group ID remain the same across an exec, the effective user ID and group ID may change:
  * if the new program is set-user-id, the effective IDs become the owner ID of the file
* generally there is a single system call (`execve`), the other five are just wrappers
* note that we can set arg0 as anything we like:
  * the `login` program does it to indicate to the shell that it is being invoked as a login shell

## Changing user IDs and group IDs

* in general, we use the _least-priviledge_ model when designing applications, aka
  * the programs should use the least amount of priviledges necessary to accomplish any task
  * so that malicious users are limited in tricking our application to overextend its priviledges
* we cant set the real user ID and the effective user ID with:
  * `int setuid(uid_t uid)`
  * `int setgid(gid_t gid)`
* when changing the user ID:
  * if the process has superuser priviledges:
    * setuid sets real ID, effective ID and saved-set-user-ID to uid
  * if the process does not have superuser priviledges:
    * if uid == real ID || uid == saved set-user-ID, then the effective user ID is set to uid; the other IDs are left unchanged
  * otherwise, errno is set to EPERM and -1 is returned
* only a superuser can change the real user ID; normally the real user ID is set by `login` when we login and it never changes
* the effective user ID is set by the exec functions only if the set-user-ID bit is set:
* at any time, the effective user ID may be set to the real user ID or the saved set-user-ID
* the saved set-user-ID is copied from the effective user ID by exec; if the program set-user-ID bit is set, this copy is performed *after* loading the file owner ID
* UNIX functions:
  * we can access the real user ID with `getuid`
  * we can access the effective use ID with `geteuid`
  * we cannot access the saved set-user-ID

## saved set-user-ID example

* the `man` utility display manpage information for programs, it is usually set-user-ID or set-group-ID
* it has to read files and run commands in order to show the manpages (or even writing to files sometimes)
* to prevent being tricked into running the wrong commands or touching the wrong files, the `man` program uses two sets of permissions:
  * those of the user running man
  * those of the user which owns the man executable
* the following steps take place
  * if the man executable is owned by the man user and has set-user-ID set, we have after the exec:
    * real user ID = user ID
    * effective user ID = man ID
    * saved set-user-ID = man ID
  * at this point, the man executable can access its configuration files and man pages. It also saves the effective user ID into `euid`
  * before running any command on our behalf, the man executable calls `setuid(getuid())`. Since we are not the superuser the effective user ID is set to the real user ID, so:
    * real user ID = user ID
    * effective user ID = user ID
    * saved set-user-id = man ID
    * at this point, it's running with our user ID so we can access the correct files and we can launch any filter
  * when the filter is done, the man executable can `setuid(euid)` to update the effective user ID to its own ID
  * the man program can now operate on its files
* this mechanism allows to the man program to start with its own priviledges, then switch to the real user ID and then revert to its own priviledges:
  * if during operation, man spawns a shell, it would have to priviledges of our user

### setreuid and setregid functions

* these functions allow to swap the real user ID and the effective user ID

### seteuid and setegid functions

* these functions allow to set the effective user ID:
  * for unpriviledged users, only values of real user ID or saved set-user-ID are allowed
  * for priviledged users, any value is permitted, but only effective user ID is set (while setuid sets all three)

## Interpreter files

* interpreter files are text files which begin with `#! pathname [ optional-argument ]`
* for example, many shell scripts start with `#! /bin/bash`
* the pathname must be an absolute, since PATH is not used

