#include <sys/mman.h> //mmap
#include <unistd.h>   //write
#include <fcntl.h>    //open
#include <sys/stat.h> //stat
#include <stdio.h>    //fprintf

void mmapcopy(int fd, int size) {
  char *bufp;
  bufp = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0); // we should use MAP_PRIVATE or MAP_SHARED flag.
  
  write(1, bufp, size);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s filename\n", argv[0]);
    return -1;
  }

  int fd = open(argv[1], O_RDONLY);
  struct stat statbuf;
  fstat(fd, &statbuf);

  mmapcopy(fd, statbuf.st_size);
  return 0;
}
