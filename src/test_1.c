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




extern Q_RETURN_t 
print_sq(CB_Q *p_queue);

/*Basic test to verify the init,enqueue,dequeue,destroy APIs 
   It takes strings from command line ,init queue ,add to queue ,dequeue & destroy queue*/
int main(int argc ,char **argv){
    CB_Q test_q;
    int size_q=argc;
	if(argc <2){
		printf("Usage: %s <data-string1> <data-string2> <data-string3> ...",argv[0]);
			exit(1);		
		}
    if(sq_init(&test_q,size_q,NULL,NULL)){
        printf("Init failed for q");
        exit(0);
    }
    Q_NODE temp_node;
    int tr_index=1;
    printf("\n====Enqueue=======\n");
    for(;tr_index<size_q +2 ;tr_index++){ /*size_q + 1 is used to check q full case*/
        temp_node.p_data=argv[tr_index%size_q];
        temp_node.size=strlen(argv[tr_index%size_q])+1;
        if(sq_enqueue(&test_q,&temp_node)){
            printf("failed to add Node %s ",temp_node.p_data);

        }
        print_sq(&test_q);	
    }
    printf("\n====Enqueue=======\n");
    tr_index=0;/*Check or empty case error */
    for(;tr_index<size_q+2;tr_index++){
        Q_NODE *p_node=NULL;
        p_node = sq_dequeue(&test_q);

        if(!p_node){
            printf("\nfailed to get  Node\n");

        }else
            printf("\nDequeued Node is :%s \n",p_node->p_data);
        print_sq(&test_q);	

    }
    if(sq_destroy(&test_q)){
        printf("\n Error while destroying the Queue\n");
    }
}

