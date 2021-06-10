#ifndef LIST_H
#define LIST_H

struct list_node
{
  void *data;

  struct list_node *next;
  struct list_node *prev;
};

struct list
{
  struct list_node *head;
  struct list_node *tail;
  unsigned int size;
};

struct list_node *
list_push ( struct list *list, void *data );

void
list_delete( struct list *list, struct list_node *node );

#endif // LIST_H
