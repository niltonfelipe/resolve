
#include "thread_pool.h"
#include "domain.h"

int
resolver_init ( unsigned int cache_size, unsigned int num_workers )
{
  if ( !thpool_init ( num_workers ) )
    return 0;

  cache_domain_init ( cache_size );

  return 1;
}

void
resolver_clean ( void )
{
  thpool_free ();
  cache_domain_free ();
}
