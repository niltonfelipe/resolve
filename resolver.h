
#ifndef RESOLVER_H
#define RESOLVER_H

// client include inly resolver.h
#include "domain.h"
#include "service.h"

int
resolver_init ( unsigned int cache_size, unsigned int num_workers );

void
resolver_clean ( void );

#endif  // RESOLVER_H
