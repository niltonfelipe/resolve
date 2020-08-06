# domain resolver async
resolver asynchronous dns,
immediately returns the requested ip in text form,
however in the next request for that ip the domain will be available immediately (if there is a domain for the ip),
without generating delays in the main application

## Example usage
```javascript
  // input...
  struct sockaddr_in6 host;
  host.sin6_family = AF_INET6;
  host.sin6_port = htons(port);
  inet_pton(AF_INET6, ip, &host.sin6_addr);
  /////////////////////////////////////////////////

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
```

 **See folder [example](https://github.com/berghetti/resolver/tree/master/example)**
