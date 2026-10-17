#include <stdio.h>

void calculate_the_maximum(int n, int k,int *max_and,int *max_or,int *max_xor) 
{
  for(int i=1;i<=n;i++)
  {
    for(int j =i+1;j<=n;j++)
    {
        int bitwise_result = i & j;
        if(bitwise_result > *max_and && bitwise_result < k)
        {
            *max_and = bitwise_result;
        }
        bitwise_result = i | j;
        if(bitwise_result > *max_or && bitwise_result < k)
        {
            *max_or = bitwise_result;
        }
        
        bitwise_result = i^j;
        if(bitwise_result > *max_xor && bitwise_result < k)
        {
            *max_xor = bitwise_result;
        }
    }
  }
  }
  

int main() {
    int n, k;
  
    scanf("%d %d", &n, &k);
    int max_and =0,max_or =0,max_xor =0;
    
    calculate_the_maximum(n, k,&max_and,&max_or,&max_xor);
    printf("%d\n",max_and);
    printf("%d\n",max_or);
    printf("%d",max_xor);
    
    
    return 0;
}
