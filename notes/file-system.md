## File System

Question:

1. why `b->refcnt` is non zero?

   ```c
   // Release a locked buffer.
    // Move to the head of the most-recently-used list.
   void
     brelse(struct buf *b)
   {
   ……
     releasesleep(&b->lock);
   
     acquire(&bcache.lock);
     b->refcnt--;
   ……
    }
   ```

   the cur process is using it, so `b->refcnt` is must greater than 1.

   > the non-zero b->refcnt prevents the buffer from being re-used for a different disk block.