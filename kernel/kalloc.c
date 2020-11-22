// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem[NCPU];

int
curcpuid(){
  push_off();
  int cur = cpuid();
  pop_off();
  return cur;
}

struct run*
steal(int cur)
{
  struct run *r;
  for(int i = 0; i < NCPU; i++) {
    if(cur == i) continue;
    acquire(&kmem[i].lock);
    r = kmem[i].freelist;
    if(r) {
      kmem[i].freelist = r->next;
      release(&kmem[i].lock);
      break;
    }
    release(&kmem[i].lock);
  }
  return r;
}

void
kinit()
{
  for(int i = 0; i < NCPU; i++) {
    initlock(&kmem[i].lock, "kmem");
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
  
  int cur = curcpuid();

  acquire(&kmem[cur].lock);
  r->next = kmem[cur].freelist;
  kmem[cur].freelist = r;
  release(&kmem[cur].lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  int cur = curcpuid();

  acquire(&kmem[cur].lock);
  r = kmem[cur].freelist;
  if(r)
    kmem[cur].freelist = r->next;
  release(&kmem[cur].lock); 
  
  if(!r)
   r = steal(cur); 

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
