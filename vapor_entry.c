// Real entry point NuttX's Application.mk renames to <PROGNAME>_main
// (via -Dmain=). Deliberately NOT toybox's own main.c/main(): that
// function unconditionally calls its internal toybox_main() (a
// SEPARATE function from this file's own renamed entry -- same name,
// different purpose), whose recursive self-dispatch requires argv[0]
// to literally equal generated/newtoys.h's own hardcoded "toybox"
// table entry name, wired via NEWTOY's name##_main token-pasting in
// more than one place in toys.h -- confirmed directly: renaming that
// internal function to sidestep the naming collision broke more than
// a single grep-able call site found. That's fundamentally
// incompatible with also wanting NuttX's own <PROGNAME>_main renaming
// to apply to the same "toybox" identifier for a different purpose
// (the real C entry point) -- both mechanisms end up wanting to claim
// literally "toybox_main" for two different functions.
//
// This wrapper sidesteps the whole conflict rather than fighting it:
// it never calls toybox's own main() or toybox_main() at all.
// toy_exec() (declared in toys.h, a real function toybox itself
// exports for exactly this "dispatch directly on argv[0]" case, used
// elsewhere in toybox's own sources) does the actual work, and never
// touches the "toybox" self-match convention -- so PROGNAME can be
// "tbx" (or anything else) with no collision, and doesn't need to
// match any string toybox's own generated tables hardcode.
//
// Invocation model: `tbx true` means argv[0]="tbx" (this program's
// own name, supplied by whatever spawned it) and argv[1]="true" (the
// applet) -- ordinary shell-style invocation, not toybox's own
// symlink-as-argv[0] convention. So argv+1 is what gets passed to
// toy_exec(), whose own argv[0] becomes "true".
//
// Also the overall dispatcher for nsh-ports/ (commands ported from
// NSH's own implementation, not toybox's -- see nsh-ports/nsh-ports.h)
// tried first, toy_exec() as the fallback for anything not listed
// there. Both this file and vaporshell's own dispatch table only ever
// see one flat command namespace either way -- this internal
// toybox-vs-nsh-ports split is purely for keeping each set easy to
// diff against its own upstream, not something a user (or vaporshell)
// needs to know about.
#include "toys.h"
#include "nsh-ports/nsh-ports.h"

int main(int argc, char *argv[])
{
  int ret;

  if (argc<2) {
    fprintf(stderr, "usage: %s <command> [args...]\n", argv[0]);

    return 1;
  }

  // Checked first, not because order matters for users (nsh-ports and
  // toybox names shouldn't ever collide), but because toy_exec() is
  // the "catch everything toybox itself doesn't know" case -- see its
  // own call below.
  ret = nshports_dispatch(argc-1, argv+1);
  if (ret != -1) {
    return ret;
  }

  toys.stacktop = &argc;
  toy_exec(argv+1);

  // toy_exec() only returns if it couldn't find/run anything (see
  // toy_exec_which()'s own "return if we can't find it" case) --
  // toybox's own unknown()/help_exit() aren't public (unknown() is
  // static to main.c), so this is a small, deliberately plain
  // stand-in, not an attempt to replicate their exact output.
  fprintf(stderr, "%s: unknown command '%s'\n", argv[0], argv[1]);

  return 127;
}
