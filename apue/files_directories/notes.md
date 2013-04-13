# Files and directories

## stat, fstat and lstat functions

* functions which return a structure of information about the file
* `stat(pathname, *buf)` 
* `fstat(filedes, *buf)`
* `lstat(pathname, *buf` supports symlinks
* example structure
    struct stat {
        mode_t     st_mode;     / * file type and mode */
        ino_t      st_ino;      / * inode number */
        dev_t      st_dev;      / * device number (file system) */
        dev_t      st_rdev;     / * device number (special) */
        nlink_t    st_nlink;    / * number of links */
        uid_t      st_uid;      / * user ID Of owner */
        gid_t      st_gid;      / * group ID Of owner */
        off_t      st_size;     / * file size in bytes, for regular files */
        time_t     st_atime;    / * last access-time */
        time_t     st_mtime;    / * last modification-time */
        time_t     st_ctime;    / * last change-time of file status */
        blksize_t  st_blksize;  / * best I/0 blocksize */
        blkcnt_t   st_blocks;   / * number of disk blocks */
    }

## File types

* from `st_mode`
* there can be:
  * regular files
  * directory file
  * block special file
  * character special file
  * FIFO
  * socket
  * symlink

## set-user-ID and set-group-ID

* from `st_mode`
* every process has six ids associated to it:
  * from password file
    * real user ID
    * real group ID
  * determine file access permissions
    * effective user ID
    * effective group ID
  * copies of effective IDs
    * saved set-user-ID
    * saved set-group-ID
* usually real ID == effective ID
* there is the possibility of telling the OS:
  * when this file is executed, set the effective user ID to the owner of the file
  * or set group ID aswell
* passwd is owned by root, but is set-user-ID so anyone can change his own password

## File permissions

* from `st_mode`
  * user, RWX
  * group, RWX
  * others, RWX
* whenever we want to open a file, we must have execute permission each directory in its path
* open "spam.txt" is the same as opening "./spam.txt"
* read permission of the directory let us list the files
* execute permission on the directory let us access the files
* we cannot create a file in a directory if we don't have execute and write permissions on it
* to delete a file, we need execute and write permissions on the folder (not on the file)
* when opening a file, the kernel:
  * if user is root, go on
  * else, if effective user ID == file owner ID, go on if permissions match
  * else, if effective group ID =~ file group ID, go on if permissions match
  * else, if flags for others allow access, go on
  * else, permission denied

## Ownership of new files and directories

* when creating a new file
  * file owner ID is set to effective user ID
  * file group ID is set to effective group ID
  * or, file group ID is set to folder group ID

## access function

* `access(pathname, mode)`
* useful when:
  * even though the process has been set-user-ID or set-group-ID
  * the kernel wants to check access permissions for the real user/group IDs

## umask function

* `umask(mode)`
* umask returns the file mode creation mask
* `umask -S`

## chmod, fchmod functions

* `chmod(pathname, mode)`
* `fchmod(filedes, mode)`
* these functions change only ctime

## sticky bit

* originally used to keep programs in swap for fast access
* if a directory has the sticky bit set (`chmod o+s <dir>`), a file can be removed or renamed only if:
  * user has write execute permissions on the directory
  * and one of the following:
    * is the superuser
    * user owns the file
    * user owns the directory
* used for tmp folders (many users have to make tmpfiles, but no one should touch other users' files)

## chown, fchown, lchown functions

## File size

* found in `st_size`
* size in bytes for regular files
* size for directories multiple of a number 16, 256
* size of pointed filename for symlinks

## Holes in files

## File truncation

* `truncate(pathname, length)`
* `ftruncate(filedes, length)`

## File systems

* partition > file system > cylinder group > inodes array > inode
* two directory entries can point to the same inode; each inode has a link count which tells if it can be deleted
* most of the stat info is fetched from the inode, the directory entry has only the dir name and the inode number
* the inode of the directory points to another inode in the same file system
* we cannot have hard links across different file systems

## link, unlink and rename

* `link(existingpath, newpath)`
* `unlink(pathname)`
* when a file link count gets to 0, the kernel deletes its content
* when the program exits, all open files are closed
* unlink is used for tmpfiles, so that those are still deleted by the kernel if the program crashes
* `rename(oldname, newname)`
* when renaming files, we are as if we were deleting the old file and creating a new one, so:
  * we need write and execute permissions on both folders

## Symbolic links

* Solve two problems of hard links:
  * you can generally create hard link only on the same file system
  * only root user can create hard links to directories
* we always need to know if a function follows symlinks

## symlink and readlink functions

* `symlink(actualpath, sympath)`
* `readlink(pathname, *buf, bufsize)`

## File times

* generally Epoch timestamps
* `st_atime`, last access-time
* `st_mtime`, last modification-time, aka last time contents were changed
* `st_ctime`, last change-time, aka last time inode was modified

## utime function

* `utime(*pathname, *times)` changes atime and mtime
* if times is NULL, set atime and mtime to current time



