## Traps and system calls

32 registers.

CPU control registers:

- satp -- page table physical address owe of direct-mapping.
- stvec -- ecall jumps here in kernel; points to trampoline
- sepc -- ecall saves user pc here
- sscratch -- address of trapframe

ecall

- user mode to supervisor mode
- save pc to sepc
- Jump to stvec/ set stevc to pc

why ecall doesn’t do more things?  Flexibility.

trampoline 

- swap a0 and sscratch

  sscratch is a pointer to trapframe.

- save the register to trapframe. 

  a0 is trapframe.

  Tp is heart id.

  sret instruction

  load kernel_stack to sp 

- switch pagetable from user to kernel.  

  It still work, because trampoline is mapped in the same location between user space and kernel space.

- go to trap.c

syscall

- The user code places the arguments for exec in registers a0 and a1, **and puts the system call number in a7.** Return value is in a0.

- get system call number from `kernel/syscall.h`

  `less user/usys.S`

  ```assembly
  #include "kernel/syscall.h"
  .global fork
  fork:
   li a7, SYS_fork
   ecall
   ret
  ```

- put arguments in a0-a8, and trampoline save these registers in trapframe.

------

- add some test file to kernel directory

  all these code in in kernel space. So we don’t have any of those annoying problems setting breakpoints.

- gdb

  tui enable

  layout reg/asm/split/source

  focus asm/reg 

  info breakpoints/ args/ locals.

  p *argv@2 / *argv@argc

  apropos info/…

- load vs store

  load: mem to reg

  store: reg to mem

- caller saver vs callee saver

  caller: temporary quantities，not preserved cross function call, function b can overwrite it.

  callee: preserved cross function call, worry about it.When the caller makes a procedure call, it can expect that those registers will hold the same value after the callee returns，or do not touch them.

- sp vs fp/s0

  stack grow downwards always.

  sp is bottom of current stack frame. / stack pointer.

  fp is top of current stack frame. / frame pointer, it is similar to ebp in Linux, base of current stack frame.

- auipc and jalr

  add upper immediate to pc 

  > The AUIPC instruction, which adds a 20-bit upper immediate to the PC, replaces the RDNPC instruction, which only read the current PC value. This results in significant savings for position-independent code.

  jump and link register

  > The AUIPC instruction supports two-instruction sequences to access arbitrary offsets from the PC for both control-flow transfers and data accesses. The combination of an AUIPC and the 12-bit immediate in a JALR can transfer control to any 32-bit PC-relative address, while an AUIPC plus the 12-bit immediate offset in regular load or store instructions can access any 32-bit PC-relative data address.

## Lab

- It’s no need to cast pointer to function pointer, we just set function address to `p->trapframe->epc`.

- what registers do you need to save and restore to resume the interrupted code correctly? 

  have a look fo trampoline.S for a example: `save the user registers in TRAPFRAME`

  ```assembly
  # save the user registers in TRAPFRAME
           sd ra, 40(a0)
           sd sp, 48(a0)
           sd gp, 56(a0)
           sd tp, 64(a0)
           sd t0, 72(a0)
           sd t1, 80(a0)
           sd t2, 88(a0)
           sd s0, 96(a0)
           sd s1, 104(a0)
           sd a1, 120(a0)
           sd a2, 128(a0)
           sd a3, 136(a0)
           sd a4, 144(a0)
           sd a5, 152(a0)
           sd a6, 160(a0)
           sd a7, 168(a0)
           sd s2, 176(a0)
           sd s3, 184(a0)
           sd s4, 192(a0)
           sd s5, 200(a0)
           sd s6, 208(a0)
           sd s7, 216(a0)
           sd s8, 224(a0)
           sd s9, 232(a0)
           sd s10, 240(a0)
           sd s11, 248(a0)
           sd t3, 256(a0)
           sd t4, 264(a0)
           sd t5, 272(a0)
           sd t6, 280(a0)
  ```

- you'll be implementing a primitive form of user-level interrupt/fault handlers;

## Reference

- https://www.cs.miami.edu/home/burt/learning/Csc421.171/workbook/stack-memory.html