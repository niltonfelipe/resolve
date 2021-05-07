#ifndef THREAD_POLL_H
#define THREAD_POLL_H

int
init_workers ( unsigned int num_workers );

void
add_task_queue ( void ( *func ) ( void * ), void *args );

#endif  // THREAD_POLL_H
