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


#define MAX_THREAD 100
#define THREAD_STACK_SIZE  21164888

pthread_t test_thread[MAX_THREAD];
int THID[MAX_THREAD];




int nb_nodes=0;
CB_Q test_q;
char **pargv=NULL;



extern Q_RETURN_t 
print_sq(CB_Q *p_queue);





/*Its keep enqueuing the data to queue*/
void * enqueue_thread(void *tid){
    int *thid=(int*)(tid);
    int tr_index=1; 
    Q_NODE temp_node;
    printf("\n Enqueue thread: %d \n",*thid);
    for(;/*tr_index < nb_nodes*/;tr_index++){
        temp_node.p_data=pargv[tr_index%nb_nodes];
        temp_node.size=strlen(pargv[tr_index%nb_nodes])+1;

        if(sq_enqueue(&test_q,&temp_node)){
            printf("\nfailed to add Node %s \n",temp_node.p_data);

        }else
            printf("\nThread id:%d:Enqueued:%s\n",*thid,(char *)pargv[tr_index%nb_nodes]);
        sleep(1);
    }
}

/*It keeps dequeing data from queue*/
void * dequeue_thread(void * p_data){
    int *thid=(int*)(p_data);
    int tr_index=0;
    Q_NODE *p_node;
    printf("\n Dqueue thread: %d \n",*thid);
    for(;/*tr_index < nb_nodes*/;tr_index++){
        p_node = sq_dequeue(&test_q);

        if(!p_node){
            printf("\nfailed to get  Node\n");

        }else
            printf("\nTHREADID :%d :Dequeued Node is :%s \n",*thid,p_node->p_data);
        free(p_node);
        sleep(2);
    }
}

/*Keep printing data from queue*/
void * printer_thread(void *p_data){
    int *thid=(int*)(p_data);
    while(1){
        printf("\nPrinter thread %d\n",*thid);
        print_sq(&test_q);	
        sleep(4);
    }
}


void (*func_ptr[3])={printer_thread,enqueue_thread,dequeue_thread};


/*Starts mentioned no of enqueuer,dequeuer & printer threads MAX value 100*/
void start_threads(int nb_threads){
    int rc = 0;
    pthread_attr_t attr_thr;
    unsigned long q_count = 0;


    /* Init for thread parameter (mostly for scheduling) */
    pthread_attr_init(&attr_thr);

    pthread_attr_setscope(&attr_thr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr_thr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&attr_thr, THREAD_STACK_SIZE);

    /* Starting all of the threads */
    for(q_count = 0; q_count < nb_threads; q_count++)
    {
        THID[q_count]=q_count;

        if((rc = pthread_create(&test_thread[q_count], &attr_thr, func_ptr[q_count%3],
                        (void *)&THID[q_count]) != 0))
        {
            printf("\nThread create error: %d", rc);
            return ;
        }
        printf("\nCreated thread: %lu\n", q_count);
        pthread_detach(test_thread[q_count]);
    }
    pthread_exit(0);
    return ;
}

/*Basic test to verify the init,enqueue,dequeue,destroy APIs 
   It takes strings from command line ,init queue ,add to queue ,dequeue & destroy queue*/
int main(int argc ,char **argv){

    int size_q=argc+2;
    int num_threads=atoi(argv[1]);

    if(argc <3){
        printf("Usage: %s <no of mixed threas > <data-string1> <data-string2> <data-string3> ...",argv[0]);
        exit(1);		
    }


    if(sq_init(&test_q,size_q,NULL,NULL)){
        printf("Init failed for q");
        exit(0);
    }
    pargv=argv;
    nb_nodes=argc-1;
    start_threads(num_threads);
}

