#include "ms.h"
#include <string.h>
//#define NDEBUG
#include <assert.h>

//remember to account for out of bounds

// Maybe some of your own function prototypes here
int num_char(unsigned width, unsigned height, unsigned len);
int char_set(unsigned len, char inp[MAXSQ*MAXSQ+1]);
int num_mines(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1]);
/*
board solve_board(board b)
{

}

*/
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

//print string to visually verify 

}


bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
unsigned len = strlen(inp);
int num_char_flag = num_char(width, height, len);
int char_set_flag = char_set(len, inp);
int num_mines_flag = num_mines(len, totmines, inp);

if(num_char_flag != 0 || char_set_flag != 0 || num_mines_flag != 0){
   return false;
   }
else{
   return true;
   }

}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1]) /* add assert testing on conversion from 1D string to array (split out into separate function) */
{

board b; 
b.w = width; 
b.h = height; 
b.totmines = totmines;

int k = 0; 

   for(int i = 0; i < height; i++){   
      for(int j = 0; j < width; j++){ 
         b.grid[(k-j)/width][k - i * width] = inp[k]; //convert 1D string into 2D array
         k++;
      }
   }


   for(int j = 0; j < height; j++){    
      for(int i = 0; i < width; i++){ 
         printf("%c", b.grid[j][i]);
      }
      printf("\n");
   }
   printf("\n");


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

int num_mines(unsigned len, unsigned totmines, char inp[MAXSQ*MAXSQ+1])
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


void test(void)
{   
   char inp[MAXSQ*MAXSQ+1];
   strcpy(inp, "11?010?011"); //too big
   assert(syntax_check(1, 3, 3, inp)==false);

   assert(num_char(2, 2, 4)==0); // length = width * height 
   assert(num_char(3, 3, 4)==1); // length != width * height

   strcpy(inp, "Q11111l0X");
   assert(char_set(9, inp)==1);

   strcpy(inp, "???X11110");
   assert(char_set(9, inp)==0); 

   strcpy(inp, " ");
   assert(char_set(1, inp)==1); 
   
   strcpy(inp, "\0");
   assert(char_set(1, inp)==1); 

   strcpy(inp, "11111110X");
   assert(num_mines(9, 1, inp)==0);

   strcpy(inp, "1X1X1110X");
   assert(num_mines(9, 4, inp)==0);

   strcpy(inp, "1X1X1110X");
   assert(num_mines(9, 2, inp)==1);

   strcpy(inp, "11?0"); //right size
   assert(syntax_check(1, 2, 2, inp)==true);
   //Invalid string (has an 'l' not a '1')
   strcpy(inp, "l");
   assert(syntax_check(1, 1, 1, inp)==false);

    //Invalid string (has an 'l' not a '1')
   strcpy(inp, "XXXXlX");
   assert(syntax_check(1, 3, 2, inp)==false);

   strcpy(inp, "11?0?X111l111X?11?11?X10?");     
   assert(syntax_check(3, 5, 5, inp)==false);   
}


