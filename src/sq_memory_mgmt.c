#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void * default_memory_alloc(void * size){
uint64_t *p_memsize=(uint64_t)(size);
return malloc(*p_memsize);
}
void * default_memory_free(void * p_mem){
 free(p_mem);
 p_mem=NULL;
 return p_mem;
}


