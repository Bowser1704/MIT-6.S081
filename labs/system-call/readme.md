## Lab2 system-call

[system-call](https://pdos.csail.mit.edu/6.828/2020/labs/syscall.html)

### 1. trace

1. Add the API for the system call to the user

   - add a prototype for the system call to `user/user.h`

     ```c
     int trace(int);
     ```

   - a stub to `user/usys.pl`

     ```perl
     entry("trace");
     ```

     > The Makefile invokes the perl script `user/usys.pl`, which produces `user/usys.S`, the actual system call stubs, which use the RISC-V `ecall` instruction to transition to the kernel.

2. Add a system call number in the `kernel/syscall.h` 

   ```c
   #define SYS_trace 22
   ```

3. Add a pointer to the system call in the `kernel/syscall.c`

   ```c
   [SYS_trace]   sys_trace,
   ```

4. Add the function prototype in the `kernel/syscall.c`

   ```c
   extern uint64 sys_trace(void);
   ```

5. Add something for the special syscall `trace`

   - add proc structure add tracemask in the `kernel/proc.h`

     ```c
     int tracemask;  // for trace
     ```

   - modify `fork()` (see `kernel/proc.c`) to copy the trace mask from the parent to the child process.

     ```c
     +  // Copy the trace mask from parent to child
     +  np->tracemask = p->tracemask;
     +
        // copy saved user registers.
        *(np->trapframe) = *(p->trapframe);
     
     ```
     
   - Modify the `syscall()` function in `kernel/syscall.c` to print the trace output. You will need to add an array of syscall names to index into.

     ```c
     +// for trace to print syscall name
     +char * syscall_name[NELEM(syscalls)] = {
     +  "",  "fork", "exit", "wait", "pipe",
     +  "read", "kill", "exec", "fstat", "chdir",
     +  "dup", "getpid", "sbrk", "sleep", "uptime",
     +  "open", "write", "mknod", "unlink", "link",
     +  "mkdir", "close", "trace"
      };
   ```
     
     ```c
        num = p->trapframe->a7;
        if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
          p->trapframe->a0 = syscalls[num]();
     +    // print trace output
     +    if((1 << num) & p->tracemask)
     +      printf("%d: syscall %s -> %d\n",p->pid,syscall_name[num],p->trapframe->
        } else {
          printf("%d %s: unknown sys call %d\n",
                  p->pid, p->name, num);
     ```

6. Implementing the function in the `kernel/sysproc.c`

   ```c
   +
   +uint64
   +sys_trace(void)
   +{
   +  int n;
   +  if(argint(0, &n) < 0)
   +    return -1;
   +  myproc()->tracemask = n;
   +  return 0;
   +}
   ```

### 2. sysinfo

1. Add `$U/_sysinfotest` to UPROGS in Makefile

2. Add the API for the system call to the user

      - add a prototype for the system call to `user/user.h`

        ```c
        struct sysinfo;
        int sysinfo(struct sysinfo *);
        ```

      - a stub to `user/usys.pl`

        ```perl
        entry("sysinfo");
        ```

3. Add a system call number in the `kernel/syscall.h` 

   ```c
   #define SYS_sysinfo 23
   ```

4. Add a pointer to the system call in the `kernel/syscall.c`

   ```c
   [SYS_sysinfo]   sys_sysinfo,
   ```

5. Add the function prototype in the `kernel/syscall.c`

   ```c
   extern uint64 sys_sysinfo(void);
   ```

6. Add something for the special syscall `sysinfo`

   1. add a function to `kernel/kalloc.c`

      ```c
      // kernel/defs.h
      +uint64          kfreemem(void);
      
      // kernel/kalloc.c
      +
      +uint64
      +kfreemem()
      +{
      +  uint64 fpn = 0; //free page num
      +  struct run *r;
      +
      +  acquire(&kmem.lock);
      +  r = kmem.freelist;
      +  while(r) {
      +    fpn += 1;
      +    r = r->next;
      +  }
      +  release(&kmem.lock);
      +
      +  return fpn * PGSIZE;  //PGSIZE is defined in riscv.h
      +}
      ```

   2. add a function to `kernel/proc.c`

      ```c
      // kernel/defs.h
       // proc.c
      +uint64          knproc(void);
      
      // kernel/proc.c
      +uint64
      +knproc(void)
      +{
      +  uint64 up = 0; //the nproc field should be set to the number of processes whose state is not UNUSED
      +  struct proc *p;
      +  for(p = proc; p < &proc[NPROC]; p++){
      +    if(p->state != UNUSED)
      +      up++;
      +  }
      +  return up;
      +}
      ```

7. Implementing the function in the `kernel/sysproc.c`

   ```c
   +#include "sysinfo.h"
   
   +uint64
   +sys_sysinfo(void)
   +{
   +  uint64 addr;
   +  // argaddr(1, &addr) is wrong
   +  if (argaddr(0, &addr) < 0)
   +    return -1;
   +
   +  struct proc *p = myproc();
   +  struct sysinfo si;
   +  si.freemem = kfreemem();
   +  si.nproc = knproc();
   +
   +  if (copyout(p->pagetable, addr, (char *)&si, sizeof(si)) < 0)
   +    return -1;
   +  return 0;
   +}
   ```

   

