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

#include <pthread.h>


#define P( mutex )                                                          \
  do { int rc ;                                                             \
    if( ( rc = pthread_mutex_lock( &mutex ) ) != 0 )                        \
      SQ_LOG(LOG_CRITICAL, "  --> Error P: %d %d", rc, errno );  \
  } while (0)

#define V( mutex )                                                          \
  do { int rc ;                                                             \
    if( ( rc = pthread_mutex_unlock( &mutex ) ) != 0 )                      \
      SQ_LOG(LOG_CRITICAL, "  --> Error V: %d %d", rc, errno );  \
  } while (0)

/* Type representing the lock itself */
typedef struct _RW_LOCK
{
  unsigned int nbr_active;
  unsigned int nbr_waiting;
  unsigned int nbw_active;
  unsigned int nbw_waiting;
  pthread_mutex_t mutexProtect;
  pthread_cond_t condWrite;
  pthread_cond_t condRead;
  pthread_mutex_t mcond;
} rw_lock_t;

int rw_lock_init(rw_lock_t * plock);
int rw_lock_destroy(rw_lock_t * plock);
int P_w(rw_lock_t * plock);
int V_w(rw_lock_t * plock);
int P_r(rw_lock_t * plock);
int V_r(rw_lock_t * plock);
