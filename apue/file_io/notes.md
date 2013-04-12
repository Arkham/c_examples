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

