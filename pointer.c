#include <stdio.h>
#include <stdlib.h>

void
f(void)
{
    int a[4]; // a is a int array pointer which has 4 element / 16 bytes
    int *b = malloc(16);  // b is 16 bytes
    int *c; // c is a pointer without memory
    int i;

    printf("1: a = %p, b = %p, c = %p\n", a, b, c);

    c = a; // c -> a
    for (i = 0; i < 4; i++)
	a[i] = 100 + i; // a[0] = 100, a[1] = 101 ...
    c[0] = 200; // a[0] = c[0] = 200
    printf("2: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    printf("200, 101, 102, 103\n");

    c[1] = 300;
    *(c + 2) = 301; // *(c + 2) == c[2], c[2] = 102 to c[2] = 301
    3[c] = 302; // 3[c] ???
    printf("3: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    printf("200, 300, 301, 302\n");

    c = c + 1; // c now is pointer to a[1];
    *c = 400;
    printf("4: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    printf("200, 400, 301, 302\n");

    c = (int *) ((char *) c + 1); // c now point to other memory, because char is 2-byte.
    *c = 500;
    printf("5: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n",
	   a[0], a[1], a[2], a[3]);
    // not printf("200, 400, 500, 302")

    b = (int *) a + 1;
    c = (int *) ((char *) a + 1);
    printf("6: a = %p, b = %p, c = %p\n", a, b, c);
}

int
main(int ac, char **av)
{
    f();
    return 0;
}
