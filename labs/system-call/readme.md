## Lab2 system-call

[system-call](https://pdos.csail.mit.edu/6.828/2020/labs/syscall.html)

### 1. trace

1. add a system call number in the `kernel/syscall.h` 

   ```c
   #define SYS_trace 22
   ```

2. add a pointer to the system call in the `kernel/syscall.c`

   ```c
   [SYS_trace]   sys_trace,
   ```

3. add the function prototype in the `kernel/syscall.c`

   ```c
   extern uint64 sys_trace(void);
   ```

4. add something for the special syscall `trace`

   - add proc structure add tracemask in the `kernel/proc.h`

     ```c
     int tracemask;  // for trace
     ```

   - modify `fork()` (see `kernel/proc.c`) to copy the trace mask from the parent to the child process.

     ```c
     int
     fork(void)
     {
         ...
     +   // Copy the trace mask from parent to child
     +   np->tracemask = p->tracemask;
         ...
     }
     ```

   - Modify the `syscall()` function in `kernel/syscall.c` to print the trace output. You will need to add an array of syscall names to index into.

     ```c
     char * syscall_name[NELEM(syscalls)] = {
       "",  "fork", "exit", "wait", "pipe",
       "read", "kill", "exec", "fstat", "chdir",
       "dup", "getpid", "sbrk", "sleep", "uptime",
       "open", "write", "mknod", "unlink", "link",
       "mkdir", "close", "trace"
     };
     ```

     ```c
     void
     syscall(void)
     {
     ...
         if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
             p->trapframe->a0 = syscalls[num]();
     +       // print trace output
     +       if((1 << num) & p->tracemask)
     +						// p->trapframe->a0 syscall return value
     +           printf("%d: syscall %s -> %d\n",p->pid, syscall_name[num], p->trapframe->a0);
         } else {
     ...
         }
     }
     ```

5. Implement the function in the `kernel/sysproc.c`

   ```c
   uint64
   sys_trace(void)
   {
     int n;
     if(argint(0, &n) < 0)
       return -1;
     myproc()->tracemask = n;
     return 0;
   }
   ```

6. add the API for the system call to the user

   - add a prototype for the system call to `user/user.h`

     ```c
     int trace(int);
     ```

   - a stub to `user/usys.pl`

     ```perl
     entry("trace");
     ```

     > The Makefile invokes the perl script `user/usys.pl`, which produces `user/usys.S`, the actual system call stubs, which use the RISC-V `ecall` instruction to transition to the kernel.

