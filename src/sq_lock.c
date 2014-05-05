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

/*Implementation of reader-write lock */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sq_lock.h"
#include "log_helper.h"


/* 
 * Take the lock for reading 
 */
int P_r(rw_lock_t * plock){
    P(plock->mutexProtect);
    plock->nbr_waiting++;

    /* no new read lock is granted if writters are waiting or active */
    if(plock->nbw_active > 0 || plock->nbw_waiting > 0)
        pthread_cond_wait(&(plock->condRead), &(plock->mutexProtect));

    /* There is no active or waiting writters, readers can go ... */
    plock->nbr_waiting--;
    plock->nbr_active++;

    V(plock->mutexProtect);
    return 0;
}                              

/*
 * Release the lock after reading 
 */
int V_r(rw_lock_t * plock){
    P(plock->mutexProtect);
    /* I am a reader that is no more active */
    plock->nbr_active--;

    /* I was the last active reader, and there are some waiting writters,  let one of them go */
    if(plock->nbr_active == 0 && plock->nbw_waiting > 0){      
        pthread_cond_signal(&plock->condWrite);
    }

    V(plock->mutexProtect);

    return 0;
}                            

/*
 * Take the lock for writting 
 */
int P_w(rw_lock_t * plock){
    P(plock->mutexProtect);

    plock->nbw_waiting++;

    /* nobody must be active obtain exclusive lock */
    while(plock->nbr_active > 0 || plock->nbw_active > 0)
        pthread_cond_wait(&plock->condWrite, &plock->mutexProtect);

    /* I become active and no more waiting */
    plock->nbw_waiting--;
    plock->nbw_active++;

    V(plock->mutexProtect);
    return 0;
}                             
/*
 * Release the lock after writting 
 */
int V_w(rw_lock_t * plock){
    P(plock->mutexProtect);

    /* I was the active writter, I am not it any more */
    plock->nbw_active--;

    if(plock->nbw_waiting > 0){

        /* There are waiting writters, but no waiting readers, I let a writter go */
        pthread_cond_signal(&(plock->condWrite));

    }
    else if(plock->nbr_waiting > 0){
        /* if readers are waiting, let them go */
        pthread_cond_broadcast(&(plock->condRead));
    }
    V(plock->mutexProtect);
    return 0;
}                               /* V_w */

                    
/*
 * Routine for initializing a lock
 */
int rw_lock_init(rw_lock_t * plock){
    int rc = 0;
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    if((rc = pthread_mutexattr_init(&mutex_attr) != 0))
        return 1;
    if((rc = pthread_condattr_init(&cond_attr) != 0))
        return 1;

    if((rc = pthread_mutex_init(&(plock->mutexProtect), &mutex_attr)) != 0)
        return 1;

    if((rc = pthread_cond_init(&(plock->condRead), &cond_attr)) != 0)
        return 1;
    if((rc = pthread_cond_init(&(plock->condWrite), &cond_attr)) != 0)
        return 1;

    plock->nbr_waiting = 0;
    plock->nbr_active = 0;

    plock->nbw_waiting = 0;
    plock->nbw_active = 0;

    return 0;
}                            

/*
 * Routine for destroying a lock
 */
int rw_lock_destroy(rw_lock_t * plock){
    int rc = 0;

    if((rc = pthread_mutex_destroy(&(plock->mutexProtect))) != 0)
        return 1;

    if((rc = pthread_cond_destroy(&(plock->condWrite))) != 0)
        return 1;
    if((rc = pthread_cond_destroy(&(plock->condRead))) != 0)
        return 1;

    memset(plock, 0, sizeof(rw_lock_t));

    return 0;
}                           

