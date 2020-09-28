#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc < 2){
        fprintf(2,"usage: xargs cmd [...]\n");
        exit(1);        
    }
    char *real_argv[MAXARG];
    int i;

    for (i = 1; i < argc; i++) {
        real_argv[i-1] = argv[i];
    }
    real_argv[argc] = 0;
    int flag = 1;
    while (flag) {
        char buf[128], *p = buf;
        while ((flag = read(0, p, 1)) && *p != '\n') {
            p++;
        }
        if (flag == 0) exit(0); // flag == 0, not '\n'.
        *p = 0; // important
        real_argv[argc - 1] = buf;
        if(fork() == 0){
            exec(argv[1], real_argv);
            exit(0);
        }
        wait(0);
    }
    return 0;
}
