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

#include "sq_lock.h"
#define DEFAULT_MEMORY_FUNC malloc


/*Enumerate for possible values of return type */
typedef enum Q_RETURN{
    Q_SUCCESS=0,
	Q_MEM_ERROR,	
    Q_ERROR
}Q_RETURN_t;


typedef struct queue_node{
    uint64_t size;        /*size of data buffer of queue node*/
    unsigned char *p_data;/* data buffer pointer*/
}Q_NODE;


/*Generic call back function 
    Used for passing user define memory mgmt function ,allocate & free memory*/
typedef void* (*cb_process)(void * cb_arg);


/*Since size of queue is fixed & user defined ,using cyclic buffer queue with  array 
   if there no predetermined Max limit of queue link-list implementation should be used  */
typedef struct cyclic_buffer_queue{
    int front,rear;
    uint64_t  q_capicity; /* Max element limit for queue*/
    Q_NODE    *q_array;   /* Q Node array */
    rw_lock_t lock;       /* A reader-writter lock used to protect the data 
                             Current for enqueue & dqueue only write lock is used 
                             reader lock could be used if queue is traversed/ready only */
    cb_process get_memory; /*Caller defined memory mgmt function*/ 
	cb_process free_memory; /*Caller defined memory mgmt function*/  						 
}CB_Q;

typedef struct queue_op{
	Q_RETURN_t (*pfn_init)
		       (CB_Q *p_queue,uint64_t q_size,cb_process p_get_memory);		       
	Q_RETURN_t (*pfn_enqueue)
	           (CB_Q *p_queue,Q_NODE *q_node);
	Q_RETURN_t (*pfn_destroy)
	            (CB_Q *p_queue);
	Q_NODE*    (*pfn_dqueue)
	           (CB_Q *p_queue);	
	int        (*pfn_isempty)
		       (CB_Q *p_queue);
	int        (*pfn_isfull)
		       (CB_Q *p_queue);
    int        (*pfn_size)
		       (CB_Q *p_queue);
}Q_OP;

typedef struct simple_queue{
    CB_Q simple_q;
    Q_OP queue_op;/*Currently all queue op function are mapped to sq op default functions
	                          We can add next version functions here and do transition of code with affecting
	                          older functionalities or version q op APIS could be maintained TODO */
}simple_queue_t;
