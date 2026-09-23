struct basename_data {
  char *s;
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

struct head_data {
  long c, n;

  int file_no;
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

struct sort_data {
  char *t;
  struct arg_list *k;
  char *o, *T, S;

  void *key_list;
  unsigned linecount;
  char **lines, *name;
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
extern union global_union {
	struct basename_data basename;
	struct cp_data cp;
	struct cut_data cut;
	struct head_data head;
	struct ls_data ls;
	struct mkdir_data mkdir;
	struct sort_data sort;
	struct tail_data tail;
	struct tee_data tee;
	struct touch_data touch;
	struct uniq_data uniq;
	struct wc_data wc;
} this;
