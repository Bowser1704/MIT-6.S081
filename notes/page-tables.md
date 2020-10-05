## Page Tables

challenge:

- how to multiplex several memories over one physical memory?
  while maintaining isolation between memories.

- how does RISC-V paging hardware translate a va?

<img src="https://i.loli.net/2020/10/05/3GEyT7VMfcgS8wZ.png" alt="image-20201005154327247" style="zoom:50%;" />

1. satp register points to PA of top/L2 PD

2. top 9 bits index L2 PD to get PTE of L1 PD 

   by using 44-bit PPN (PPN can get a page number) to get physical address of L1 PD.
   
   > top 44 bits of PTE are top bits of physical address “physical page number”

….

## Virtual Memory

three pieces

- Caching

  Disk are also considered as physical memory.

- Memory management

  Every process has its own process address space and page table.

  ![image-20200929234455504](https://i.loli.net/2020/09/29/zUkqTQoWCj1D8hI.png)

- Memory protection

  Different process has different permissions to access same physical memory，and only can access physical memory which it has permissions.

> - 虚拟内存可以利用内存起到缓存的作用，提高进程访问磁盘的速度；
> - 虚拟内存可以为进程提供独立的内存空间，简化程序的链接、加载过程并通过动态库共享内存；
> - 虚拟内存可以控制进程对物理内存的访问，隔离不同进程的访问权限，提高系统的安全性

### Multi-Level Page Tables

> But if we had a 32-bit address space, 4 KB pages, and a 4-byte PTE, then we would need a 4 MB page table resident in memory at all times, even if the application referenced only a small chunk of the virtual address space.

----------

## Lab

1. pageable_t is defined in `kernel/riscv.h`

   ```c
   typedef uint64 *pagetable_t; // 512 PTEs
   ```

   every pagetable has 512 PTEs.

-----

FAQ

1. 虚拟内存机制中 swap 分区是必须的吗？csapp 9.3。

2. 在 64 位的操作系统上，每个进程都会拥有 256 TiB 的内存空间，内核空间和用户空间分别占 128 TiB?

   >  48 位？2^48 = 256 TiB。2^57 = 128 PiB。

3. page size 4KB

   12 bit offset = 4k

## Ref

https://draveness.me/whys-the-design-os-virtual-memory

https://stackoverflow.com/questions/381244/purpose-of-memory-alignment