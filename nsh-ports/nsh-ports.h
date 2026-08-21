/* nsh-ports.h -- commands ported from NSH's own implementation rather
 * than toybox's, because they're fundamentally about NuttX board/
 * kernel state (boardctl(), the scheduler, mount table, etc) that
 * toybox has no concept of at all -- see each command's own top-of-
 * file comment for its specific NSH source and what changed getting
 * here. Kept in their own directory, and dispatched through their own
 * small table here, deliberately separate from toys/ -- not because
 * users should ever see the difference (they don't: vapor_entry.c
 * tries this table first, then falls through to toybox's own
 * toy_exec() for anything not listed here), but so a future NSH
 * update can be diffed against nsh-ports/ alone, the same way toys/
 * only ever needs diffing against upstream toybox.
 */

#ifndef NSH_PORTS_H
#define NSH_PORTS_H

/* Returns the command's own exit code if argv[0] matched something in
 * this table (having already run it), or -1 if nothing matched --
 * exactly the same "return if not found, don't exit()" contract as
 * toybox's own toy_exec(), so vapor_entry.c can try one after the
 * other without either needing special-case handling.
 */

int nshports_dispatch(int argc, char *argv[]);

#endif
