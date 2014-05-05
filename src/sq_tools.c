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

/*API to print all queue elements ,assuming each node data is char string*/
Q_RETURN_t 
print_sq(CB_Q *p_queue){
    Q_RETURN_t ret = Q_SUCCESS;	
    P_r(&p_queue->lock);
    if(sq_isempty(p_queue)){
        V_r(&p_queue->lock);
        SQ_LOG(LOG_CRITICAL,"Queue is empty");
        goto end;
    }
	int tr_index=p_queue->front;
	printf("\n=======PRINTING Q ELEMENTS===========\n");
	while(tr_index!=p_queue->rear){
		printf(" %s ",(char *)(p_queue->q_array[tr_index].p_data));
		tr_index=(tr_index+1)%p_queue->q_capicity;		
		}
	printf(" %s\n",(char *)(p_queue->q_array[tr_index].p_data));
    V_r(&p_queue->lock);
	end:
		return ret;
	
}



