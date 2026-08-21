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

