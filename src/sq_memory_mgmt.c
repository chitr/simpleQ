/*
 * vim:expandtab:shiftwidth=4:tabstop=4:
 *
 * Copyright   (2014)      Contributors
 * Contributor : chitr   chitr.prayatan@gmail.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * ---------------------------------------
 */

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


