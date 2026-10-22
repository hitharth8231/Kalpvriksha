#include<stdio.h>

struct Employee
{
    int Employee_id;
    char name[40];
    char Destination[40];
    int manager_id;
};
int AddEmployee(int i,struct Employee employee[])
{
    printf("Enter Employee Id: ");
    if(scanf("%d",&employee[i].Employee_id)!= 1)
    {
       printf("Invalid Employee Id.\n");
       return 0;
    }
    printf("Enter name: ");
    if(scanf("%39s",employee[i].name)!= 1)
    {
       printf("Invalid Name.\n");
       return 0;
    }
    printf("Enter Destination: ");
    if(scanf("%39s",employee[i].Destination)!= 1)
    {
       printf("Invalid Destination.\n");
       return 0;
    }
    printf("Enter Reporting_manager(-1 if none): ");
     if(scanf("%d",&employee[i].manager_id)!= 1)
    {
       printf("Invalid Manager Id.\n");
       return 0;
    }

    printf("User Added.\n");
    return 1;

}

int RemoveEmployee(int RemoveEmployee_id,struct Employee employee[],int count)
{
    int i=0;
    if(count == 0)
    {
        printf("No Employee Registered Till Now.");
        return 0;
    }
   while(i<count && RemoveEmployee_id != employee[i].Employee_id)
   {
    i++;
   }
   if(i==count)
    {
        printf("Invalid Employee Id.");
        return 0;
    }
   int RemoveEmployeeManagerId = employee[i].manager_id;
   for(int j=0;j<count;j++)
   {
    if(employee[j].manager_id == RemoveEmployee_id)
    {
        employee[j].manager_id = RemoveEmployeeManagerId;
    }
   }
   while(i < count-1)
   {
    employee[i] = employee[i+1];
    i++;
   }

   return 1;
}

void DisplayEmployeeHierarchy(int Employee_id,struct Employee employee[],int count,int level)
{
    int i=0;
    while(i<count && Employee_id != employee[i].Employee_id)
    {
      i++;
    }
    if(i == count)
    {
        printf("Employee Not Found.");
        return;
    }
    for(int k=0;k<level;k++)
    {
        printf("    ");
    }
    printf("%d %s %s\n",employee[i].Employee_id,employee[i].name,employee[i].Destination);
    for(int j=0;j<count;j++)
    {
        if(employee[i].Employee_id == employee[j].manager_id)
        {
            DisplayEmployeeHierarchy(employee[j].Employee_id,employee,count,level+1);
        }
    }
}

int CountEmployees(int Employee_id, struct Employee employee[], int count)
{
    int total = 0;

    for(int j = 0; j < count; j++)
    {
        if(employee[j].manager_id == Employee_id)
        {
            total++;  
            total = CountEmployees(employee[j].Employee_id,employee,count)+total;
        }
    }

    return total;
}



int main()
{
    struct Employee employee[100];
    int n;
    int i=0;

do{
    printf("===== EMPLOYEE HIERARCHY MANAGEMENT =====\n\n");
    printf("1. Add Employee\n");
    printf("2. Remove Employee\n");
    printf("3. Display Employee Hierarchy\n");
    printf("4. Display Complete Hierarchy\n");
    printf("5. Exit\n\n");
    printf("Enter your choice:\n");
    if(scanf("%d", &n) != 1)
    {
        printf("Invalid menu input. Try again.\n");
        return 0;
    }

    switch(n)
    {
        case 1:
          {
                if(i >= 100)
                {
                    printf("Employee limit reached.\n");
                    break;
                }
            if(AddEmployee(i,employee)) 
               i++;
          break;
          }

        case 2:
        {
           int RemoveEmployee_id;
           printf("Enter Employee Id to remove: ");
           scanf("%d",&RemoveEmployee_id);
           if(RemoveEmployee(RemoveEmployee_id,employee,i))
           {
            i--;
            printf("Employee Removed.\n");
           }
           break;
        }
        
        case 3:
        {
          int Employee_id;
          printf("Enter Employee Id to display: ");
          scanf("%d",&Employee_id);
          DisplayEmployeeHierarchy(Employee_id, employee, i, 0);
          printf("\n\n");
          printf("Total employees under this employee: %d\n",
          CountEmployees(Employee_id, employee, i));
          break;
        }
        
        case 4:
        {
            if(i ==0)
            {
                printf("No Employee Registered Till Now.\n");
                break;
            }
            int foundRoot=0;
            printf("\n===== COMPLETE EMPLOYEE HIERARCHY =====\n");
            for(int j = 0; j < i; j++)
            {
                if(employee[j].manager_id == -1)
                {
                    foundRoot = 1;
                    DisplayEmployeeHierarchy(employee[j].Employee_id, employee, i, 0);
                }
            }
            if(foundRoot == 0)
            {
                printf("No top-level employee found. Please check the hierarchy.\n");
            }
            else{
            printf("\nTotal employees in organization: %d\n", i);
            }
            break;
        }
        case 5:
         printf("Exit.");
         break;

        default:
            printf("Invalid choice. Select 1 to 5.\n");
            break;
          
    }
}while(n!=5);


}