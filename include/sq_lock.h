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
