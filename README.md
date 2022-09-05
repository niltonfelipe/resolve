#Resolvedor de Domínio Assíncrono
Retorna imediatamente o ip solicitado em forma de texto, porém na próxima requisição desse ip o domínio estará disponível imediatamente (caso exista um domínio para o ip), sem gerar atrasos na aplicação principal.

## Exemplo de uso
```javascript
#include "domain.h"

  // 0 use the default number of workers
  thpool_init(0);

// input...
struct sockaddr_in6 host;
host.sin6_family = AF_INET6;
inet_pton ( AF_INET6, ip, &host.sin6_addr );
/////////////////////////////////////////////////

char buff_domain[NI_MAXHOST];
char buff_service[NI_MAXSERV];

// return ip immediately, no dns query latency
ip2domain ( ( struct sockaddr_storage * ) &host, buff_domain, NI_MAXHOST );
printf ( "ip returned 1ª call     - %s\n", buff_domain );

// life continue (working...)
sleep ( 1 );

// the next query the domain will be available immediately (cache)
ip2domain ( ( struct sockaddr_storage * ) &host, buff_domain, NI_MAXHOST );
printf ( "domain returned 2ª call - %s\n", buff_domain );

```

 **See folder [example](https://github.com/niltonfelipe/resolve/tree/master/example)**
