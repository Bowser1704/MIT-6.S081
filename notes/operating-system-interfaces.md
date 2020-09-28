## FAQ

1. You might wonder why fork and exec are not combined in a single call.

   > between the two, the shell has a chance to redirect the child’s I/O without disturbing the I/O setup of the main shell.

2. fork and exec immediately mabye cause the wastefulness (creating a duplicate process). how to avoid.

   > by using virtual memory techniques such as copy-on-write

3. why malloc reduces the number of sbrk() system calls that the program must perform

   > say, if you call malloc() to request 10 bytes memory, the implementation may use sbrk (or other system call like mmap) to request 4K bytes from OS. Then when you call malloc() next time to request another 10 bytes, it doesn't have to issue system call; it may just return some memory allocated by system call of the last time 4K.
   >
   > https://stackoverflow.com/questions/19676688/how-malloc-and-sbrk-works-in-unix

4. Does parent and child share anything?

   > Although fork copies the file descriptor table, **each underlying file offset** is shared between parent and child.

------

## Bell Labs and CSP Threads

https://swtch.com/~rsc/thread/

> That is, it is a widespread mistake to think only of concurrent programming as a means to increase performance, *e.g.*, to overlap disk I/O requests, to reduce latency by prefetching results to expected queries, or to take advantage of multiple processors.

--------

## pointer

1. If `int *p = (int*)100`, then `(int)p + 1` and `(int)(p + 1)` are different numbers: the first is `101` but the second is `104`. When adding an integer to a pointer, as in the second case, the integer is implicitly multiplied by the size of the object the pointer points to.

## Lab

sizeof vs sizeof() https://stackoverflow.com/questions/1393582/why-is-sizeof-considered-an-operator

> sizeof is a operator.