#include<stdio.h>
#include<string.h>

struct User
{
    int id;
    char name[40];
    int age;
};

void createUser()
{
     struct User user;
     FILE* file;

     scanf("%d", &user.id);
     scanf("%s", user.name);
     scanf("%d", &user.age);

     file =fopen("users.txt","a");

     if(file == NULL)
     {
        printf("File Not Opened ");
        return;
     }
     else{
        fprintf(file,"%d %s %d\n",user.id, user.name, user.age);
     }
     fclose(file);
}

void readUser()
{
    struct User user;
    FILE* file;
    file = fopen("users.txt","r");
     if(file == NULL)
     {
        printf("File Not Opened ");
        return;
     }
     else
     {
       while(fscanf(file,"%d %s %d\n", &user.id, user.name,&user.age) == 3){
       printf("%d %s %d\n",user.id,user.name,user.age);
       }
     }
     fclose(file);

}

void updateUser()
{
    FILE* file;
    FILE* temp;

    struct User user;
    int updateId;
    char newName[40];
    int age;

    scanf("%d %s %d",&updateId,newName,&age);
    file = fopen("users.txt","r");
    if(file == NULL)
    {
        printf("File Not Opened ");
        return;
    }
    else{
        temp = fopen("temp.txt","w");
        if(temp == NULL)
        {
            printf("File Not Opened ");
            fclose(file);
            return;
        }
    while(fscanf(file,"%d %s %d\n", &user.id,user.name,&user.age) == 3)
    {
        if(user.id == updateId)
        {
           strcpy(user.name,newName);
           user.age =  age;
           fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
        else{
          fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
    }
    fclose(temp);
}
    fclose(file);
    remove("users.txt");
    rename("temp.txt","users.txt");
}

void deleteUser()
{
    FILE* file;
    FILE* temp;

    struct User user;
    int deleteId;

    scanf("%d",&deleteId);
    file = fopen("users.txt","r");
    if(file == NULL)
    {
        printf("File Not Opened ");
        return;
    }
    else{
        temp = fopen("temp.txt","w");
        if(temp == NULL)
        {
            printf("File Not Opened ");
            fclose(file);
            return;
        }
    while(fscanf(file,"%d %s %d\n", &user.id,user.name,&user.age) == 3)
    {
        if(user.id == deleteId)
        {
          continue;
        }
        else{
          fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
    }
    fclose(temp);
}
    fclose(file);
    remove("users.txt");
    rename("temp.txt","users.txt");
}

int main()
{
    int choice;

    do
    {
       printf("\n==operations==\n");
       printf("1. Create User\n");
       printf("2. Read Users\n");
       printf("3. Update Users\n");
       printf("4. Delete Users\n");
       printf("5. Exit\n");

       printf("Enter your choice\n");
       scanf("%d",&choice);

       switch(choice)
       {
        case 1:
            createUser();
            break;

        case 2:
            readUser();
            break;

        case 3:
            updateUser();
            break;

        case 4:
            deleteUser();
            break;

        case 5:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice\n");
       }
    } while (choice != 5);
    return 0;
}