# malloc
My implementation of the libc malloc, realloc and free functions using system mmap

##Features

  * Size segmentation: divides requests into SMALL, MEDIUM and LARGE, use pre-allocated zones for SMALL and MEDIUM requests
  
  * De-fragment freed spaces to optimize re-usage
  
  * Hexdump allocated memory functionality
  
  
  ## Functions implemented:
  
  * malloc -  allocates “size” bytes of memory and returns a pointer to the allocated memory
  
  * realloc - tries to change the size of the allocation pointed to by “ptr” to “size”, and returns “ptr”. If there is not enough room to enlarge the memory allocation pointed to by ptr, realloc() creates a new allocation, copies as much of the old data pointed to by “ptr” as will fit to the new allocation, frees the old allocation, and returns a pointer to the allocated memory.
  
  * free - deallocates the memory allocation pointed to by “ptr”. If “ptr”is a NULL pointer, no operation is performed
  
  * show_alloc_mem
