# Standard I/O Library

## Introduction

* standard I/O handle details like
  * buffer allocation
  * I/O in optimal-sized chunks

## Streams and FILE objects

* other I/O functions (open, write, read) use file descriptors, standard I/O uses streams
* in ASCII, a character is represented by a single byte
* in international character sets a single character may be composed of multiple bytes (multibyte, wide)
* a stream's orientation decides if chars are single byte or multi-byte
* `fwide(*fp, int mode)` can change the orientation of a stream

## Standard input, output and error

* three streams are automatically available, `stdin, stdout & stderr`

## Buffering

* the buffering provided by the standard I/O library is to optimize the number of `read` and `write` calls
* _flush_ means writing of a standard I/O buffer
* three types of buffering are provided:
  * fully buffered
    * I/O takes place when standard I/O buffers are filled
    * files on disk are normally fully buffered
  * line buffered
    * I/O takes place when a newline is found in input or output
    * this kind of buffering is used on terminals for stdin and stdout
    * since the size of the buffer is fixed, I/O may take place before a newline is encountered
    * whenever input is requested through stdio from an unbuffered stream or a line-buffered stream, all line-buffered output buffers are flushed
  * unbuffered
    * standard I/O does not buffer characters
    * the standard error stream is generally unbuffered
* `setbuf(*fp, *buf)` sets a buffer or disables buffering with NULL
* `setvbuf(*fp, *buf, mode, size)` sets a specified buffering mode
* `fflush(*fp)` forces any unwritten data to be passed to the kernel; if NULL is passed, every output stream is flushed



