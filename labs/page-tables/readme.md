## Lab 3: Page Tables

### 1. print page table

1. add prototype to `kernel/defs.h`

   ```c
    // vm.c
   +void            vmprint(pagetable_t);
   ```

2. add vmprint call in `kernel/exec.c`

   ```c
   +  // print init process page table
   +  if (p->pid == 1) {
   +    vmprint(p->pagetable);
   +  }
   +
      return argc; // this ends up in a0, the first argument to main(argc, argv)
   ```

3. add the macros at the end of the file `kernel/riscv.h`

   ```c
   +#define PAGETABLE_READ 512
   ```

4. Implement vmprint in `kernel/vm.c`

   ```c
   +/*
   + * prints the content of page tables
   + */
   +void
   +vmprint(pagetable_t pd_l2)
   +{
   +  printf("page table %p\n", pd_l2);
   +  int i, j, k;
   +  for(i = 0; i < PAGETABLE_READ; i++) {
   +    pagetable_t pd_l1 = (pagetable_t)PTE2PA(pd_l2[i]);
   +    // PTE is valid or not
   +    if (pd_l1 && PTE_V) {
   +      printf("..");
   +      printf("%d: pte %p pa %p\n", i, pd_l2[i], pd_l1);
   +      for(j = 0; j < PAGETABLE_READ; j++) {
   +        pagetable_t pd_l0 = (pagetable_t)PTE2PA(pd_l1[j]);
   +        // PTE is valid or not
   +        if (pd_l0 && PTE_V) {
   +          printf(".. ..");
   +          printf("%d: pte %p pa %p\n", j, pd_l1[j], pd_l0);
   +          for(k = 0; k < PAGETABLE_READ; k++) {
   +            pagetable_t pte = (pagetable_t)PTE2PA(pd_l0[k]);
   +            // PTE is valid or not
   +            if (pte && PTE_V) {
   +              printf(".. .. ..");
   +              printf("%d: pte %p pa %p\n", k, pd_l0[k], pte);
   +            }
   +          }
   +        }
   +      }
   +    }
   +  }
   +}
   +
   ```

### 2. A kernel page table per process

`proc_pagetable` create a user page table for the given process.

…..

[code](https://github.com/Bowser1704/MIT-6.S081/tree/pgtbl)

