#include "ms.h"
#include <string.h>
#define NDEBUG
#include <assert.h>

//why are some things ints and some unsigned?
int num_char(unsigned width, unsigned height, unsigned len);
int char_set(unsigned len, char inp[MAXSQ*MAXSQ+1]);
int num_mines_str(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1]);
unsigned num_mines_b(unsigned width, unsigned height, board b);
board moore_count_rule_1(unsigned width, unsigned height, char position, char find, board b); //rename function - returns board
board apply_rule_2(unsigned width, unsigned height, unsigned row, unsigned col, board b);
unsigned moore_neighbour_cnt(unsigned width, unsigned height, unsigned row, unsigned col, char find, board b);

board solve_board(board b)
{
unsigned width = b.w; 
unsigned height = b.h; 
unsigned totmines = b.totmines; 

//rule 2
   for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){ 
         if(b.grid[row][col] != MINE && b.grid[row][col] != UNK){ //i.e. if it's a number 
            unsigned num = b.grid[row][col];
            unsigned unk_count = moore_neighbour_cnt(width, height, row, col, UNK, b); 
            unsigned mine_count = moore_neighbour_cnt(width, height, row, col, MINE, b);
            //printf("num %c, unk_count %u, mine_count %u\n", num, unk_count, mine_count);
               if(num - '0' == unk_count + mine_count){ // - '0' to convert from bad ASCII number we don't know about
                  b = apply_rule_2(width, height, row, col, b);   
               } 
           }
       }
    }

//temp print function
for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){ 
         printf("%c", b.grid[row][col]);
      }
      printf("\n");
   }
   printf("\n");
  

//rule 1 

   if(num_mines_b(width, height, b)==totmines){
      printf("Applying rule 1...\n");
      b = moore_count_rule_1(width, height, UNK, MINE, b);
   }


//temp print function
for(unsigned row = 0; row < height; row++){    
      for(unsigned col = 0; col < width; col++){ 
         printf("%c", b.grid[row][col]);
      }
      printf("\n");
   }
   printf("\n");
  
return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
int width = b.w;
int height = b.h; 

int k = 0; 
   for(int i = 0; i < height; i++){   
      for(int j = 0; j < width; j++){ 
         s[k] = b.grid[(k-j)/width][k - i * width];
         k++;
      }
   }
}

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
unsigned len = strlen(inp);
unsigned num_char_rtn = num_char(width, height, len);
unsigned char_set_rtn = char_set(len, inp);
unsigned num_mines_rtn = num_mines_str(len, totmines, inp);

if(num_char_rtn != 0 || char_set_rtn != 0 || num_mines_rtn != 0){
   return false;
   }
else{
   return true;
   }

}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1]) /* split conversion from 1D string to 2D array into a separate function, then assert test it with expected values */
{ //use unsigned int instead of int

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

int num_char(unsigned width, unsigned height, unsigned len)
{
// Make sure number of characters in string == width*height

   if(len != width * height){
      return 1;
   }
   else{
      return 0;
   }
}

int char_set(unsigned len, char inp[MAXSQ*MAXSQ+1])
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
   for(unsigned i = 0; i < len + 1; i++){
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

unsigned num_mines_b(unsigned width, unsigned height, board b) //compares mines in board with totmines
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

board moore_count_rule_1(unsigned width, unsigned height, char position, char find, board b) 
{
int cnt= 0;
for(unsigned row = 0; row < height; row++){    
         for(unsigned col = 0; col < width; col++){ 
            if(b.grid[row][col] == position){
               unsigned row_unk = row;
               unsigned col_unk = col;
                     cnt = moore_neighbour_cnt(width, height, row, col, find, b); //this isn't working
                     b.grid[row_unk][col_unk] = cnt+'0'; //could this just be row and col?
                     cnt = 0;  //reset counter 
              }
          }
      }
return b;
}

board apply_rule_2(unsigned width, unsigned height, unsigned row, unsigned col, board b)
{
                     printf("Applying Rule 2...\n");
                     if(row>0){   // can I simplify this bounds checking and/or move it out to a function? 
                     char N = b.grid[row-1][col]; // can these if statements be switch? 
                       if(N == UNK){  //could N S E W directions be an enumerated type?
                           b.grid[row-1][col] = MINE;
                                                      //if I have time, convert all these if statements into 2 nested loops
                           }
                     }
                     if(row<height-1){   
                     char S = b.grid[row+1][col];
                        if(S == UNK){
                           b.grid[row+1][col] = MINE;
                        }
                     }
                     if(col>0){
                     char W = b.grid[row][col-1];
                        if(W == UNK){
                           b.grid[row][col-1] = MINE;
                        }
                     }
                     if(col < width-1 ){
                     char E = b.grid[row][col+1];
                       if(E == UNK){
                           b.grid[row][col+1] = MINE;
                        }
                     }
                     if(row> 0 && col >0){
                     char NW = b.grid[row-1][col-1];
                     if(NW == UNK){
                           b.grid[row-1][col-1] = MINE;
                        }
                     }
                      if(row>0 && col<width-1){
                     char NE = b.grid[row-1][col+1];
                        if(NE == UNK){
                           b.grid[row-1][col+1] = MINE;
                        }
                     }
                     if(row< height-1 && col > 0){
                     char SW = b.grid[row+1][col-1];
                      if(SW == UNK){
                           b.grid[row+1][col-1] = MINE;
                        }
                     }
                     if(row < height-1 && col < width-1){
                     char SE = b.grid[row+1][col+1];
                        if(SE == UNK){
                           b.grid[row+1][col+1] = MINE;
                        }
                     }
return b; 
}

unsigned moore_neighbour_cnt(unsigned row, unsigned col, unsigned height, unsigned width, char find, board b)
{ 
printf("row: %u, col: %u, height: %u, width: %u, find: %c\n", row, col, height, width, find); 
int cnt = 0;
                    if(row>0){   
                     char N = b.grid[row-1][col]; 
                       if(N == find){  
                           cnt++;
                           printf("Hello N\n");
                        }
                     }
                     if(row<height-1){   
                     char S = b.grid[row+1][col];
                           if(S == find){
                           cnt++;
                           printf("Hello S\n");
                        }
                     }
                     if(col>0){
                     char W = b.grid[row][col-1];
                     if(W == find){
                           cnt++;
                           printf("Hello W\n");
                        }
                     }
                     if(col < width-1 ){
                     char E = b.grid[row][col+1];
                       if(E == find){
                           cnt++;
                           printf("Hello E\n");
                        }
                     }
                     if(row> 0 && col >0){
                     char NW = b.grid[row-1][col-1];
                     if(NW == find){
                           cnt++;
                           printf("Hello NW\n");
                        }
                     }
                      if(row>0 && col<width-1){
                     char NE = b.grid[row-1][col+1];
                        if(NE == find){
                           cnt++;
                           printf("Hello NE\n");
                        }
                     }
                     if(row< height-1 && col > 0){
                     char SW = b.grid[row+1][col-1];
                      if(SW == find){
                           cnt++;
                           printf("Hello SW\n");
                        }
                     }
                     if(row < height-1 && col < width-1){
                     char SE = b.grid[row+1][col+1];
                        if(SE == find){
                           cnt++;
                           printf("Hello SE\n");
                        }
                 }
printf("cnt in moore_neighbour_cnt: %i\n", cnt);
return (unsigned) cnt; 
}

void test(void)
{   
   board b;
   char inp[MAXSQ*MAXSQ+1];

   // Unknowns
   strcpy(inp, "XXX?XX535XX303X?535XXX??X");
   assert(syntax_check(16, 5, 5, inp)==true);
   b = make_board(16, 5, 5, inp);
   board2str(inp, b);
   assert(strcmp(inp, "XXX?XX535XX303X?535XXX??X")==0);   

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

   // length = width * height 
   assert(num_char(2, 2, 4)==0); 
   // length != width * height
   assert(num_char(3, 3, 4)==1); 

   //non-X letter
   strcpy(inp, "Q11111l0X"); 
   assert(char_set(9, inp)==1);

   //correct chars in string
   strcpy(inp, "???X11110");
   assert(char_set(9, inp)==0); 

   //space in string
   strcpy(inp, " ");
   assert(char_set(1, inp)==1); 
   
   //null char in string
   strcpy(inp, "\0");
   assert(char_set(1, inp)==1); 

   //num mines in string == totmines
   strcpy(inp, "11111110X");
   assert(num_mines_str(9, 1, inp)==0);

   //num mines in string < totmines
   strcpy(inp, "1X1X1110X");
   assert(num_mines_str(9, 4, inp)==0);

   //num mines in string > totmines
   strcpy(inp, "1X1X1110X");
   assert(num_mines_str(9, 2, inp)==1);     

   // num mines in board
   strcpy(inp, "XXXXXX535XX303XX535XXXXXX");
   assert(syntax_check(16, 5, 5, inp)==true);
   b = make_board(16, 5, 5, inp);
   b = solve_board(b);
   board2str(inp, b);
   assert(num_mines_b(5, 5, b)==16);

   // moore_neighbour_cnt '?' 
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_neighbour_cnt(5, 5, 3, 1, '?', b)==1);
   
   // moore_neighbour_cnt 'X' 
   strcpy(inp, "0111013X311XXX113?3101110");
   assert(syntax_check(4, 5, 5, inp)==true);
   b = make_board(4, 5, 5, inp);
   assert(moore_neighbour_cnt(5, 5, 3, 1, 'X', b)==2);
// unsigned width, unsigned height, unsigned row, unsigned col, char find, board b

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

   //add lots more testing and take out any redundant tests
 
}
