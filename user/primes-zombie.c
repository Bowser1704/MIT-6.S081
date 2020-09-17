#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int is_prime(int);

int main() {
    int left[2], right[2];
    int i, prime;
    
    // 0 for input/read 1 for output/write
    pipe(left);
    pipe(right);

    for (i = 2; i <= 35; i++) {
        write(left[1], &i, 4);
        if (fork() == 0) {
            // child
            int num;
            close(left[1]);
            close(right[0]);
            read(left[0], &num, 4);
            if (is_prime(num)) {
                write(right[1], &num, 4);
            }
            close(left[0]);
            close(right[1]);
            exit(0);
        }   
    }
    while (read(right[0], &prime, 4) != 0) {
        printf("prime %d\n", prime);
    }
    exit(0);
}

int is_prime(int num) {
    int i;
    for (i = 1; i*i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
