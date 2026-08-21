/* poweroff.c -- shut the board down.
 *
 * Modeled directly on NSH's own cmd_poweroff() (apps/nshlib/nsh_syscmds.c)
 * -- boardctl(BOARDIOC_POWEROFF, ...) has no equivalent in toybox at all
 * (toybox targets generic Linux/POSIX systems, with no concept of
 * NuttX's own board-control interface), so there's no upstream toybox
 * source to diverge from here the way there is for the toys/ ports.
 * NSH's own implementation IS the reference -- kept as close to it as
 * the vtbl dependency allows, specifically so a future change to NSH's
 * own cmd_poweroff can be manually reapplied here by diffing the two,
 * same rationale as toys/ tracking toybox upstream.
 *
 * Real differences from NSH's own version, and why:
 * - nsh_error(vtbl, ...) -> plain fprintf(stderr, ...): this isn't an
 *   NSH command, so there's no struct nsh_vtbl_s to route output
 *   through.
 * - NSH_ERRNO's own strerror(errno)-vs-plain-errno toggle
 *   (CONFIG_NSH_STRERROR) isn't replicated -- that exists in NSH so
 *   memory-constrained targets can drop strerror() for code size, a
 *   concern that doesn't apply here: CONFIG_LIBC_STRERROR=y is
 *   already a hard, unconditional requirement for this whole project
 *   (see vaporOS-nuttx's own build.sh). Just uses strerror(errno)
 *   directly.
 * - argv[0] is this program's own name ("poweroff", supplied by
 *   vapor_entry.c's dispatch), not NSH's -- same meaning either way.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/boardctl.h>

int nshport_poweroff_main(int argc, char *argv[])
{
  /* Invoke the BOARDIOC_POWEROFF board control to shutdown the board. If
   * the board_power_off function returns, then it was not possible to
   * power-off the board due to some constraints.
   */

  if (argc > 1)
    {
      boardctl(BOARDIOC_POWEROFF, atoi(argv[1]));
    }
  else
    {
      boardctl(BOARDIOC_POWEROFF, 0);
    }

  /* boardctl() will not return in any case. If it does, it means that
   * there was a problem with the shutdown operation.
   */

  fprintf(stderr, "%s: boardctl failed: %s\n", argv[0], strerror(errno));
  return 1;
}
