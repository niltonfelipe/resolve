
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
#include <stdlib.h>  // atoi
#include <netdb.h>   // NI_MAXHOST NI_MAXSERV
#include <arpa/inet.h>
#include <unistd.h>

#include "../resolver/resolver.h"

struct demand
{
  char buff_domain[NI_MAXHOST];
  char *ip;
  int type;
};

static void
fatal ( char *msg )
{
  fprintf ( stderr, "%s\n", msg );
  exit ( 1 );
}

void
start_demand ( struct demand *demand, int size )
{
  for ( int i = 0; i < size; i++ )
    {
      if ( demand[i].type == AF_INET )
        {
          struct sockaddr_in host = { .sin_family = AF_INET };

          inet_pton ( AF_INET, demand[i].ip, &host.sin_addr );
          ip2domain ( ( struct sockaddr_storage * ) &host,
                      demand[i].buff_domain,
                      NI_MAXHOST );
        }
      else if ( demand[i].type == AF_INET6 )
        {
          struct sockaddr_in6 host = { .sin6_family = AF_INET6 };

          inet_pton ( AF_INET6, demand[i].ip, &host.sin6_addr );
          ip2domain ( ( struct sockaddr_storage * ) &host,
                      demand[i].buff_domain,
                      NI_MAXHOST );
        }
      else
        continue;

      printf ( "%s - %s\n", demand[i].ip, demand[i].buff_domain );
    }
}

int
main ( void )
{
  if ( !resolver_init ( 0 ) )
    fatal ( "Error init_workers" );

  // input...
  static struct demand demand[] = { { { 0 }, "8.8.8.8", AF_INET },
                                    { { 0 }, "9.9.9.9", AF_INET },
                                    { { 0 }, "201.10.128.3", AF_INET },
                                    { { 0 }, "201.10.128.2", AF_INET },
                                    { { 0 }, "204.79.197.212", AF_INET },
                                    { { 0 }, "2001:4860:4860::8844", AF_INET6 },
                                    { { 0 }, "8.8.4.4", AF_INET },
                                    { { 0 }, "208.67.222.222", AF_INET },
                                    { { 0 }, "208.67.220.220", AF_INET },
                                    { { 0 }, "1.1.1.1", AF_INET },
                                    { { 0 }, "142.250.218.197", AF_INET },
                                    { { 0 }, "216.58.202.142", AF_INET } };

  puts ( "First call, return immediately\n" );
  start_demand ( demand, sizeof demand / sizeof demand[0] );

  sleep ( 1 );

  puts ( "\nSecond call, return immediately whith name resolveds (if there was "
         "time )\n" );
  start_demand ( demand, sizeof demand / sizeof demand[0] );

  resolver_clean ();

  return 0;
}
