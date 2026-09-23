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
#endif
