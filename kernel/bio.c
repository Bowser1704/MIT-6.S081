// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#define NBUCKET 13

struct {
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // Sorted by how recently the buffer was used.
  // head.next is most recent, head.prev is least.
  struct buf head[NBUCKET];
  struct spinlock lock[NBUCKET];
} bcache;

void
binit(void)
{
  struct buf *b;
  
  for(int i=0; i<NBUCKET; i++) {
    initlock(&bcache.lock[i], "buf hash table");
  }

  // Create linked list of buffers
  for(int i=0; i<NBUCKET; i++){
    bcache.head[i].prev = &bcache.head[i];
    bcache.head[i].next = &bcache.head[i];
  }
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    b->next = bcache.head[0].next;
    b->prev = &bcache.head[0];
    initsleeplock(&b->lock, "buffer");
    b->next->prev = b;
    b->prev->next = b;
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;

  uint key = blockno % NBUCKET;
  acquire(&bcache.lock[key]);
  // Is the block already cached?
  for(b = bcache.head[key].next; b != &bcache.head[key]; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      release(&bcache.lock[key]);
      acquiresleep(&b->lock);
      return b;
    }
  }

  // Not cached.
  // Recycle the least recently used (LRU) unused buffer.
  uint new_key = key;
  while (1) {
    new_key = (new_key + 1) % NBUCKET;
    if (new_key == key) continue;
    acquire(&bcache.lock[new_key]);
    for(b = bcache.head[new_key].prev; b != &bcache.head[new_key]; b = b->prev){
      if(b->refcnt == 0) {
        b->dev = dev;
        b->blockno = blockno;
        b->valid = 0;
        b->refcnt = 1;
        // delete b from table[new_key]
        b->prev->next = b->next;
        b->next->prev = b->prev;
        release(&bcache.lock[new_key]);

        // insert b into table[key]
        b->prev = &bcache.head[key];
        b->next = bcache.head[key].next;
        b->next->prev = b;
        b->prev->next = b;

        release(&bcache.lock[key]);
        acquiresleep(&b->lock);
        return b;
      }
    }
    release(&bcache.lock[new_key]);
  }
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);

  uint key = b->blockno % NBUCKET;
  acquire(&bcache.lock[key]);
  b->refcnt--;
  if (b->refcnt == 0) {
    // no one is waiting for it.
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head[key].next;
    b->prev = &bcache.head[key];
    b->next->prev = b;
    b->prev->next = b;
  }
  release(&bcache.lock[key]);
}

void
bpin(struct buf *b) {
  uint key = b->blockno % NBUCKET;
  acquire(&bcache.lock[key]);
  b->refcnt++;
  release(&bcache.lock[key]);
}

void
bunpin(struct buf *b) {
  uint key = b->blockno % NBUCKET;
  acquire(&bcache.lock[key]);
  b->refcnt--;
  release(&bcache.lock[key]);
}


