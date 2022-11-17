#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
cmp_file(char* name, char* buf) { 
  char *p;

  // Find first character after last slash.
  for(p = buf + strlen(buf); p >= buf && *p != '/'; p--)
    ;
  p++;
  if (strcmp(p, name) == 0) {
	printf("%s\n", buf);
  }

  return;
}


void
find_file(char* name, char* buf)
{
  char *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(buf, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", buf);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", buf);
    close(fd);
    return;
  }
  cmp_file(name, buf);

  switch(st.type){
    case T_FILE:
      break;

    case T_DIR:
    if(strlen(buf) + 1 + DIRSIZ + 1 > 512){
      printf("ls: path too long\n");
      break;
    }
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
	  int l = strlen(de.name);
	  memmove(p, de.name, l);
      p[l] = 0;
	  
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
	  find_file(name, buf); 
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  char buf[512];
  memcpy(buf, argv[1], strlen(argv[1]));

  find_file(argv[2], buf);
  exit(0);
}
