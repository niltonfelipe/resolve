
/*
 *  Copyright (C) 2021 Mayco S. Berghetti
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>  // malloc
#include <string.h>  // memcpy

#include "domain.h"     // struct host
#include "sock_util.h"  // check_addr_equal

static struct host *head = NULL, *last = NULL;

static void
remove_first ( struct host *node )
{
  head = node->next;
  head->prev = NULL;
}

static void
remove_middle ( struct host *node )
{
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

static void
remove_last ( struct host *node )
{
  last = node->prev;
  last->next = NULL;
}

// remove node from current position, but not free node
static void
remove_node ( struct host *node )
{
  if ( !node )
    return;

  if ( !node->prev )
    {
      // only one element in list
      if ( !node->next )
        last = head = NULL;
      // first element
      else
        remove_first ( node );
    }
  // last element
  else if ( !node->next )
    remove_last ( node );
  else
    remove_middle ( node );
}

static void
insert_before ( struct host *node, struct host *before )
{
  if ( !before )
    return;

  struct host *tmp = before->prev;

  if ( !tmp )
    {
      head = node;  // first element
      head->prev = NULL;
    }
  else
    {
      tmp->next = node;
      node->prev = tmp;
    }

  node->next = before;
  before->prev = node;
}

struct host *
create_node ( struct sockaddr_storage *ss )
{
  struct host *node = malloc ( sizeof *node );

  if ( node )
    {
      memcpy ( &node->ss, ss, sizeof ( *ss ) );
      node->status = RESOLVING;
      node->hit = 0;
      node->next = NULL;
      node->prev = NULL;
    }

  return node;
}

void
insert_node_tail ( struct host *node )
{
  if ( !head )
    {
      head = last = node;
    }
  else
    {
      last->next = node;
      node->prev = last;
      last = node;
    }
}

struct host *
search_node ( struct sockaddr_storage *ss )
{
  struct host *node = head;

  while ( node )
    {
      if ( check_addr_equal ( ss, &node->ss ) )
        break;

      node = node->next;
    }

  return node;
}

void
reposition_node ( struct host *node )
{
  struct host *p = head;

  while ( p )
    {
      if ( p->hit <= node->hit )
        break;

      p = p->next;
    }

  if ( p != node )
    {
      remove_node ( node );
      insert_before ( node, p );
    }
}

void
remove_last_node ( void )
{
  struct host *node = last;

  if ( head )
    {
      // only one element
      if ( head == last )
        head = last = NULL;
      else
        remove_last ( node );
    }

  free ( node );
}

void
list_free ( void )
{
  struct host *tmp;

  while ( head )
    {
      tmp = head->next;

      free ( head );

      head = tmp;
    }
}
