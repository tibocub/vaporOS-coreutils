/* dispatch.c -- the only file that needs editing when a new nsh-port
 * is added: declare its _main here, add one line to g_nshports[]. See
 * nsh-ports.h for the contract (-1 = not found, don't exit()).
 */

#include <string.h>

#include "nsh-ports.h"

int nshport_poweroff_main(int argc, char *argv[]);

struct nshport_s
{
  const char *name;
  int (*main)(int argc, char *argv[]);
};

static const struct nshport_s g_nshports[] =
{
  { "poweroff", nshport_poweroff_main },
};

#define NSHPORTS_COUNT (sizeof(g_nshports) / sizeof(g_nshports[0]))

int nshports_dispatch(int argc, char *argv[])
{
  size_t i;

  for (i = 0; i < NSHPORTS_COUNT; i++)
    {
      if (strcmp(argv[0], g_nshports[i].name) == 0)
        {
          return g_nshports[i].main(argc, argv);
        }
    }

  return -1;
}
