// double linked list
// generic data store

#include <stdlib.h> // malloc, free
#include <assert.h>
#include "list.h"

static struct list_node *
create_node( void *data )
{
  struct list_node *n = malloc( sizeof *n );

  if ( n )
    {
      n->data = data;
      n->prev = NULL;
    }

  return n;
}

// push head
struct list_node *
list_push ( struct list *list, void *data )
{
  struct list_node *new_node = create_node ( data );

  if ( new_node )
    {
      list->size++;

      if ( list->head )
        list->head->prev = new_node;
      else
        list->tail = new_node;

      new_node->next = list->head;

      list->head = new_node;
    }

  return new_node;
}

void
list_delete( struct list *list, struct list_node *node )
{
  assert( node != NULL );

  list->size--;

  if ( node->next )
    node->next->prev = node->prev;

  if ( node->prev )
    node->prev->next = node->next;

  if ( list->head == node )
    list->head = node->next;

  free( node );
}
