#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void swapValues(int *p, int *q)
{
    int temp =*p;
    *p = *q;
    *q = temp;
}

void smoothMatrix(int *matrix,int n)
{
    int prev[n],curr[n];

    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
      {
        curr[j] =*(matrix+i*n+j);
      }
      for(int j=0;j<n;j++)
      {
        int sum =0;
        int count =0;

        for(int r=i-1;r<=i+1;r++)
        {
            for(int c = j-1;c<=j+1;c++)
            {
                if(r>=0 && r<n && c>=0 && c<n)
                {
                    if(r == i-1)
                    {
                        sum = sum+prev[c];
                    }
                    else if(r==i)
                    {
                        sum = sum+curr[i];
                    }
                    else{
                        sum = sum+*(matrix+r*n+c);
                    }
                    count++;
                }
            }
        }
        *(matrix+i*n+j) = sum/count;
      }
      for(int j=0;j<n;j++)
      {
        prev[j] = curr[j];
      }
    }
}

int main()
{
    int n;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);

    if(n<2 || n>10)
    {
        printf("Invalid input");
        return 0;
    }

    int matrix[n][n];
    srand(time(NULL));

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            *(*(matrix+i)+j) = rand()%256;
        }
    }

    printf("\nOriginal Randomly Generated Matrix:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("%d ",*(*(matrix+i)+j));
        }
        printf("\n");
    }

    // Rotation of matrix
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
           swapValues(*(matrix+i)+j,*(matrix+j)+i);
        }
    }

    int left =0;
    int right = n-1;

    while(left<right)
    {
        for(int j=0;j<n;j++)
        {
           swapValues(*(matrix+left)+j,*(matrix+right)+j);
        }
        left++;
        right--;
    }

    printf("\nMatrix after 90-degree Clockwise Rotation:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("%d ",*(*(matrix+i)+j));
        }
        printf("\n");
    }
    
    smoothMatrix(&matrix[0][0],n);

    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("%d ",*(*(matrix+i)+j));
        }
        printf("\n");
    }
    return 0;
}

