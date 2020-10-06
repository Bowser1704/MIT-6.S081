#include "sysinfo.h"

// for sysinfo system call 
int
sysinfo (uint64 addr) 
{
  struct proc *p = myproc();
  struct sysinfo si;
  si.freeem = kfreemem();
  si.nproc = knproc();
  if (copyout(p->pagetable, addr, (char *)&si, sizeof(si)) < 0)
    return -1;
  return 0;
}
