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
* no shadow file on Mac OS

## Group file

*
