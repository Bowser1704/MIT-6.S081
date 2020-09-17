#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char*);

struct dirent de;
struct stat st;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void find(char *path, char *name) {
    int fd;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // tow much string / char pointer. it makes me confused.
    switch(st.type) {
        case T_FILE:
            if (strcmp(fmtname(path), name) == 0)
                printf("%s\n", concat(concat(path, "/"), name));
            break;
        case T_DIR:
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "usage: find path target-name\n");
        exit(1);
    }
    //char prefix[512];
    find(argv[1], argv[2]);
    exit(0);
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
