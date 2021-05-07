
#include <pthread.h>
#include <limits.h>  // for PTHREAD_STACK_MIN

#define DEFAULT_NUM_WORKERS 3
#define LEN_QUEUE_TASK 256

struct task
{
  void ( *func_task ) ( void * );
  void *args;
};

static struct task task_queue[LEN_QUEUE_TASK];
static unsigned int task_count = 0;

static pthread_mutex_t mutex_queue = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_queue = PTHREAD_COND_INITIALIZER;

static void
execute_task ( struct task *task )
{
  task->func_task ( task->args );
}

static void *
start_workers ( __attribute__ ( ( unused ) ) void *args )
{
  struct task *task;
  while ( 1 )
    {
      pthread_mutex_lock ( &mutex_queue );

      // wait for jobs
      while ( !task_count )
        pthread_cond_wait ( &cond_queue, &mutex_queue );

      // task = &task_queue[current_task];
      task = &task_queue[0];
      for ( unsigned int i = 0; i < task_count - 1; i++ )
        task_queue[i] = task_queue[i + 1];

      task_count--;

      pthread_mutex_unlock ( &mutex_queue );

      execute_task ( task );
    }

  pthread_exit ( NULL );
}

int
init_workers ( unsigned int num_workers )
{
  num_workers = ( num_workers > 0 ) ? num_workers : DEFAULT_NUM_WORKERS;

  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init ( &attr );
  pthread_attr_setstacksize ( &attr, PTHREAD_STACK_MIN );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_DETACHED );

  while ( num_workers-- )
    {
      if ( pthread_create ( &tid, &attr, start_workers, NULL ) )
        return 0;
    }

  pthread_attr_destroy ( &attr );

  return 1;
}

void
add_task_queue ( void ( *func ) ( void * ), void *args )
{
  // struct task task = { .func_task = func, .args = args };

  pthread_mutex_lock ( &mutex_queue );
  task_queue[task_count] = ( struct task ){
          .func_task = func, .args = args };  // test compund literal here after
  task_count++;
  pthread_mutex_unlock ( &mutex_queue );

  // wake up a thread to work
  pthread_cond_signal ( &cond_queue );
}
