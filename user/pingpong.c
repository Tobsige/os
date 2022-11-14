#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  char buf[9] = {'a'};
  pipe(p);

  if (fork() == 0) {
	read(p[0], buf, 1);
	printf("%d: received ping\n", getpid());
	close(p[0]);

	write(p[1], buf, 1);
	close(p[1]);
	exit(0);
  } else {
	write(p[1], buf, 1);
	read(p[0], buf, 1);
	close(p[0]);
  	printf("%d: received pong\n", getpid());

  } 
  exit(0);
}
