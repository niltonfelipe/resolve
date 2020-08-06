/*
 *  Copyright (C) 2020 Mayco S. Berghetti
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
#include <stdlib.h>     // atoi
#include <arpa/inet.h>
#include <netdb.h>      // NI_MAXHOST NI_MAXSERV
#include <unistd.h>

#include "../domain.h"
#include "../service.h"

int main(int argc, char **argv)
{
  // example
  char *ip = "2001:4860:4860::8844";
  short int port = 53;

  if (argc > 1)
    ip = argv[1];

  if (argc > 2)
    port = atoi(argv[2]);

  // input...
  struct sockaddr_in6 host;
  host.sin6_family = AF_INET6;
  host.sin6_port = htons(port);
  inet_pton(AF_INET6, ip, &host.sin6_addr);

  char buff_domain[NI_MAXHOST];
  char buff_service[NI_MAXSERV];

  // return ip immediately, no dns query latency
  ip2domain((struct sockaddr_storage *) &host, buff_domain, NI_MAXHOST);
  printf("ip returned 1ª call     - %s\n", buff_domain);

  // life continue (working...)
  sleep(1);

  // the next query the domain will be available immediately (cache)
  ip2domain((struct sockaddr_storage *) &host, buff_domain, NI_MAXHOST);
  printf("domain returned 2ª call - %s\n", buff_domain);

  // example get service name
  port2serv(host.sin6_port, NULL, buff_service, NI_MAXSERV);
  printf("service name            - %s\n", buff_service);


  return 0;

}
