#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "sq_generic.h"
#include "log_helper.h"
#include "sq_memory_mgmt.h"
#include "sq_Qop.h"

/*User is supposed to provide memory management functions as p_get_memory & p_free_memory 
   if these values are NULL default memory mgmt functions are used*/
Q_RETURN_t 
sq_init(CB_Q *p_queue,
         uint64_t q_size,
         cb_process p_get_memory,
         cb_process p_free_memory){
    Q_RETURN_t ret=Q_SUCCESS;
    if(rw_lock_init(&(p_queue->lock)) != 0){
        SQ_LOG(LOG_CRITICAL,"Unable to intialize reader write log for queue");
        ret = Q_ERROR;
        goto end;
    }
    P_w(&p_queue->lock);
    if(!p_get_memory)
        p_get_memory=default_memory_alloc;

    if(!p_free_memory)
        p_free_memory=default_memory_free;

	p_queue->q_capicity=q_size;
    p_queue->front=p_queue->rear = -1;

    q_size=q_size * sizeof(Q_NODE);

    p_queue->get_memory=p_get_memory;
	p_queue->free_memory=p_free_memory;
    p_queue->q_array = ((p_get_memory)(&q_size));
    if(!p_queue->q_array){
        SQ_LOG(LOG_CRITICAL,"Unable to allocate memory for queue of size %l",q_size);
        ret = Q_MEM_ERROR;
        V_w(&p_queue->lock);
        goto end;
    }
    
    V_w(&p_queue->lock);
end:
    return ret;
}

Q_RETURN_t 
sq_enqueue(CB_Q *p_queue,
               Q_NODE *p_node){
    Q_RETURN_t ret=Q_SUCCESS;
    P_r(&p_queue->lock);
    if(sq_isfull(p_queue)){
        V_r(&p_queue->lock);
        SQ_LOG(LOG_CRITICAL,"Fail to enqueue ,Queue is full ");
        ret=Q_ERROR;
        goto end;
    }
    V_r(&p_queue->lock);
    P_w(&p_queue->lock);
    p_queue->rear = (p_queue->rear+1)% p_queue->q_capicity;
    p_queue->q_array[p_queue->rear].size=p_node->size;
    p_queue->q_array[p_queue->rear].p_data=p_node->p_data;
    if(p_queue->front == -1)
        p_queue->front = p_queue->rear;
    V_w(&p_queue->lock);		
end:
    return ret;
}
Q_NODE *
sq_dequeue(CB_Q *p_queue){
    Q_NODE *p_node = NULL;	
    P_r(&p_queue->lock);
    if(sq_isempty(p_queue)){
        V_r(&p_queue->lock);
        SQ_LOG(LOG_CRITICAL,"Fail to dequeue ,Queue is empty");
        goto end;

    }
    V_r(&p_queue->lock);
    P_w(&p_queue->lock);
	uint64_t node_size=sizeof(Q_NODE);
    p_node = (Q_NODE*)(p_queue->get_memory)(&node_size);
    if(!p_node){
        SQ_LOG(LOG_CRITICAL,"Memory Error while dequeue");
        V_w(&p_queue->lock);
        goto end;		
    }
    p_node->p_data=p_queue->q_array[p_queue->front].p_data;
    p_node->size=p_queue->q_array[p_queue->front].size;
    if(p_queue->front == p_queue->rear)
        p_queue->front = p_queue->rear = -1;
else
    p_queue->front = (p_queue->front + 1)%p_queue->q_capicity;
        V_w(&p_queue->lock);
end:
    return p_node;
}
int  
sq_isempty(CB_Q *p_queue){
    /*Caller should ensure it has reader lock for queue */
    return (p_queue->front== -1);
}
int  
sq_isfull(CB_Q *p_queue){
	/*Caller should ensure it has reader lock for queue */
return ((p_queue->rear +1) % p_queue->q_capicity == p_queue->front);
}
int 
sq_size(CB_Q *p_queue){
    /*Caller should ensure it has reader lock for queue */
    return ((p_queue->q_capicity - p_queue->front +p_queue->rear +1)% p_queue->q_capicity);
}
Q_RETURN_t 
sq_destroy(CB_Q *p_queue){
    Q_RETURN_t ret=Q_SUCCESS;
    P_w(&p_queue->lock);
    /*Caller should make sure it has freed all data of individual queue nodes*/
    if(p_queue->q_array)
        ((p_queue->free_memory)(p_queue->q_array));
    V_w(&p_queue->lock);
	/*comment to avoid freeing the locks */
	if(rw_lock_destroy(&p_queue->lock)){
		SQ_LOG(LOG_CRITICAL,"Unable to destroy the locks ");
		ret=Q_ERROR;		
		}
    return ret;
}






