#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

long arraymanipulation(int arr_size,int number_of_queries,int **queries)
{
   long *arr = calloc(arr_size+1,sizeof(long));
    if (arr == NULL)
    {
        printf("Memory allocation failed\n");
        return -1;
    }

   for(int i=0;i<number_of_queries;i++)
   {
    int a = *(*(queries+i)+0);
    int b = *(*(queries+i)+1);
    int k = *(*(queries+i)+2);

    *(arr+a-1) += k; 
    *(arr+b) -= k;
   }

   long maximum_value = LONG_MIN;
   long sum =0;
   for(int i=0;i<arr_size;i++)
   {
    sum = sum+ *(arr+i);
    if(maximum_value<sum)
    {
        maximum_value = sum;
    }
   }

   free(arr);
   return maximum_value;

}

int main()
{
    int arr_size;
    int number_of_queries;

    printf("Enter array size and Number of Queries: ");
    scanf("%d %d",&arr_size,&number_of_queries);

    int **queries = malloc(number_of_queries*sizeof(*queries));
    if(queries == NULL)
    {
        return 1;
    }

    for(int i=0;i<number_of_queries;i++)
    {
        *(queries+i) = malloc(3*sizeof(*(queries+i)));
        if(*(queries+i) == NULL)
        {
            for(int j=0;j<i;j++)
            {
                free(*(queries+j));
            }
            free(queries);
            return 1;
        }

        printf("Enter a,b and k for query %d: ",i+1);

        for(int j=0;j<3;j++)
        {
            scanf("%d",(*(queries+i)+j));
        }
    
    }
    
    long result = arraymanipulation(arr_size,number_of_queries,queries);
    printf("Maximum value: %d\n", result);

    for (int i = 0; i < number_of_queries; i++)
    {
        free(*(queries + i));
    }

    free(queries);

    return 0;
}

