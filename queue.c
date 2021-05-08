
#include <stdlib.h> // for malloc

struct queue
{
  void *data;
  struct queue *next;
};

static struct queue *queue_head = NULL;
static struct queue *queue_tail = NULL;

static struct queue *
create_element ( void *data )
{
  struct queue *e = malloc ( sizeof ( *e ) );
  if ( !e )
    return NULL;

  e->data = data;
  e->next = NULL;

  return e;
}

int
enqueue ( void *data )
{
  struct queue *element = create_element ( data );
  if ( !element )
    return -1;

  if ( !queue_head )
    {
      queue_head = element;
      queue_tail = element;
    }
  else
    {
      queue_tail->next = element;
      queue_tail = element;
    }

  return 0;
}

void *
dequeue ( void )
{
  void *data = queue_head->data;

  void *t = queue_head;
  queue_head = queue_head->next;
  free ( t );

  return data;
}
