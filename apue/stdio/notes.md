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

## Opening a stream

* `fopen(*pathname, *type)` opens a file
* `freopen(*pathname, *type, *fp)` opens a file on a defined stream
  * it closes it if it's open
  * it resets its orientation
  * used for opening a file as a predefined stream such as stdin, stdout or stderr
* `fdopen(filedes, *type)` opens a file descriptor on a stream
  * useful for opening pipes and network channels
* type may be
  * r: open for reading
  * w: truncate to 0 or create for writing
  * a: append, open for writing at the end file
  * r+: open for reading and writing
  * w+: truncate to 0 or create for reading and writing
  * a+: open or create for reading and writing at the end of file
* `fclose(*fp)` closes the stream and flushes the buffers

## Reading and writing a stream

* three types of unformatted I/O:
  * character at a time I/O, `getc, fgetc, getchar`
  * line at a time I/O `fputs, fgets`
  * direct I/O `fread, fwrite`

* getchar == fgetc(stdin)
* getc can be implemented as a macro, fgetc is guaranteed to be a function.
  * so getc argument should not be an expression with side effects
  * we can extract fgetc function address
* in case of error, a negative value is returned: to distinguish error from eof:
  * `ferror(*fp)`
  * `feof(*fp)`
* `ungetc(c, *fp)` pushes back character c
* `putc, fputc, putchar`

## Line at a time I/O

* `fgets(*buf, n, *fp)` lets us specify the size of the receiving buffer
* `gets(*buf)` should never be used
* `fputs(*str, *buf)`
* `puts(*str)` not unsafe as gets

## Standard I/O efficiency

* when using stdio we don't need to worry about choosing the right size of buffers
* system calls are generally much more expensive than ordinary function calls
* very useful routines, which do not cause a huge loss of performance

## Binary I/O

* `fread(*ptr, size, nobj, *fp)`
* `fwrite(*ptr, size, nobj, *fp)`
* useful for:
  * read or write a binary array: `fwrite(data[2], sizeof(float), 4, fp)`
  * read or write a structure
* binary I/O functions may be used only on the same system, since
  * offset of member within a structure can differ between compilers, due to different alignment requirements
  * binary formats used to store multibyte int and float differ among machine architectures

## Positioning a string

* three ways to position a standard I/O stream:
  * `ftell, fseek` using long integers
  * `ftello, fseeko` using `off_t`
  * `fgetpos, fsetpos` using `fpos_t` *portable, preferred*
* example signatures:
  * `ftell(*fp)`
  * `fseek(*fp, offset, whence)`
  * `rewind(*fp)`

## Formatted I/O

* `printf(format, ...)`
* `fprintf(*fp, format, ...)`
* `sprintf(*buf, format, ...)`
* `snprintf(*buf, size, format, ...)`
* format structure:
  * flags
  * fieldwidth
  * precision
  * lenmodifier
  * convtype

## Formatted input

* `scanf(format, ...)`
* `fscanf(*fp, format, ...)`
* `sscanf(*buf, format, ...)`

## Implementation details

* each stream has an associated file descriptor, `fileno(*fp)`
* useful for calling dup or fcntl

## Temporary files

* `char *tmpnam(*ptr)`
* `FILE *tmpfile(void)`
* tmpfile generally calls tmpnam, creates a file with mode w+, then unlinks it
* `char *tempnam(*directory, *prefix)`
* `int mkstemp(*template` similar to tmpfile, but returns a file descriptor instead of a stream

