## Scheduling

```
there are other techniques for interleaving multiple tasks
  look up event-driven programming, or state machines
  threads are not the most efficient, but they are usually the most convenient
```

while

```c
// Process is done running for now.
// It should have changed its p->state before coming back.
c->proc = 0;
```

alert table add index `idx` ()

One goal is ensure that when `thread_schedule()` runs a given thread for the first time, the thread executes the function passed to `thread_create()`

How put func into thread?

```c
t->context.ra = (uint64)func;
t->context.sp = (uint64)t->stack + STACK_SIZE;
```

### QA

1. how the `fork()` systemcall return twice with different value.

   `fork()` invoke kernel syscall, then kernel create a process, set the child process’s trapframe `a0` register to 0, so child process `fork()` will return 0.

   ```c
   // Cause fork to return 0 in the child.
   np->trapframe->a0 = 0;
   ```

   And return process pid, then parent process `fork()` will return child pid.

   when the scheduler schedule child process, it store trapframe register value to real register, then the return value is 0.

   ```
   getting from one process to another involves multiple transitions:
       user -> kernel; saves user registers in trapframe
       kernel thread -> scheduler thread; saves kernel thread registers in context
       scheduler thread -> kernel thread; restores kernel thread registers from ctx
       kernel -> user; restores user registers from trapframe
       
   struct proc in proc.h
     p->trapframe holds saved user thread's registers
     p->context holds saved kernel thread's registers
     p->kstack points to the thread's kernel stack
     p->state is RUNNING, RUNNABLE, SLEEPING, &c
     p->lock protects p->state (and other things...)
   ```

   