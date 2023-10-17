#include "ms.h"
#include <string.h>
//#define NDEBUG
#include <assert.h>

//remember to account for out of bounds

// Maybe some of your own function prototypes here
int num_char(unsigned width, unsigned height, unsigned len);
int char_set(unsigned len, char inp[MAXSQ*MAXSQ+1]);
int num_mines_str(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1]);
unsigned num_mines_b(unsigned width, unsigned height, board b);
board moore_count(unsigned width, unsigned height, board b); //rename function

board solve_board(board b)
{

unsigned width = b.w; 
unsigned height = b.h; 
unsigned totmines = b.totmines;

   if(num_mines_b(width, height, b)==totmines){
      printf("Applying rule 1...\n");
      b = moore_count(width, height, b);
   }

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
//printf("%s\n", s); //print string to eyeball it
}


bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
unsigned len = strlen(inp);
int num_char_rtn = num_char(width, height, len);
int char_set_rtn = char_set(len, inp);
int num_mines_rtn = num_mines_str(len, totmines, inp);

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
/*
   for(int row = 0; row < height; row++){    
      for(int col = 0; col < width; col++){ 
         printf("%c", b.grid[row][col]);
      }
      printf("\n");
   }
   printf("\n");
*/

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

board moore_count(unsigned width, unsigned height, board b) //why does this cause a segmentation fault when it hits an UNK for the 2nd time
{
int w = (int) width;
int h = (int) height;
int cnt = 0;
for(int row = 0; row < h; row++){    
         for(int col = 0; col < w; col++){ 
            if(b.grid[row][col] == UNK){
               int row_unk = row;
               int col_unk = col;
                  if(row - 1 >= 0 && row < h && col - 1 >= 0 && col < w){
                     char N = b.grid[row-1][col];
                     //printf("N %c\n", N);
                     char S = b.grid[row+1][col];
                     //printf("S %c\n", S);
                     char W = b.grid[row][col-1];
                     //printf("W %c\n", W);
                     char E = b.grid[row][col+1];
                     //printf("E %c\n", E);
                     char NW = b.grid[row-1][col-1];
                     //printf("NW %c\n", NW);
                     char NE = b.grid[row-1][col+1];
                     //printf("NE %c\n", NE);
                     char SW = b.grid[row+1][col-1];
                     //printf("SW %c\n", SW);
                     char SE = b.grid[row+1][col+1];
                     //printf("SE %c\n", SE); 
                        if(N == MINE){ //change to switch statement? 
                           cnt++;
                        }
                        if(S == MINE){
                           cnt++;
                        }
                        if(W == MINE){
                           cnt++;
                        }
                        if(E == MINE){
                           cnt++;
                        }
                        if(NW == MINE){
                           cnt++;
                        }
                        if(NE == MINE){
                           cnt++;
                        }
                        if(SW == MINE){
                           cnt++;
                        }
                        if(SE == MINE){
                           cnt++;
                        }
                     printf("Mine count %u\n", cnt);
                     b.grid[row_unk][col_unk] = cnt+'0';
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
   
   // Rule 1 : 3x3 with 8 mines, 1 unknown
   strcpy(inp, "XXXX?XXXX");
   assert(syntax_check(8, 3, 3, inp)==true);
   b = make_board(8, 3, 3, inp);
   b = solve_board(b);
   board2str(inp, b);
   assert(strcmp(inp, "XXXX8XXXX")==0);

}

