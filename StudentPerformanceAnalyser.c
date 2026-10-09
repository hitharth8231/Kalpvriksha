#include<stdio.h>

struct Student{
    int rollno;
    char name[40];
    int marks1, marks2,marks3;
};
 
void ListRoll(struct Student students[],int i, int p)
    {
       if(i == p)
       {
        return;
       } 
       int roll = students[i].rollno;

       printf("%d ",roll);
       ListRoll(students,i+1,p);
    }


int main()
{
    int n;
    scanf("%d", &n);
    if(n>100 || n<0)
    {
       printf(" Invalid input for students");
       return 0;
    }

    struct Student students[n];
    printf("Input: \n\n");
    for(int i=0;i<n;i++)
    {
        scanf("%d %s %d %d %d", &students[i].rollno,students[i].name,&students[i].marks1,&students[i].marks2,&students[i].marks3);
        if(students[i].marks1 > 100 ||students[i].marks2 > 100 || students[i].marks3 > 100
             || students[i].marks1 < 0 ||students[i].marks2 < 0 || students[i].marks3 < 0 )
             {
                printf("Invalid Marks");
                return 0;
             }
    }

    printf("\nOutput: \n\n");

    for(int i=0;i<n;i++)
    {
        int total;
        float average;

        total = students[i].marks1 + students[i].marks2 + students[i].marks3;
        average = total/3.0;
        printf("Roll: %d\n", students[i].rollno);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", total);
        printf("Average: %.2f\n", average);

            if(average >= 85)
            {
                printf("Grade: A\n");
                printf("Performance: *****");
            }
            else if(average >= 70)
            {
                printf("Grade: B\n");
                printf("Performance: ****");
            }
            else if(average >= 50)
            {
                printf("Grade: C\n");
                printf("Performance: ***");
            }
            else if(average >= 35)
            {
                printf("Grade: D\n");
                printf("Performance: **");
            }
            else{
                printf("Grade: F");
            }
            printf("\n\n");
       }

       printf("List of Roll Numbers (via recursion): ");
       ListRoll(students,0,n);

       return 0;
}