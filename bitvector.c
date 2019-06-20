#include <stdlib.h>
#include <stdbool.h>
#include "lib/bitvector.h"
#include "lib/contracts.h"

bitvector bitvector_new(){
  return (bitvector)0;
}

bool bitvector_get(bitvector B, uint8_t i){
  REQUIRES(i < BITVECTOR_LIMIT);
  int x = (B >> (bitvector)i) & (bitvector)1;
  return x;
}

bitvector bitvector_flip(bitvector B, uint8_t i){
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector mask = ((bitvector)1 << (bitvector)i);
  return B ^ mask;
}

bool bitvector_equal(bitvector B1, bitvector B2){
  
  return (B1 == B2);
}
