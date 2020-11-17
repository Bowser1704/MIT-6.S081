## Page faults

Eager

using page fault to implement COW (copy-on-write)

- It would be more efficient if the child and parent could share the parent’s physical memory.

Three  register. 

- scause the type of page fault

- stval the address that couldn’t be translated

- sepc what instruction that cause the page fault.

using page faults exception

Make mapping dynamic 

- Lazy alloction
- Zero filled
- COW forks
- Demand paging
- memory-mapped files.

vm  

1. isolation
2. Level of indirection

Lazy alloction

- Plus n to the p->sz
- Page fault —> allocate and map to the pgtbl.

   Don't allocat this page. why it make sense. 33 min

Zero filled

zero filled on demand / BSS (Uninitialized data (.bss))

- Map all zero-filled page to one physical page with read only bit.
- allocate a big array
- Exec fastly.

COW

- invariant, distinguish this is a COW page.

  RSW bit.

- make parents to  only READ its page.
- can’t free some physical page owe of children. physical page has a Ref count.

Demand paging.

- evict a page 

  least-recent-used, is not dirty (not written).

  Responds to some files

- what is dirty page?

  A page is dirty if it has been changed since it was loaded into memory.

- There are some bit in the pte

  D bit for dirty page

  A bit for accessed, some algorithms for this.

Memory mapped files

- map memeory to disk. steady

![image-20201021193223389](/Users/bowser/Library/Application Support/typora-user-images/image-20201021193223389.png)

## Labs

### Lazy allocation

1. why this page fault occurs?

   0x12a4 call a store instruction, it maybe called by malloc, and it triggered a exception which code is 15.

2. If you see the error "incomplete type proc", include "spinlock.h" **then** "proc.h".

   You can't change the order randomly.

3. why panic unmap.

4. how can i know it realy py address.

5. Handle the case in which a process passes a valid address from sbrk() to a system call such as read or write, but the memory for that address has not yet been allocated.

   How can i use kernel trap page fault.

6. Handle faults on the invalid page below the user stack.

### COW

1. usertests stacktest

	```c
   if (va <= PGROUNDDOWN(p->trapframe->sp) && va >= PGROUNDDOWN(p->trapframe->sp) - PGSIZE)
    	p->killed = 1;
	```

2. usertests sbrkfail

   ```c
   // check phsical address.
   if(pa == 0) return -1;  //for usertests sbrkfail.
   ```


3. usertests bigdir timeout.



## Reference

1. https://stackoverflow.com/questions/600079/git-how-do-i-clone-a-subdirectory-only-of-a-git-repository/52269934#52269934

   how to clone a subdirectory only in a GitHub repository.

2. 