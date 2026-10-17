#include <stdio.h>

int main() 
{

    int n;
    scanf("%d", &n);
  	
    int size = (n*2)-1;
    
    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
          int row_val = row;
          int col_val = col;
          
          if(row_val>=n)
          {
            row_val = 2*n -2-row;
          }  
          if(col_val >= n)
          {
            col_val = 2*n-2-col;
          }
          
          int minimum = row_val;
          if(row_val>col_val)
          {
            minimum = col_val;
          }
          printf("%d ",n-minimum);
        }
        printf("\n");
    }
    
    return 0;
}
