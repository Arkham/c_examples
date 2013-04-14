# System Data Files and Information

## Introduction

* portable interfaces to interact with `/etc/passwd` and `/etc/group`

## Password file

* `ls -la /etc/passwd`
    -rw-r--r-- 1 root root 1.3K Feb 17 01:39 /etc/passwd
* `head -7 /etc/passwd`
    root:x:0:0:root:/root:/bin/bash
    bin:x:1:1:bin:/bin:/bin/false
    daemon:x:2:2:daemon:/sbin:/bin/false
    mail:x:8:12:mail:/var/spool/mail:/bin/false
    ftp:x:14:11:ftp:/srv/ftp:/bin/false
    http:x:33:33:http:/srv/http:/bin/false
    nobody:x:99:99:nobody:/:/bin/false
* user:password:uid:gid:comment:home:shell
* root has uid 0
* encrypted passwords aren't stored in this file anymore, since it is word-readable
* for system or daemon users who shouldn't be logging in, `/bin/false, /bin/true or /dev/null` are used as shells
* `vipw` command for safe editing
* `struct passwd *getpwuid(uid_t uid)`, used by ls to map the uid in the inode to the user login name
* `struct passwd *getpwnam(const char *name)`, used by the login program
* if we want to go through the whole file:
  * `struct passwd *getpwent(void)`
  * `void setpwent(void)`
  * `void endpwent(void)`

## Shadow passwords

* `ls -la /etc/shadow`
    -rw------- 1 root root 755 Mar 19 08:41 /etc/shadow
* file not readable by world, only few programs need it (i.e. login and passwd)
* mandatory fields: username and encrypted password
* `struct spwd *getspnam(const char *name)`
* `struct spwd *getspent(void), void setspent(void), void endspent(void)`
* no shadow file on Mac OS

## Group file

* `head /etc/group`
    root:x:0:root
    bin:x:1:root,bin,daemon
    daemon:x:2:root,bin,daemon
    sys:x:3:root,bin
    adm:x:4:root,daemon
    tty:x:5:
    disk:x:6:root
    lp:x:7:daemon,pippo,pluto
    mem:x:8:
    kmem:x:9
* group:password:gid:userlist
* `struct group *getgrgid(gid_t *gid)`
* `struct group *getgrnam(const char *name)`
* `struct group *getgrent(void), void setgrent(void), void endgrent(void)`

## Supplementary group ids

* initially a user had a single group and had to use `newgrp` to switch groups
* `int getgroups(int gidsetsize, gid_t grouplist[])` fills the array grouplist up to gidsetsize elements
* `int setgroups(int ngroups, const gid_t grouplist[])` set group list for calling process
* `int initgroups(const char *username, gid_t basegid)` initializes groups for username, called by `login`

## Implementation differences

## Other data files

* `/etc/hosts, /etc/networks, /etc/protocols, /etc/services`
* the structure of these functions is always very similar:
  * a get function which opens the file if necessary, returns a pointer to a structure or NULL if end is reached
  * a set function which opens the file if necessary, rewinds the file
  * a end function which closes the file

## Login accounting

* `utmp` contains all the users currently logged in
* `wtmp` contains track of all logins and logouts
* example structure
    struct utmp {
      char ut_line[8]; /* tty line */
      char ut_name[8]; /* username */
      long ut_time;    /* unix timestamp */
* on login, an entry is added to utmp and wtmp
* on logout, an entry is removed from utmp and added to wtmp
* `who` reads from utmp, `last` reads from wtmp

## System identification

* `uname`, `int uname(struct utsname *name)`
* example structure:
    struct utsname {
      char sysname[];  /* OS name */
      char nodename[]; /* node name */
      char release[];  /* current OS release */
      char version[];  /* current OS release version */
      char machine[];  /* hardware type */
* uname -a
    Linux 3box 3.8.6-1-ARCH #1 SMP PREEMPT Sat Apr 6 08:44:16 CEST 2013 i686 GNU/Linux
    Darwin MetalMacBook.local 12.3.0 Darwin Kernel Version 12.3.0: Sun Jan  6 22:37:10 PST 2013; root:xnu-2050.22.13~1/RELEASE_X86_64 x86_64
* `int gethostname(char *name, int namelen)`

## Time and date routines

* Epoch: 1 January 1970
* UNIX keeps:
  * UTC time instead of localtime
  * automatically handling conversions (i.e. DST)
  * time and date are kept in single entity
* `time_t time(time_t *calptr)` in seconds
* `int gettimeofday(structu timeval *restrict tp, void *restrict tzp)` in microseconds
      struct timeval {
        time_t tv_sec;
        long   tv_usec;
      }
* `localtime, gmtime` convert calendar times into a tm struct with broken down values
* `mktime` converts a tm struct to calendar time
* `asctime, ctime` return a string representation of tm struct or calendar time
* `size_t strftime(char *restrict buf, size_t maxsize, const char *restrict format, const struct tm *restrict tmptr)`
* `locatime, mktime, ctime & strftime` are affected by the TZ environment variable, which is used instead of the default time zone
