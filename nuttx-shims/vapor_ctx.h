#ifndef VAPOR_CTX_H
#define VAPOR_CTX_H
// Per-task toybox globals for NuttX.
//
// Unix gives every toybox applet its own process, so the globals `toys`,
// `this` (the union of every applet's GLOBALS()), `toybuf` and `libbuf`
// are private to one running command. NuttX's flat build runs each tbx
// invocation as a task in ONE address space, so with plain globals two
// applets in a pipeline (`yes | head`) overwrite each other's state.
// Verified with gdb: head's read() into toybuf clobbered the iovec array
// yes was writev()ing from, and the sim segfaulted.
//
// So each tbx task gets its own heap-allocated struct vapor_ctx (created
// in vapor_entry.c, freed by a NuttX task-TLS destructor at task exit),
// and these macros redirect the four names to it. Included at the very
// end of toys.h so every applet and lib file picks it up unchanged.
//
// vapor_ctx() is declared const: it returns the same pointer for the whole
// life of a task, so the compiler may call it once per function instead of
// once per access (toybuf[pos] in a byte loop stays cheap).
//
// Not covered: file-scope `static` variables inside individual applets or
// lib/*.c -- those are still shared between concurrent tasks. Check any
// newly ported applet for them (grep -n '^static [^(]*[;=]').
struct vapor_ctx {
  struct toy_context t;
  union global_union g;
  char tb[4096], lb[4096];
};

struct vapor_ctx *vapor_ctx(void) __attribute__((const));

#define toys   (vapor_ctx()->t)
#define this   (vapor_ctx()->g)
#define toybuf (vapor_ctx()->tb)
#define libbuf (vapor_ctx()->lb)
#endif
