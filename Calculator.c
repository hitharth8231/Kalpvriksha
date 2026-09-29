#include<stdio.h>
#include <ctype.h>
#include<string.h>

int main()
{
    char input[100];
    printf("Input:");
    scanf("%[^\n]",input);


    char clear[100];
    int j=0;

    for(int i=0;input[i] != '\0';i++)
    {
        if(isspace(input[i]))
        {
            continue;
        }
        else if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || 
            (input[i] >= '0' && input[i] <= '9'))
        {
          clear[j] = input[i];
          j++;
        }
        else
        {
            printf("Error: Invalid expression.");
            return 0;
        }
    }
     clear[j] = '\0';
    int ln = strlen(clear);
    if(ln<1)
    {
        printf("Error: Invalid expression.");
        return 0;
    }

    if(clear[0] == '+' || clear[0] == '-' || clear[0] == '*' || clear[0] == '/')
            {
                printf("Error: Invalid expression.");
                return 0;
            }


    int result =0;
    int lastterm = 0;
    int currnumber = 0;
    char operator = '+';

    for(int i=0;clear[i] != '\0';i++)
    {
        if(clear[i]>='0' && clear[i]<='9')
        {
            currnumber =currnumber*10 + (clear[i]-'0');
        }
        else
        {
            if(i == ln-1)
            {
                printf("Error: Invalid expression.");
                return 0;
            }
            if(clear[i+1] == '+' || clear[i+1] == '*' || clear[i+1] == '-' || clear[i+1] == '/')
            {
                 printf("Error: Invalid expression.");
                 return 0;
            }
            if(operator == '+')
            {
                result = lastterm+result;
                lastterm = currnumber;
            }
            else if(operator == '-')
            {
                result = lastterm+result;
                lastterm = -currnumber;
            }
            else if(operator == '*')
            {
                lastterm = lastterm*currnumber;
            }
            else if(operator == '/')
            {
                if(currnumber == 0)
                {
                    printf("Error: Division by zero.");
                    return 0;
                }
                lastterm = lastterm/currnumber;
            }
            operator = clear[i];
            currnumber = 0;

        }
    }
       if(operator == '+')
        {
             result = lastterm+result;
             lastterm = currnumber;
        }
        else if(operator == '-')
        {
            result = lastterm+result;
            lastterm = -currnumber;
        }
        else if(operator == '*')
        {
            lastterm = lastterm*currnumber;
        }
        else if(operator == '/')
        {
            if(currnumber == 0)
            {
                printf("Error: Division by zero.");
                return 0;
            }
            lastterm = lastterm/currnumber;
        }

        printf("%d",lastterm+result);
        return 0;
}