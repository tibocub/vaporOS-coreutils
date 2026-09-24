struct md5sum_data {
  int sawline;
};

struct mktemp_data {
  char *p, *tmpdir;
};

struct tac_data {
  struct double_list *dl;
};

struct truncate_data {
  char *s;

  long long size;
  int type;
};

struct xxd_data {
  long s, g, o, l, c;
};

struct expr_data {
  char **tok, *delete;
};

struct tr_data {
  short *map;
  int len1, len2;
};

struct basename_data {
  char *s;
};

struct chmod_data {
  char *mode;
};

struct cmp_data {
  long n;

  int fd;
  char *name;
};

struct cp_data {
  union {
    // install's options
    struct {
      char *g, *o, *m, *t;
    } i;
    // cp's options
    struct {
      char *t, *preserve;
    } c;
  };

  char *destname;
  struct stat top;
  int (*callback)(struct dirtree *try);
  uid_t uid;
  gid_t gid;
  int pflags;
};

struct cut_data {
  char *d, *O;
  struct arg_list *select[5]; // we treat them the same, so loop through

  unsigned line;
  int pairs;
  regex_t reg;
};

struct date_data {
  char *s, *r, *I, *D, *d;

  unsigned nano;
};

struct env_data {
  struct arg_list *u;
  char *e;
};

struct expand_data {
  struct arg_list *t;

  unsigned tabcount, *tab;
};

struct find_data {
  char **filter;
  struct double_list *argdata;
  int topdir, xdev, depth;
  time_t now;
  long max_bytes;
  char *start;
};

struct fold_data {
  long w;
};

struct grep_data {
  long m, A, B, C;
  struct arg_list *f, *e, *M, *S, *exclude_dir;
  char *color;

  char *purple, *cyan, *red, *green, *grey;
  struct double_list *reg;
  int found, tried, delim;
  struct arg_list **fixed;
};

struct head_data {
  long c, n;

  int file_no;
};

struct ln_data {
  char *t;
};

struct ls_data {
  long w, l, block_size;
  char *color, *sort;

  struct dirtree *files, *singledir;
  unsigned screen_width;
  int nl_title;
  char *escmore;
};

struct mkdir_data {
  char *m, *Z;
};

struct nl_data {
  char *s, *n, *b;
  long w, l, v;

  // Count of consecutive blank lines for -l has to persist between files
  long lcount, slen;
};

struct od_data {
  struct arg_list *t;
  char *A;
  long N, w, j;

  int address_idx;
  unsigned types, leftover, star;
  char *buf; // Points to buffers[0] or buffers[1].
  char *bufs[2]; // Used to detect duplicate lines.
  off_t pos;
};

struct paste_data {
  char *d;

  int files;
};

struct sed_data {
  char *i;
  struct arg_list *f, *e;

  // processed pattern list
  struct double_list *pattern;

  char *nextline, *remember, *tarxform;
  void *restart, *lastregex;
  long nextlen, rememberlen, count;
  int fdout, noeol;
  unsigned xx, tarxlen, xflags;
  char delim, xftype;
};

struct sort_data {
  char *t;
  struct arg_list *k;
  char *o, *T, S;

  void *key_list;
  unsigned linecount;
  char **lines, *name;
};

struct split_data {
  long n, l, b, a;

  char *outfile;
};

struct tail_data {
  long n, c;
  char *s;

  int file_no, last_fd, ss;
  struct xnotify *not;
  struct {
    char *path;
    int fd;
    struct dev_ino di;
  } *F;
};

struct tee_data {
  void *outputs;
  int out;
};

struct touch_data {
  char *t, *r, *d;
};

struct uniq_data {
  long w, s, f;

  long repeats;
};

struct wc_data {
  unsigned long totals[5];
};

struct xargs_data {
  long s, n, P;
  char *E, *a, *process_slot_var;

  long entries, bytes, np;
  char delim;
  FILE *tty;
  pid_t *pids;
};
extern union global_union {
	struct md5sum_data md5sum;
	struct mktemp_data mktemp;
	struct tac_data tac;
	struct truncate_data truncate;
	struct xxd_data xxd;
	struct expr_data expr;
	struct tr_data tr;
	struct basename_data basename;
	struct chmod_data chmod;
	struct cmp_data cmp;
	struct cp_data cp;
	struct cut_data cut;
	struct date_data date;
	struct env_data env;
	struct expand_data expand;
	struct find_data find;
	struct fold_data fold;
	struct grep_data grep;
	struct head_data head;
	struct ln_data ln;
	struct ls_data ls;
	struct mkdir_data mkdir;
	struct nl_data nl;
	struct od_data od;
	struct paste_data paste;
	struct sed_data sed;
	struct sort_data sort;
	struct split_data split;
	struct tail_data tail;
	struct tee_data tee;
	struct touch_data touch;
	struct uniq_data uniq;
	struct wc_data wc;
	struct xargs_data xargs;
} this;
