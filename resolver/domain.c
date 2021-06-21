
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

#include <stdlib.h>      // free
#include <string.h>      // strncpy
#include <sys/socket.h>  // getnameinfo
#include <netdb.h>       // getnameinfo

#include "thread_pool.h"
#include "sock_util.h"   // check_addr_equal

struct host
{
  struct sockaddr_storage ss;
  char fqdn[NI_MAXHOST];
  int status;
// status
#define RESOLVED  0
#define RESOLVING 1
};

// (2048 * sizeof(struct host)) == ~2.31 MiB cache of domain
#define CACHE_ENTRIES 2048
struct cache
{
  struct host hosts[CACHE_ENTRIES];
  int size;
  int idx;
};

// run on thread
static void
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

static struct host *
search_host ( struct cache *cache, struct sockaddr_storage *ss )
{
  struct host *host = cache->hosts;

  int i = cache->size;
  while ( i-- )
    {
      if ( host->status == RESOLVED &&
           check_addr_equal ( &host->ss, ss ) )
        return host;

      host++;
    }

  return NULL;
}

// return:
//  1 name resolved
//  0 name no resolved
int
ip2domain ( struct sockaddr_storage *ss, char *buff, const size_t buff_len )
{
  static struct cache cache;

  struct host *host = search_host ( &cache, ss );

  if ( host )
    {
      if ( host->status == RESOLVED )  // cache hit
        {
          strncpy ( buff, host->fqdn, buff_len );
          return 1;
        }
      else  // resolving, thread working
        {
          sockaddr_ntop ( ss, buff, buff_len );
          return 0;
        }
    }

  // cache miss

  // transform binary to text
  sockaddr_ntop ( ss, buff, buff_len );

  host = &cache.hosts[cache.idx];

  // not thread working here, start new work
  if ( host->status == RESOLVED )
    {
      memcpy ( &host->ss, ss, sizeof ( *ss ) );
      host->status = RESOLVING;
      add_task ( ip2domain_exec, host );  // add task to workers (thread pool)

      cache.idx = ( cache.idx + 1 ) % CACHE_ENTRIES;

      if ( cache.size < CACHE_ENTRIES )
        cache.size++;
    }

  return 0;
}
