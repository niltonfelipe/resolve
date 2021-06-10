
#ifndef LIST_H
#define LIST_H

#include <sys/socket.h>  // struct sockaddr_storage

#include "domain.h"

struct host *
create_node ( struct sockaddr_storage *ss );

struct host *
search_node ( struct sockaddr_storage *ss );

void
insert_node_tail ( struct host *node );

// improves the position of the node in the list,
// raising its position according to criteria
void
reposition_node ( struct host *node );

void
remove_last_node ( void );

void
list_free ( void );

#endif  // LIST_H
