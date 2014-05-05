


#define GET_MEMORY malloc
/*Using last 3 bits of pointers to store data  i.e if it is free or used 
   using only 2 states out of 7 possible states :)*/

#define VAL_POINTER(p) (unsigned long)(0xffffffffffffff8UL & (unsigned long)(p))
#define VAL_DATA(p) (unsigned long)(7 & (unsigned long)(p))




