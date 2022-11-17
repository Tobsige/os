#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int 
run(char *s, char *argv[], int line_id, int argc)
{
  char *argv_new[argc + 1]; // add input and 0, substract xargs
  for (int i = 1; i < argc; i++) {
	argv_new[i - 1] = argv[i];
  }
  argv_new[argc - 1] = s;
  argv_new[argc] = 0;

  if (fork() == 0) {
	exec(argv[1], argv_new);
  } else {
	wait(0);
	line_id++;
  }
  return line_id;
} 

int
main(int argc, char *argv[])
{
  char buf[MAXARG], str[MAXARG][MAXARG];
  read(0, buf, MAXARG);
  char *p = buf;
  int num = 0; // number of line 

  while (*p) {
	int i = 0;
	while (*p != '\n' && *p != '\0' && i < MAXARG) {
	  str[num][i++] = *p;
	  p++;
	}
	if (!(*p)) break;
	p++;
	num++;
  }

  for (int i = 0; i < num; i++) {
	run(str[i], argv, i, argc); 
  }
  exit(0);
}
