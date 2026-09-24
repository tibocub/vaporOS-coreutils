# Porting notes

A real spike, not just reading source -- vendored toybox upstream,
generated its own build config for a minimal 4-applet set (`true`,
`false`, `echo`, `pwd`), and compiled the actual generated sources
against NuttX's real headers.

**Multicall dispatch confirmed, no toybox patches needed for this
part.** Read `main.c` directly: it inspects `basename(argv[0])`,
looks it up in a build-generated `toy_list[]` table (name -> applet
`_main` function), and dispatches -- the standard multicall pattern.
Checked NuttX's own spawn chain (`binfmt_exec.c` -> `binfmt/builtin.c`)
confirms `argv[]` passes through from caller to the started task's
`main()` untouched -- nothing overwrites `argv[0]` with the resolved
program name.

**Superseded by a real build failure, not by further reading:** the
paragraph above describes what was *planned* -- spawning the program
literally as `toybox` with `argv[0]` rewritten to the matched applet
name, using toybox's own `main()`/`toybox_main()` dispatch directly.
That turned out to be unusable as-is. Toybox's own `main.c`
unconditionally calls an internal `toybox_main()` multiplexer whose
name is required (via `NEWTOY`'s `name##_main` token-pasting, wired
in more than one place in `toys.h`, not just one call site) to match
`generated/newtoys.h`'s own hardcoded `"toybox"` table entry.
Separately, NuttX's own `Application.mk` renames whatever program's
real `int main(argc, argv)` to `<PROGNAME>_main` -- with
`PROGNAME="toybox"` (required for the runtime string match above to
work) that's *also* literally `toybox_main`, a genuine symbol
collision, confirmed by a real build, not fixable by picking a
different `PROGNAME` alone (tried first: renaming the binary to
`tbx` fixed the collision but broke the runtime dispatch, since
`toy_find("tbx")` never matches the table's hardcoded `"toybox"`
entry).

Real fix: the program is registered as `tbx`, and its actual entry
point is a new file, `toybox/vapor_entry.c` -- it never calls
toybox's own `main()`/`toybox_main()` at all, calling the public
`toy_exec()` (toybox's own stable function for exactly this "dispatch
directly on argv[0]" case) instead. Toybox's own `main.c` is compiled
as a plain, unused `CSRCS` file; even its own `int main(argc, argv)`
had to be renamed away (to `toybox_unused_main`) since NuttX's *sim*
architecture has its own genuine `main()` (`arch/sim/src/sim/sim_head.c`,
the real host-process entry point for the whole simulated OS image) --
a plain, unrenamed, never-called `main()` sitting in a static library
still collides with that at final link, confirmed by a real
`multiple definition of 'main'` linker error.

Practical upshot for vaporshell: invocation is `argv[0]="tbx"` with
the applet name as `argv[1]` (`vapor_entry.c` calls `toy_exec(argv+1)`),
not the `argv[0]`-rewriting convention described above -- see the
multicall dispatch table entry in the Feature list.

**Two real bugs found and fixed in toybox's own `lib/portability.h`**
(patch kept separately, against toybox upstream, not this repo --
worth submitting there too once proven out further):
- Its generic (non-Apple/BSD) branch assumes `struct statfs` is
  already visible via some transitive include, true on glibc, not
  true on NuttX's more strictly-scoped headers -- needed an explicit
  `#include <sys/statfs.h>`.
- Once visible, NuttX's `struct statfs` (checked directly,
  `sys/statfs.h`) has no `f_frsize` field at all, unlike Linux --
  needed a NuttX-specific case using `f_bsize` for both, same as the
  pre-`f_frsize` Unix convention.

**Resolved (was "current real blocker" here):** `paths.h` (BSD-
derived, provides `_PATH_DEFPATH` and friends) doesn't exist anywhere
in NuttX's tree at all, and toybox's `toys.h` includes it
unconditionally. Fixed with a small compatibility header
(`toybox/nuttx-shims/paths.h`) providing just `_PATH_DEFPATH`, the
only macro from it actually referenced anywhere in this project's
toybox scope.

Also resolved along the way, worth remembering for next time rather
than re-discovering: NuttX's own math library headers
(`libs/libm/newlib/include/math.h` and its `machine/ieeefp.h`) aren't
copied into the top-level `include/` until a build actually reaches
that step -- an incomplete build's `include/` directory will be
missing `math.h` even though NuttX genuinely has it. In the end this
didn't matter for the actual fix: only `fabs()`/`sin()` are referenced
anywhere in this project's toybox scope, both from a single dead-for-
this-applet-set call site, so a small header-only shim
(`toybox/nuttx-shims/math.h`) was used instead of pulling in all of
newlib's real `libm` (which has its own portability issues against a
modern host `gcc`, hit directly: `libm/common/nanl.c` uses
`__GNUC_PREREQ`, a glibc-only macro).

**Current state, not just the original spike:** significantly more
ground covered since the paragraphs above were written -- real fixes
also landed for `lib/env.c` (NuttX has no setter for the whole
`environ` array, unlike glibc/BSD; delegates to NuttX's own
`setenv`/`unsetenv`/`clearenv` instead), a `CODE` macro collision
with NuttX's own `<nuttx/compiler.h>`, several more `portability.c`
gaps (`dev_minor`/`major`/`makedev`, `fs_type_name`, honest stubs for
mount-table/file-watch/xattr/`chroot`/raw `syscall`, none of which
NuttX has equivalents for), and the `toybox_main`/`vapor_entry.c`
saga above. Applet count: `true`, `false`, `echo`, `pwd` (batch 1),
`cat`, `mkdir`, `rmdir`, `touch`, `printf`, `rm` (batch 2). `ls`,
`cp`, `mv` deliberately deferred to their own batch -- much larger,
heavier applets (directory traversal, permission/symlink handling)
likely to surface new platform gaps the way earlier batches did.


## Batch 4: head tail wc tee cut uniq sort yes

**Workflow now:** to enable an applet, flip its line in
`scripts/toybox.config`, copy its `.c` from upstream (same `toys/<dir>/`),
add it to the Makefile `CSRCS`, run `scripts/regen.sh <toybox checkout at
the pinned commit>`. `tests/` holds the host-toybox-vs-sim comparison.

Problems found by running on the sim (all reproduced with gdb, not
guessed), and where each is handled:

- **Concurrent tbx tasks shared toybox's globals.** Flat build = one address
  space, so `toys`, `this`, `toybuf`, `libbuf` were shared by every stage of
  a pipeline (`yes | head > file` segfaulted). Each tbx task now gets a heap
  context through task-local storage; `nuttx-shims/vapor_ctx.h` remaps the
  four names, `vapor_entry.c` allocates it. Not covered: file-scope
  `static` variables inside applets/lib -- check new ports for them.
- **`getline()`/`getdelim()` with a NULL buffer used an uninitialized size**
  (POSIX ignores it, NuttX mallocs it): sort/cut/uniq printed nothing or
  one line. Fixed once in `nuttx-shims/vapor_libc.h`.
- **`tail -f`** needed `xnotify_*`; implemented by polling (250 ms). On the
  sim's VFAT `/tmp` an already-open fd does not see bytes appended through
  another open (hostfs does), so growth is followed by re-opening the path
  and `dup2()`ing it back at the same offset (`vapor_refresh_fd`).
- **`wc` name collision** with the `portable_wc` app (`wc_main`); the app
  is now `vwc`. Any future applet named like a NuttX app has the same risk.
- `cut -F`, and later grep/sed/expr, need `CONFIG_LIBC_REGEX`. NuttX's has
  no `REG_STARTEND`, so text with embedded NUL bytes is cut off at the NUL.
- `seq` left out: upstream needs `TOYBOX_FLOAT`, which is off here.


## Batch 5: grep egrep fgrep sed tr ln cmp uname arch expr date chmod

Same workflow as batch 4 (`tests/batch5.t`, host toybox vs sim). Left out on
purpose: find, xargs, env -- they need to run other programs (`xrun()`,
`xexec()`), which has no fork()+exec() to sit on; that wants a
posix_spawn-based helper of its own, so its own batch.

Found by running on the sim, and where each is handled:

- **lseek() on a pipe succeeds.** NuttX's `file_seek()` bumps a meaningless
  offset for anything whose driver has no seek method (pipes, ptys, serial)
  instead of failing with ESPIPE. grep's binary-file check (`read` 256 bytes,
  `lseek` back, only "on lseekable files") therefore ate the start of every
  piped input. `vapor_lseek()` in `nuttx-shims/vapor_libc.h` returns ESPIPE
  for FIFOs, sockets and ttys.
- **st_ino/st_dev are always 0.** No NuttX filesystem fills them in, so
  `same_file()` was true for every pair: `cp a dir/a` failed with
  "'dir/a' is 'a'" (already true for batch 3's cp/mv). cp.c uses
  `vapor_same_node()` (lib/portability.c), which compares resolved paths
  (`F_GETPATH` + `realpath`) when the stat data can't tell. Still to do:
  `test -ef`, `tail -F`'s dev/ino check.
- **tr indexed a table with a signed char.** Bytes >= 0x80 (always for
  `tr -c`, and any UTF-8 input) hit `TT.map[-128..-1]`; on NuttX that corrupted
  the heap and hung the whole sim, only after an earlier tr had run. Casts to
  unsigned char (also reported upstream-worthy). tr also spun forever on a
  read() error; now exits.
- **uname walked `struct utsname` in equal strides.** NuttX's fields have
  different sizes, so `uname -a` was garbage and `-m` printed the version.
  Fields are indexed by name now.
- **strftime() has no %Z.** `date` printed an empty zone and `date +%Z`
  failed; `nx_strftime()` in date.c expands it from `tm_zone`/`tzname`.
- **ln and chmod fail with ENOSYS** ("Invalid system call number") on the
  sim's FAT `/tmp`: no hard links, no symlinks, no permission bits there.
  Not worked around; the applets report the error. Not checked on hostfs.
- vaporshell's tbx command table (`vaporshell/dispatch.c`) must list every new
  applet, or it answers "command not found".

Expected differences in `tests/batch5.t`: the `ln` and `chmod` blocks (above)
and `chmod`'s usage message (no help text compiled in).


## Batch 6: find xargs env nohup + text/file utilities

`tests/batch6.t` (103 commands, host toybox vs sim: no differences). Added:
find xargs env nohup comm expand fold nl od paste split tty unlink rev tac
truncate xxd mktemp md5sum sha1sum sha224sum sha256sum sha384sum sha512sum.

**Running other programs without fork()+exec().** find -exec, xargs, env and
`xpopen_setup()`/`xrun()` all assumed fork()/vfork() plus exec(). New
`vapor_spawn()` (lib/portability.c) does it with `posix_spawnp()`: an installed
program of that name wins (same rule as vaporshell's `vs_plat_spawn()`), and
only on ENOENT is the name looked up as a tbx command (toybox applet or
nsh-port) and run as `tbx <name> <args>`. Redirections go through
`posix_spawn_file_actions_*` (checked against NuttX's `task_spawnparms.c`:
actions run in order on the child's fd list, open/dup2/close as in POSIX).
Where it is used:

- `xpopen_setup()` (so `xrun`, `xpopen`, `xrunread`): NuttX version in
  lib/xwrap.c. It needs `argv`; the "re-run myself in the child" and
  `callback` uses can't exist without fork and error out (nothing in the
  applets ported so far uses them; `timeout` does, so it is not ported). A
  failed spawn is reported like the fork()ed child would have (error message,
  exit status 126/127, returned as a negative "pid" that `xwaitpid()`
  understands).
- `xexec()`: no exec() that replaces the task on NuttX, so it runs the command
  as a child and exits with its status.
- xargs (own spawn call: stdin from /dev/null or /dev/tty, slot variable) and
  env (the whole environment array, `-i`).

Problems found on the sim:

- **Arguments live on the new task's stack.** NuttX copies argv onto the
  spawned task's stack (`nxtask_setup_stackargs`), and tbx's stack is small.
  `sysconf(_SC_ARG_MAX)` is 4096, which made xargs' `ARG_MAX - environment -
  4096` negative ("command too long" for everything). find and xargs use
  `VAPOR_ARGS_MAX` (a quarter of the tbx stack size) instead; longer input is
  split into several runs, as POSIX allows.
- **`open(".")` fails** (trailing "." is never resolved, see lib/dirtree.c):
  find -exec opens the start directory; it now uses the real cwd path.
- **`same_file()` was true for everything** (no st_ino/st_dev, see batch 5):
  find called every subdirectory a "loop". `same_file()` now says "no" when
  both sides have no inode information; cp keeps using `vapor_same_node()`.
  Not fixed: `find -samefile`/`-inum`, `test -ef`.
- **`environ` is NULL when a task has no environment** (`get_environ_ptr()`
  returns `tg_envp`), so `env -i printenv X` dereferenced NULL and hung the
  sim. `vapor_environ()` in nuttx-shims/vapor_libc.h hands out an empty array.

Known and not fixed: paths with a `.` or `..` component *inside* a FAT
directory fail with "Not a directory" (`cat sub/./q.txt`; from inside `/tmp/sub`,
`cat ./q.txt` and `cat ../p.txt`; `/tmp/./p.txt` and `cat ./p.txt` from `/tmp`
itself work). Reproduced on the sim's FAT `/tmp`, cause not investigated
(FAT driver, not toybox). It makes `find . -exec cat {} \;` and `xargs rm`
over `find .` output fail when run from a subdirectory. Not checked on hostfs.

