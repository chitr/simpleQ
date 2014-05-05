
Q_RETURN_t
sq_init(CB_Q *p_queue,uint64_t q_size,cb_process p_get_memory,cb_process p_free_memory);
Q_RETURN_t 
sq_enqueue(CB_Q *p_queue,Q_NODE *p_node);
Q_NODE *
sq_dequeue(CB_Q *p_queue);
int  
sq_isempty(CB_Q *p_queue);
int  
sq_isfull(CB_Q *p_queue);
int 
sq_size(CB_Q *p_queue);
Q_RETURN_t 
sq_destroy(CB_Q *p_queue);
