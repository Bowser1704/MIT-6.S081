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

