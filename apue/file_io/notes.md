# File I/O

## Introduction

* main functions: open, read, write, lseek, close
* unbuffered I/O functions, each read or write invokes a system call in the kernel
* how files are shared amongst multiple processes
* dup, fcntl, sync, fsync, ioctl

## File descriptors

* all open files are referred to by file descriptors
* 0 -> stdin, 1 -> stdout, 2 -> stderr
* maximum number of open files: `OPEN_MAX`

## open function

* open(path, flags, (mode))
* flags: one of `O_RDONLY`, `O_WRONLY`, `O_RDWR`
* others may be combination of `O_APPEND`, `O_CREAT`, `O_EXCL`, `O_TRUNC`, `O_NOCTTY`, `O_NONBLOCK`, `O_DSYNC`, `O_SYNC`, `O_RSYNC`

## creat function

* creat(path, mode)
* equal to `open(path, O_WRONLY | O_CREAT | O_TRUNC, mode)`

## close function

* closing a file releases locks that processes may have on the file
* when a process terminates, all its open files are closed automatically by the kernel

## lseek function

* every open file has a current file offset
* lseek(filedes, offset, whence)
* can only seek files, can't seek pipes, fifo or sockets
* a file's offset can extend over its current length: hole in the file!

## read function

* `read(filedes, *buf, nbytes)`
* reads nbytes at max and increases offset
* could read less when EOF, tty, network, pipes or signals interrupt it

## write function

* `write(filedes, *buf, nbytes)`
* could write less if exceeded disk quota or file max size limit
* if file was openede with `O_APPEND`, file is seeked to end before each write

## I/O Efficiency

* many file systems read ahead for performance

## File sharing

* Every process has an entry in the process table; within each entry, there is a list of open file descriptors
* Every file descriptor has
  * file descriptor flags (close on exec)
  * a pointer to the file table entry
* The kernel maintains a file table of all open files
* Each file table entry has:
  * file status flags ( read, write, append, sync, nonblock )
  * file current offset
  * a pointer to the vnode table entry
* Each vnode entry has:
  * inode information
    * owner of file
    * size of file
    * pointers to which actual data blocks contain the file
* lseek does not cause I/O, just updates current offset

## Atomic operations

* multiple process writing on the same file should always open with `O_APPEND`, otherwise they can overwrite each other
* pread and pwrite functions do it atomically, they lseek and read/write but cannot be interrupted
* file creation should use `O_CREAT | O_EXCL` to do it atomically

## dup and dup2 functions

* `dup(filedes)`
* `dup2(filedes, filedes2)`
* duplicate file descriptors (in the process table entry), new filedes with lowest number
* all file table data is the same, file status flags, offset, etc
* dup2 is an atomic operation
  * if descriptors are different, it closes filedes2 if it's open, then duplicates filedes into filedes2
  * if they're equal, filedes2 is not closed
* dup is equal to: `fcntl(filedes, F_DUPFD, 0)`

## sync, fsync, fdatasync

* delayed write: UNIX kernels use a buffer/page cache through which disk I/O passes
* these functions ensure consistency with disks
* `sync(void)` queues modified blocks for read/write; does not wait; is generally launched by a daemon every N seconds
* `fsync(filedes)` waits for disk to complete before returning, i.e. useful for databases
* `fdatasync(filedes)` affects only data portions of a file

## fcntl function

* `fcntl(filedes, cmd, (arg))`
* cmd behaviour
  * `F_DUPFD` duplicate existing file descriptor
  * `F_GETFD, F_SETFD` get or set file descriptor flags, i.e. `FD_CLOEXEC`
  * `F_GETFL, F_SETFL` get or set file status flags
  * `F_GETOWN, F_SETOWN` get or set async I/O ownership
  * `F_GETLK, F_SETLK, F_SETLKW` get or set record locks
