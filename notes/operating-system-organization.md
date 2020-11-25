## Three pieces ?

### Virtualization

Goals: isolation

- Processors provide user/kernel mode
    kernel mode: can execute "privileged" instructions
      e.g., setting kernel/user bit
      e.g., reprogramming timer chip
    user mode: cannot execute privileged instructions
    Run OS in kernel mode, applications in user mode
    [RISC-V has also an M mode, which we mostly ignore] — machine mode.
- Processors provide virtual memory
    Hardware provides page tables that translate virtual address to physical
    Define what physical memory an application can access
    OS sets up page tables so that each application can access only its memory

> If an application in user mode attempts to execute a privileged instruction, then the CPU doesn’t execute the instruction, but switches to supervisor mode so that supervisor-mode code can terminate the application, because it did something it shouldn’t be doing.
>
> it’s kind of funny.

### Kernel organization

micro/monolithic kernel

- monolithic kernel

  A downside of the monolithic organization is that the interfaces between different parts of the operating system are often complex

  In a monolithic kernel, a mistake is *fatal*, because an error in supervisor mode will often cause the kernel to fail.

- micro kernel

  ![image-20200917202121624](https://tva1.sinaimg.cn/large/007S8ZIlgy1gitwzpf7czj30ty0fmmyq.jpg)

### storage

> Xv6 uses page tables (which are implemented by hardware) to give each process its own ad- dress space. 
>
> implemented by hardware ???

![image-20200917203211696](https://tva1.sinaimg.cn/large/007S8ZIlgy1gitxawbbn2j30ty0nogn2.jpg)

> At the top of the address space xv6 reserves a page for a **trampoline** and a page mapping the process’s **trapframe** to switch to the kernel, as we will explain in Chapter 4.

----

how the fork function return twice;