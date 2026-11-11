#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int hourglasssum(int arr_row,int arr_column,int **arr)
{
   int maximum_sum = INT_MIN;
   for(int row =0;row<arr_row-2;row++)
   {
    for(int column =0;column<arr_column-2;column++)
    {
        int sum =0;
        for(int i=row;i<row+3;i++)
        {
            for(int j=column;j<column+3;j++)
            {
                if(i == row+1 && j != column+1)
                {
                  continue;
                }
                sum += *(*(arr+i)+j); 
            }
        }
        if(sum>maximum_sum)
        {
            maximum_sum = sum;
        }
    }
   }
   return maximum_sum;
}

int main()
{
    int rows =6,columns=6;

    int **arr = malloc(rows*sizeof(*arr));
    if(arr == NULL)
    {
        return 1;
    }

    for(int i=0;i<rows;i++)
    {
        *(arr+i) = malloc(columns*sizeof(*(arr+i)));
        if(*(arr+i) == NULL)
        {
            for(int j=0;j<i;j++)
            {
             free(*(arr+j));
            }
            free(arr);
            return 1;
        }

        printf("Enter row %d elements: ",i+1);
        for(int j =0;j<columns;j++)
        {
            scanf("%d",(*(arr+i)+j));
        }
    }

    int result = hourglasssum(rows,columns,arr);
    printf("\nMaximum Hour Glass Sum: %d",result);
    for(int i=0;i<rows;i++)
    {
        free(*(arr+i));
    }
    free(arr);
    return 0;
}

