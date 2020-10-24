## Page Tables

<img src="https://i.loli.net/2020/10/05/3GEyT7VMfcgS8wZ.png" alt="image-20201005154327247" style="zoom:50%;" />

1. satp register points to PA of top/L2 PD

2. top 9 bits index L2 PD to get PTE of L1 PD 

   by using 44-bit PPN (PPN can get a page number) to get physical address of L1 PD.
   
   > top 44 bits of PTE are top bits of physical address “physical page number”

….

## Virtual Memory

```  sh
page table 0x0000000087fa7000
..0: pte 0x0000000021fe8c01 pa 0x0000000087fa3000
.. ..0: pte 0x0000000021fe8801 pa 0x0000000087fa2000
.. .. ..0: pte 0x0000000021fe901f pa 0x0000000087fa4000
.. .. ..1: pte 0x0000000021fe840f pa 0x0000000087fa1000
.. .. ..2: pte 0x0000000021fe801f pa 0x0000000087fa0000
..255: pte 0x0000000021fe9801 pa 0x0000000087fa6000
.. ..511: pte 0x0000000021fe9401 pa 0x0000000087fa5000
.. .. ..510: pte 0x0000000021fed807 pa 0x0000000087fb6000
.. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
```

kernel address space

  ![image-20201008113457084](https://i.loli.net/2020/10/08/zaOiBr4vp5Swdtg.png)

  process address space

  ![image-20201008120126463](https://i.loli.net/2020/10/08/f5YiIloKm7DjLC3.png)

- Memory protection

  Different process has different permissions to access same physical memory，and only can access physical memory which it has permissions
  
- **[Memory-mapped I/O](https://en.wikipedia.org/wiki/Memory-mapped_I/O)**

>  So when an address is accessed by the CPU, it may refer to a portion of [physical RAM](https://en.wikipedia.org/wiki/Physical_memory), or it can instead refer to memory of the I/O device. Thus, the CPU instructions used to access the memory can also be used for accessing devices. 

#### Two special page tables

pagetable address 0x0000000087fa7000

- Trampoline

  Contains these instructions to basically transition and out of the kernel.

  user to kernel.

  255-511-511 page XV executable instructions

- Trapframe

  store some states when we jump to the kernel.

  kernel to user.

  255-511-510 page RWV

### Some hints

1. We only uses 38 bit of the virtual address in xv6.

2. put data and text in the same page just for simplicity.

3. one gigabyte is 0x40000000.

### Some QA

> why setting kernel stack high in the virtual address space. 

we put the kernel stack high, so the guard page is no need to map the physical memory, however, we hava to map it to the physical memory when guard page is in the low virtual address space owe of direct mapping.

## Lab

[code](https://github.com/Bowser1704/MIT-6.S081/tree/pgtbl)

1. pageable_t is defined in `kernel/riscv.h`

   ```c
   typedef uint64 *pagetable_t; // 512 PTEs
   ```

   every pagetable has 512 PTEs.

2. A kernel page table per process.

3. Simplify `copyin/copyinstr`userinit copy user pgtbl。

   we are going to put the user page table to the kernel l2 pd zero entry.


## Real World

three pieces

- Caching

  Disk are also considered as physical memory.

- Memory management

  Every process has its own process address space and page table.

  Figure 8.13 in CSAPP is Linux process address space.

  User space from 4G / 2^22 to 256T / 2^48.

  ![image-20200929234455504](https://i.loli.net/2020/09/29/zUkqTQoWCj1D8hI.png)

  **In xv6, every process has a user page table and all process share a single kernel page table which has a lot of independent kernel stack for every process.**
  
  > lab pgtbl has an assignments which we are required to create a kernel page table per process.

## Ref

https://stackoverflow.com/questions/381244/purpose-of-memory-alignment