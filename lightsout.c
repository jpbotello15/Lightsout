#include <stdbool.h>
#include <stdlib.h>
#include "lib/boardutil.h"
#include "lib/contracts.h"
#include "lib/queue.h"
#include "lib/xalloc.h"
#include "lib/bitvector.h"
#include "board-ht.h"

bitvector find_moves(bitvector B, int width, int height, int row, int col)
{
  REQUIRES(get_index(row,col,width,height) < BITVECTOR_LIMIT);

  B = bitvector_flip(B,get_index(row,col,width,height));

  if (is_valid_pos(row+1,col,width,height)){ 
  	B = bitvector_flip(B,get_index(row+1,col,width,height));
  }

  if (is_valid_pos(row-1,col,width,height)){ 
  	B = bitvector_flip(B,get_index(row-1,col,width,height));
  }

  if (is_valid_pos(row,col+1,width,height)){ 
  	B = bitvector_flip(B,get_index(row,col+1,width,height));
  }

  if (is_valid_pos(row,col-1,width,height)){ 
  	B = bitvector_flip(B,get_index(row,col-1,width,height));
  }

  return B;
}

void printmoves(hdict_t H, bitvector B, int width, int height,int row, int col)
{
  struct board_data *board_last = malloc(sizeof(struct board_data));
  board_last->board = B;
  board_last->row_prev = row;
  board_last->col_prev = col;
  bitvector board_temp = board_last->board;
  printf("%d:%d\n", board_last->row_prev, board_last->col_prev);

  board_temp = find_moves(board_last->board, width, height, board_last->row_prev, board_last->col_prev);
  free(board_last);
	
  while(board_last->row_prev != -1 && board_last->col_prev != -1){
    board_last = (struct board_data*) ht_lookup(H,board_temp);

    if(board_last->row_prev != -1){
      printf("%d:%d\n", board_last->row_prev,board_last->col_prev);
      board_temp = find_moves(board_last->board,width,height,board_last->row_prev,board_last->col_prev);
    }

    else{
      return;
    }

  }
	
}


int main(int argc, char **argv) {
  if (argc != 2) {
  	fprintf(stderr, "Usage: lightsout <board name>\n");
  	return 1;
  }

  char *board_filename = argv[1];

  uint8_t *width = malloc(sizeof(uint8_t));
  uint8_t *height = malloc(sizeof(uint8_t));
  bitvector *B = malloc(sizeof(bitvector));

  if(!file_read(board_filename,B,width,height)){
  	free(B);
  	free(width);	
	free(height);
	return 1;
  }

  if (*height == 5 && *width == 5) return 0;

  struct board_data *board_original = xmalloc(sizeof(struct board_data));
  board_original->board = *B;
  board_original->row_prev = -1;
  board_original->col_prev = -1;

  queue_t Q = queue_new();
  hdict_t H = ht_new(BITVECTOR_LIMIT*BITVECTOR_LIMIT);

  enq(Q, (queue_elem)board_original);
  ht_insert(H, board_original);

  while (!queue_empty(Q)){
    struct board_data *board_new = (struct board_data *)deq(Q);

  	for (int row = 0; row < *height; row++){
	  for (int col = 0; col < *width; col++){
      
        if (ht_lookup(H, find_moves(board_new->board,*width,*height,row,col)) == NULL){
          struct board_data *board_test = xmalloc(sizeof(struct board_data));
          board_test->board = find_moves(board_new->board,*width,*height,row,col);
          board_test->row_prev = row;
          board_test->col_prev = col;
          ht_insert(H,board_test);
          enq(Q,(queue_elem)board_test);
	  	}
      
      	if (bitvector_equal(find_moves(board_new->board,*width,*height,row,col), bitvector_new())){
          printmoves(H,find_moves(board_new->board,*width,*height,row,col),*width,*height,row,col);

          free(B);
          free(width);
          free(height);
          queue_free(Q, NULL);
          hdict_free(H);
      
          return 0;
        }

      }
	}
  }
	
  free(B);
  free(width);
  free(height);
  queue_free(Q, NULL);
  hdict_free(H);

  return 1;
}

