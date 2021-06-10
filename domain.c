
/*
 *  Copyright (C) 2020-2021 Mayco S. Berghetti
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

#include <stdio.h>
#include <string.h>      // strncpy
#include <sys/socket.h>  // getnameinfo
#include <netdb.h>       // getnameinfo

#include "domain.h"
#include "thread_pool.h"
#include "list.h"
#include "sock_util.h"

// (2048 * sizeof(struct host)) == ~2.31 MiB cache of domain
#define DEFAULT_CACHE_ENTRIES 2048

static unsigned int cache_size = DEFAULT_CACHE_ENTRIES;

void
cache_domain_init ( unsigned int size )
{
  if ( size )
    cache_size = size;
}

void
cache_domain_free ( void )
{
  list_free ();
}

// run on thread
void
ip2domain_exec ( void *arg )
{
  struct host *host = ( struct host * ) arg;

  // convert ipv4 and ipv6
  // if error, convert to text ip same
  if ( getnameinfo ( ( struct sockaddr * ) &host->ss,
                     sizeof ( host->ss ),
                     host->fqdn,
                     sizeof ( host->fqdn ),
                     NULL,
                     0,
                     NI_DGRAM ) )
    {
      sockaddr_ntop ( &host->ss, host->fqdn, sizeof ( host->fqdn ) );
    }

  host->status = RESOLVED;
}

// return:
//  1 name resolved
//  0 name no resolved
int
ip2domain ( struct sockaddr_storage *ss, char *buff, const size_t buff_len )
{
  static unsigned int tot_hosts_cache = 0;

  struct host *host = search_node ( ss );
  if ( host )
    {
      if ( host->status == RESOLVED )  // cache hit
        {
          strncpy ( buff, host->fqdn, buff_len );

          // improved position of host on cache list
          host->hit++;
          reposition_node ( host );

          return 1;
        }
      else  // resolving, thread working
        {
          sockaddr_ntop ( ss, buff, buff_len );
          return 0;
        }
    }
  else  // cache miss
    {
      // transform binary to text
      sockaddr_ntop ( ss, buff, buff_len );

      // add task to workers (thread pool)
      host = create_node ( ss );
      add_task ( ip2domain_exec, host );
      insert_node_tail ( host );

      if ( tot_hosts_cache == cache_size )
        remove_last_node ();
      else
        tot_hosts_cache++;
    }

  return 0;
}
