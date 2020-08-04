//This file is an implementation of binsert that will allow to search for a key in an array and insert it in if not found.
#include "samples/prototypes.h"
#include <stdio.h>
#include <string.h>
//This generic function takes in a key, pointer to an array, a pointer to the number of elements in the array, the size of each element, and a comparison function pointer. The function builds upon an implementaiton of bsearch but also allows for the automatic insertion of the key if it is not found in the array already. The function returns a void * pointer to the newly added element or to the found key.
void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    
   
    void *end = (char *)base + *p_nelem * width;
    
    for (size_t nremain = *p_nelem; nremain != 0; nremain >>= 1) {
        void* p = (char *)base + (nremain >> 1) * width;
        int sign = compar(key, p);
        if (sign == 0) {
           
            return (char *)key;
        }
        if (sign > 0) {  /* key > p: move right */
            base = (char *)p + width;
            nremain--;
        }       /* else move left */
    }

    
    memmove((char *)base+width, base, (char *)end-(char*)base);
    memcpy((char *)base, key, width);
    
   
    p_nelem[0] ++;
    
    return base;

}
