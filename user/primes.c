#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
child_write(int *p, char* buf, char* send) {
	int base = buf[0];
	int n = strlen(buf);
	int j = 0;
	printf("prime %d\n", base);
	for (int i = 1; i < n; i++) {
	  int num = buf[i];
	  if (num % base != 0) {
		send[j++] = num;
	 }
	}
	send[j] = '\0';
	return j;
}

int
main(int argc, char *argv[])
{
  int p[2];
  char buf[35];
  char send[35];
  pipe(p);

 
  int j = 0;
  for (int i = 2; i <= 35; i++) {
	send[j++] = i;
  } 
  send[j] = '\0';

  for (int i = 0; i < 35; i++) {
	if (fork() == 0) {
	  buf[0] = '\0';
	  sleep(5);
	  int num = read(p[0], buf, 34); 
	  if (num < 0) {
		printf("no read %d\n", num);
		break;
		exit(0);
	  }
	  int num_w = child_write(p, buf, send);  
	  if (num_w == 1) {
		printf("prime %d\n", send[0]);
		exit(0);
	  }
	} else {
	  write(p[1], send, 34);
	  wait(0);
	  exit(0);
    }  
	if (strlen(send) == 1 || (i == 0 && strlen(buf) == 0)) break;
  } 
   close(p[0]);
   close(p[1]);
   exit(0);
}
