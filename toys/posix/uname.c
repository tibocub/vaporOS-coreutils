/* uname.c - return system name
 *
 * Copyright 2008 Rob Landley <rob@landley.net>
 *
 * See http://opengroup.org/onlinepubs/9699919799/utilities/uname.html

USE_UNAME(NEWTOY(uname, "paomvrns", TOYFLAG_BIN))
USE_ARCH(NEWTOY(arch, 0, TOYFLAG_USR|TOYFLAG_BIN))

config ARCH
  bool "arch"
  default y
  help
    usage: arch

    Print machine (hardware) name, same as uname -m.

config UNAME
  bool "uname"
  default y
  help
    usage: uname [-asnrvmo]

    Print system information.

    -s	System name
    -n	Network (domain) name
    -r	Kernel Release number
    -v	Kernel Version
    -m	Machine (hardware) name
    -a	All of the above (in order)

    -o	Userspace type
*/

#define FOR_uname
#define FORCE_FLAGS
#include "toys.h"

void uname_main(void)
{
  struct utsname *u = (void *)toybuf;
  // Upstream steps through the struct in sizeof(sysname) strides, which
  // assumes every field is the same size (true on Linux, all 65 bytes). NuttX's
  // struct utsname has differently sized fields (see sys/utsname.h), so
  // `uname -a` printed garbage and `uname -m` printed the version string.
  // Index the fields by name instead: correct wherever the layout differs.
  char *field[5] = {u->sysname, u->nodename, u->release, u->version,
                    u->machine};
  int i, needspace = 0;

  if (uname(u)) perror_exit("uname");
  if (!toys.optflags) toys.optflags = FLAG_s;
  for (i=0; i<6; i++) if (toys.optflags & ((1<<i)|FLAG_a)) {
    if (needspace++) xputc(' ');
    xputsn(i==5 ? "Toybox" : field[i]);
  }
  if (FLAG(p)) xputsn(" unknown"+!needspace);
  xputc('\n');
}

void arch_main(void)
{
  toys.optflags = FLAG_m;
  uname_main();
}
