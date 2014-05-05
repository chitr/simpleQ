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



