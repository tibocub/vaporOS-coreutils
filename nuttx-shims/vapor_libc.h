#ifndef VAPOR_LIBC_H
#define VAPOR_LIBC_H
// getline()/getdelim() with POSIX's "*lineptr == NULL means allocate" rule.
//
// POSIX: when *lineptr is NULL the buffer is allocated and *n is ignored.
// glibc/musl/BSD do that; NuttX's getdelim() instead mallocs *n bytes, so
// an uninitialized *n makes it fail with ENOMEM (or truncate the read).
// toybox relies on the POSIX behaviour in many places, e.g.
//   do_lines():   getdelim(&line, (void *)&len, ...)   len uninitialized
//   uniq_main():  size_t thissize;  (never initialized)
// which showed up as sort/cut/uniq silently printing nothing or one line,
// depending on stack garbage. Fixed once here rather than at every call
// site, so ported applets keep matching upstream. Included at the end of
// toys.h, after <stdio.h>.
static inline ssize_t vapor_getdelim(char **line, size_t *size, int delim,
                                     FILE *fp)
{
  if (!*line) *size = 0;

  return getdelim(line, size, delim, fp);
}

#define getdelim(line, size, delim, fp) vapor_getdelim(line, size, delim, fp)
#define getline(line, size, fp) vapor_getdelim(line, size, '\n', fp)

// lseek() on a pipe/FIFO/socket/tty must fail with ESPIPE (POSIX). NuttX's
// file_seek() instead "succeeds" on anything whose driver has no seek
// method (pipes, ptys, serial devices...) by just bumping a per-file
// offset that means nothing. toybox uses lseek(fd, 0, SEEK_CUR) as its
// "is this seekable?" probe, e.g. grep's binary-file check reads 256 bytes
// and then lseek()s back: on a pipe the probe says yes, the rewind does
// nothing, and `cat f | grep x` silently loses the first 256 bytes (all of
// a short input). Reject the non-seekable types up front instead.
static inline off_t vapor_lseek(int fd, off_t off, int whence)
{
  struct stat st;

  if (!fstat(fd, &st) && (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode) ||
                          (S_ISCHR(st.st_mode) && isatty(fd)))) {
    errno = ESPIPE;

    return -1;
  }

  return lseek(fd, off, whence);
}

#define lseek(fd, off, whence) vapor_lseek(fd, off, whence)
#endif
