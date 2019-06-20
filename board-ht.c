#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"
#include "board-ht.h"

//Implementation

bool key_equal(hdict_key x, hdict_key y)
{
  REQUIRES(x != NULL && y != NULL);
  return bitvector_equal(*(bitvector*)x, *(bitvector*)y);
}

size_t key_hash(hdict_key x)
{
  REQUIRES(x != NULL);
  int value = 0;

  for (int i = 0; i < BITVECTOR_LIMIT; i++){
  	if (bitvector_get((*(bitvector*)x), i)) value &= (1<<i);
  }
  return value;

}

void bfree(void* x)
{
  free((struct board_data*)x);
}

hdict_t ht_new(size_t capacity)
{
  REQUIRES(capacity > 0);  
  hdict_t H = hdict_new(capacity, &key_equal, &key_hash, &bfree);

  ENSURES(H != NULL);

  return H;
}

struct board_data *ht_lookup(hdict_t H, bitvector B)
{
  REQUIRES(H != NULL);
  bitvector *item = &B;
  void* lookup = hdict_lookup(H, (void*)item);

  return (struct board_data*) lookup;

}

void ht_insert(hdict_t H, struct board_data *DAT)
{
  REQUIRES(H != NULL);
  REQUIRES(ht_lookup(H, DAT->board) == NULL);
  void *item = &(DAT->board);
  hdict_insert(H, item, (void*)DAT);
}