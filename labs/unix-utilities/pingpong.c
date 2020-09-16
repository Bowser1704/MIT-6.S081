#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {

    // 0 for input 1 for output
    // p2c parent to child
    // c2p child to parent
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);
    char ping = 'i', pong = 'o';

    if (fork() != 0) {
        // parent
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], &ping, 1);
        read(c2p[0], &ping, 1);
        printf("%d: received pong\n", getpid());
    }else {
        // child
        close(p2c[1]);
        close(c2p[0]);
        read(p2c[0], &pong, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[1], &pong, 1);
    }
    exit(0);
}
