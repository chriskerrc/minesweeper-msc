#include "ms.h"
#include <string.h>
#include <assert.h>

#define CHAR_DIFF '0'
#define MOORE_STEP 1
#define STR_CONST 1
#define LOW_BOUND 0

int str_len_w_h(unsigned width, unsigned height, unsigned len); //compare string len & array width, height
int char_in_set(unsigned len, char inp[MAXSQ*MAXSQ+1]); //check chars are in allowed set
int num_mines_str(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1]);
unsigned num_mines_board(unsigned width, unsigned height, board b);
unsigned moore_count(unsigned width, unsigned height, unsigned row, unsigned col, char find_val, board b);
board rule_1_board(unsigned width, unsigned height, char cell_val, char find_val, board b); 
board apply_rule_2(unsigned width, unsigned height, unsigned row, unsigned col, board b);

board solve_board(board b)
{
   unsigned width = b.w; 
   unsigned height = b.h; 
   unsigned totmines = b.totmines; 

   //rule 2
   for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){ 
         if(b.grid[row][col] != MINE && b.grid[row][col] != UNK){ //if value is number 
            unsigned num = b.grid[row][col];
            unsigned unk_count = moore_count(width, height, row, col, UNK, b);
            unsigned mine_count = moore_count(width, height, row, col, MINE, b);
               if(num - CHAR_DIFF == unk_count + mine_count){ // convert char to unsigned int
                  b = apply_rule_2(width, height, row, col, b);   
               } 
         }
      }
   }  

   //rule 1 
   if(num_mines_board(width, height, b)==totmines){
      b = rule_1_board(width, height, UNK, MINE, b);
   }
   return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
   int width = b.w;
   int height = b.h; 

   int str_index = 0; 
   for(int row = 0; row < height; row++){   
      for(int col = 0; col < width; col++){ 
         s[str_index] = b.grid[(str_index-col)/width][str_index - row * width]; //convert 2D array to string
         str_index++;
      }
   }
}


bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
   unsigned len = strlen(inp);
   unsigned str_len_w_h_rtn = str_len_w_h(width, height, len);
   unsigned char_in_set_rtn = char_in_set(len, inp);
   unsigned num_mines_rtn = num_mines_str(len, totmines, inp);

   if(str_len_w_h_rtn != 0 || char_in_set_rtn != 0 || num_mines_rtn != 0){
      return false;
   }
   else{
      return true;
   }
}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{ 
   board b; 
   b.w = width; 
   b.h = height; 
   b.totmines = totmines;
   int str_index = 0; 

   for(int row = 0; row < height; row++){   
      for(int col = 0; col < width; col++){ 
         b.grid[(str_index - col)/width][str_index - row * width] = inp[str_index]; //convert 1D string into 2D array
         str_index++;
      }
   }
   return b;
} 

int str_len_w_h(unsigned width, unsigned height, unsigned len)
{
   // Make sure number of characters in string == width*height
   if(len != width * height){
      return 1;
   }
   else{
      return 0;
   }
}

int char_in_set(unsigned len, char inp[MAXSQ*MAXSQ+1])
{
   // Ensure only characters are from the set:   012345678?X
   unsigned cnt = 0; 
   for(unsigned i = 0; i < len; i++){
      if(isalpha(inp[i]) != 0 && inp[i] != MINE){
         cnt++;
      }      
      if(ispunct(inp[i]) != 0 && inp[i] != UNK){
         cnt++;
      }
      if(isgraph(inp[i]) == 0){
         cnt++;
      }
      if(inp[i] == '9'){
         cnt++;
      }
   }
   if(cnt > 0){
      return 1;
   }
   else{
      return 0;
   } 
}

int num_mines_str(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1])
{
   // Ensure mines in string <= totmines
   unsigned cnt = 0;
   for(unsigned i = 0; i < len + STR_CONST; i++){
      if(inp[i] == 'X'){
         cnt++;
      }
   } 
   if(cnt > totmines){
      return 1;
   }
   else{
      return 0;
   }
}

unsigned num_mines_board(unsigned width, unsigned height, board b) //compares mines in board with totmines
{
   unsigned cnt = 0;
   for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){   
         if(b.grid[row][col] == MINE){
            cnt++;
         }
      }
   } 
   return cnt;
}

board rule_1_board(unsigned width, unsigned height, char cell_val, char find_val, board b) 
{
   int cnt= 0;
   for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){ 
         if(b.grid[row][col] == cell_val){
            cnt = moore_count(width, height, row, col, find_val, b);
            b.grid[row][col] = cnt+'0'; 
            cnt = 0;
         }
      }
   }
   return b;
}

unsigned moore_count(unsigned width, unsigned height, unsigned row, unsigned col, char find_val, board b)
{
   unsigned cnt= 0;
      for(int j = row - MOORE_STEP; j <= (int)row + MOORE_STEP ; ++j){
         for(int i = col - MOORE_STEP; i <= (int)col + MOORE_STEP; ++i){
            if(i >= LOW_BOUND && j >= LOW_BOUND && i< (int)width && j<(int)height){ //check array bounds
               if(!((j == (int)row) && (i == (int) col))){
                  int val = b.grid[j][i];
                  if(val == find_val){
                     cnt++;
                  }
               }
            }
         }
      }
   return cnt;
}

board apply_rule_2(unsigned width, unsigned height, unsigned row, unsigned col, board b)
{   
   for(int j = row - MOORE_STEP; j <= (int)row + MOORE_STEP ; ++j){
      for(int i = col - MOORE_STEP; i <= (int)col + MOORE_STEP; ++i){
         if(i >= LOW_BOUND && j >= LOW_BOUND && i< (int)width && j<(int)height){ //check array bounds
            if (!((i == (int) col) && (j == (int)row))) {
               int val = b.grid[j][i];
               if(val == UNK){
                  b.grid[j][i] = MINE;
               }
            }
         }
      }
   }
   return b; 
}

void test(void)
{   
   board b;
   char inp[MAXSQ*MAXSQ+1];
    
// STR_LEN_W_H 
   
   // length = width * height 
   assert(str_len_w_h(2, 2, 4)==0); 
   
   // length != width * height
   assert(str_len_w_h(3, 3, 4)==1); 

// CHAR_IN_SET

   //non-'X' letters
   strcpy(inp, "Q1ra1bl0X"); 
   assert(char_in_set(9, inp)==1);

   //non-'?' punct
   strcpy(inp, "0)1@1./0X"); 
   assert(char_in_set(9, inp)==1);

   //correct chars in string
   strcpy(inp, "?X012345678");
   assert(char_in_set(11, inp)==0); 

   //space in string
   strcpy(inp, " ");
   assert(char_in_set(1, inp)==1); 
   
   //null char in string
   strcpy(inp, "\0");
   assert(char_in_set(1, inp)==1); 
   
   //'9' in string
   strcpy(inp, "?X0123456789");
   assert(char_in_set(12, inp)==1); 

// NUM_MINES_STR

   //num mines in string == totmines
   strcpy(inp, "11214510X");
   assert(num_mines_str(9, 1, inp)==0);

   //num mines in string < totmines
   strcpy(inp, "XX?10X");
   assert(num_mines_str(6, 4, inp)==0);

   //num mines in string > totmines
   strcpy(inp, "1X1X1110X?");
   assert(num_mines_str(10, 2, inp)==1);     
   
// NUM_MINES_BOARD

   // num mines in board == totmines (>0)
   strcpy(inp, "XXXXXX535XX303XX535XXXXXX");
   assert(syntax_check(16, 5, 5, inp)==true);
   b = make_board(16, 5, 5, inp);
   board2str(inp, b);
   assert(num_mines_board(5, 5, b)==16);
   
   // num mines in board == totmines (0)
   strcpy(inp, "0?0123435?03031?03543218?");
   assert(syntax_check(0, 5, 5, inp)==true);
   b = make_board(0, 5, 5, inp);
   board2str(inp, b);
   assert(num_mines_board(5, 5, b)==0);
   
// RULE_1_BOARD

   // Rule 1 : 4x3 with 6 mines, 2 unknowns
   strcpy(inp, "X?XX?XX0X123");
   assert(syntax_check(6, 4, 3, inp)==true);
   b = make_board(6, 4, 3, inp);
   b = rule_1_board(4, 3, UNK, MINE, b);
   board2str(inp, b);
   assert(strcmp(inp, "X4XX3XX0X123")==0);

   // Rule 1 : 2x5 with 3 mines, 3 unknowns
   strcpy(inp, "0?X2?X?0X1");
   assert(syntax_check(3, 2, 5, inp)==true);
   b = make_board(3, 2, 5, inp);
   b = rule_1_board(2, 5, UNK, MINE, b);
   board2str(inp, b);
   assert(strcmp(inp, "01X22X20X1")==0);

   // Rule 1 : 1x6 with 2 mines, 2 unknowns
   strcpy(inp, "0?X2?X");
   assert(syntax_check(2, 1, 6, inp)==true);
   b = make_board(2, 1, 6, inp);
   b = rule_1_board(1, 6, UNK, MINE, b);
   board2str(inp, b);
   assert(strcmp(inp, "01X21X")==0);

   // Rule 1 : 4x2 with 0 mines, 0 unknowns
   strcpy(inp, "01234567");
   assert(syntax_check(0, 4, 2, inp)==true);
   b = make_board(0, 4, 2, inp);
   b = rule_1_board(4, 2, UNK, MINE, b);
   board2str(inp, b);
   assert(strcmp(inp, "01234567")==0);
   
   // Rule 1 : 4x2 with 1 mine, 0 unknowns
   strcpy(inp, "012345X7");
   assert(syntax_check(1, 4, 2, inp)==true);
   b = make_board(1, 4, 2, inp);
   b = rule_1_board(4, 2, UNK, MINE, b);
   board2str(inp, b);
   assert(strcmp(inp, "012345X7")==0);
   
// MOORE_COUNT
 
   // moore_count '?'
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_count(5, 5, 3, 1, '?', b)==1);

   // moore_count '?' edge 
   strcpy(inp, "X13X1X1?02?1");
   assert(syntax_check(3, 3, 4, inp)==true);
   b = make_board(3, 3, 4, inp);
   assert(moore_count(3, 4, 3, 2, '?', b)==2);
 
   // moore_count 'X' 
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_count(5, 5, 3, 1, 'X', b)==2);
   
   //moore_count 'X' edge 1
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_count(5, 5, 0, 0, 'X', b)==0);
   
   //moore_count 'X' edge 2
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_count(5, 5, 0, 2, 'X', b)==1);

// SYNTAX_CHECK

   //too big
   strcpy(inp, "11?010?011"); 
   assert(syntax_check(1, 3, 3, inp)==false);
   
   //right size
   strcpy(inp, "11?0"); 
   assert(syntax_check(1, 2, 2, inp)==true);
   
   // too many mines in string   
   strcpy(inp, "11?0?X111l111X?11?11?X10?");   
   assert(syntax_check(3, 5, 5, inp)==false); 
   
   //space char
   strcpy(inp, "11? "); 
   assert(syntax_check(1, 2, 2, inp)==false);
   
   //'9'
   strcpy(inp, "119??X"); 
   assert(syntax_check(1, 2, 3, inp)==false);

   //non-X letters
   strcpy(inp, "119???wX"); 
   assert(syntax_check(1, 2, 4, inp)==false);
   
   //null-char
   strcpy(inp, "\0"); 
   assert(syntax_check(1, 1, 1, inp)==false);

// BOARD_2_STR & MAKE_BOARD

   // Unknowns
   strcpy(inp, "XXX?XX535XX303X?535XXX??X");
   assert(syntax_check(16, 5, 5, inp)==true);
   b = make_board(16, 5, 5, inp);
   board2str(inp, b);
   assert(strcmp(inp, "XXX?XX535XX303X?535XXX??X")==0);   

   // Unknowns and no mines
   strcpy(inp, "012?34535673038?535012??0");
   assert(syntax_check(0, 5, 5, inp)==true);
   b = make_board(0, 5, 5, inp);
   board2str(inp, b);
   assert(strcmp(inp, "012?34535673038?535012??0")==0);   
 
   // Mines and no unknowns
   strcpy(inp, "XXX1XX535XX303X0535XXX23X");
   assert(syntax_check(16, 5, 5, inp)==true);
   b = make_board(16, 5, 5, inp);
   board2str(inp, b);
   assert(strcmp(inp, "XXX1XX535XX303X0535XXX23X")==0); 

// APPLY_RULE_2

   // Rule 2 one unknown
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   b = apply_rule_2(5, 5, 4, 2, b);
   board2str(inp, b);
   assert(strcmp(inp, "0111013X311XXX113X3101110")==0); 

   // Rule 2 multiple unknowns
   strcpy(inp, "2?X12?32112X");
   assert(syntax_check(4, 4, 3, inp)==true);
   b = make_board(4, 4, 3, inp);
   b = apply_rule_2(4, 3, 1, 0, b);
   board2str(inp, b);
   assert(strcmp(inp, "2XX12X32112X")==0); 

// SOLVE_BOARD 
  
   // Rule 2 : 5x5 with 4 mines, 1 unknown 
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   b = solve_board(b);
   board2str(inp, b);
   assert(strcmp(inp, "0111013X311XXX113X3101110")==0);

   // Rules 1&2 : 5x5 with 3 mines, multiple unknowns
   strcpy(inp, "?110?1?2101?X?1012?1?0111");
   assert(syntax_check(3, 5, 5, inp)==true);
   b = make_board(3, 5, 5, inp);
   b = solve_board(b);
   board2str(inp, b);
   assert(strcmp(inp, "111001X21012X21012X100111")==0);
   
   // Rules 1&2 : 3x4 with 3 mines, multiple unknowns
   strcpy(inp, "1X223?X111?0");
   assert(syntax_check(3, 3, 4, inp)==true);
   b = make_board(3, 3, 4, inp);
   b = solve_board(b);
   board2str(inp, b);
   assert(strcmp(inp, "1X223XX11110")==0);
}
