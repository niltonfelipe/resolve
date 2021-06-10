
// test queue FIFO

#include <stdio.h>
#include "../queue.h"

#define ARRAY_SIZE( x ) ( sizeof ( x ) / sizeof ( ( x )[0] ) )

void
test1 ()
{
  puts ( "\ntest 1\n" );
  static int values[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
  unsigned int i;

  for ( i = 0; i < ARRAY_SIZE ( values ); i++ )
    {
      printf ( "stacking %dª item - value %d\n", i + 1, values[i] );
      enqueue ( &values[i] );
    }

  putchar ( '\n' );

  i = 0;
  int *val;
  while ( ( val = dequeue () ) )
    printf ( "unstacking %dª item - value %d\n", ++i, *val );
}

void
test2 ()
{
  puts ( "\ntest 2\n" );
  static int values[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
  unsigned long int data_size = ARRAY_SIZE ( values );

  unsigned int i, j;
  int *val;
  int *p;

  p = values;
  for ( i = 0; i < data_size / 2; i++ )
    {
      printf ( "stacking %dª item - value %d\n", i + 1, *p );
      enqueue ( p++ );
    }

  putchar ( '\n' );

  j = 0;
  while ( ( val = dequeue () ) )
    printf ( "unstacking %dª item - value %d\n", ++j, *val );

  putchar ( '\n' );

  for ( i = 0; i < data_size / 2; i++ )
    {
      printf ( "stacking %dª item - value %d\n", i + 1, *p );
      enqueue ( p++ );
    }

  putchar ( '\n' );

  j = 0;
  while ( ( val = dequeue () ) )
    printf ( "unstacking %dª item - value %d\n", ++j, *val );
}

void
test3 ()
{
  puts ( "\ntest 3\n" );
  static char *values[] = { "BANANA",
                            "ABACATE",
                            "LARANJA",
                            "ABACAXI",
                            "MANGA",
                            "UVA",
                            "GOIABA",
                            "TOMATE",
                            "ACEROLA",
                            "JABUTICABA" };
  unsigned long int data_size = ARRAY_SIZE ( values );
  char **p;
  unsigned int i;

  p = values + data_size - 1;
  for ( i = 0; i < data_size; i++ )
    {
      printf ( "stacking %dª item - value %s\n", i + 1, *p );
      enqueue ( *p-- );
    }

  putchar ( '\n' );

  i = 0;
  char *val;
  while ( ( val = dequeue () ) )
    printf ( "unstacking %dª item - value %s\n", ++i, val );
}

int
main ( void )
{
  test1 ();
  test2 ();
  test3 ();

  return 0;
}
