#ifndef SPINMEMORYMANAGEH
#define SPINMEMORYMANAGEH

#define memory_pool_size	40  

bool memory_pool_init();
void* spin_memory_malloc(uchar size);
bool spin_memory_free(void*);

#endif