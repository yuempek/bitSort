#include <time.h>
#include <stdlib.h>
/*
  Copyright © 2019 Yuempek

  free to use!
*/

double randomd()   /* uniform distribution [0..1] */
{
  int i;
  unsigned long x = 0;

  srand(clock());
  for(i = 0; i < 8; i++)
      x = (x << 8) | (rand() & 0xFF);

  return 1.0*x / (unsigned long)(-1);
}
