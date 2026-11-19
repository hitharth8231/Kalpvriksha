#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
}Node;

Node* createlinkedlist(int size)
{
    Node* dummy = malloc(sizeof(Node));
    if(dummy == NULL)
       {
        printf("Memory allocation faliure!");
        return NULL;
       }
    dummy->next = NULL;
    Node *head = dummy;

    printf("Enter elements of linked list: ");
    while(size)
    {
       Node *temp = malloc(sizeof(Node));
       if(temp == NULL)
       {
        printf("Memory allocation faliure!");
        head = dummy->next;
        free(dummy);
        while(head != NULL)
        {
            Node *head_next = head->next;
            free(head);
            head = head_next;
        }
        return NULL;
       }

       temp->next =NULL;
       scanf("%d",&temp->data);
       head->next = temp;
       head = temp;
       size--;
    }
    head = dummy->next;
    free(dummy);
    return head;
}

Node *cyclecreation(Node **head,int index)
{
    if(index == -1)
    {
        return *head;
    }
    Node *current = *head;
    while(index)
    {
        current = current->next;
        index--;
    }
    
    Node *tail = current;
    while(tail->next != NULL)
    {
        tail = tail->next;
    }
    tail->next = current;
    return *head;
}

void cycledetection(Node **head)
{
    Node *fast = *head;
    Node *slow = *head;

    while(fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        slow = slow->next;

        if(fast == slow)
        {
            printf("Cycle detectd!");
            return;
        }
    }
    printf("Cycle not detected!");
}

int main()
{
   int size;
   printf("Enter size of LinkedList: ");
   scanf("%d",&size);

   if(size <= 0)
   {
    return 1;
   }

   Node *head = createlinkedlist(size);
   if(head == NULL)
   {
    return 1;
   }

   printf("Original Linked List: ");
   Node *current = head;

   while(current)
   {
    printf("%d ",current->data);
    current = current->next;
   }

   printf("Enter a Node index where you need to form cycle(-1 if no cycle): ");
   int index;
   scanf("%d",&index);

   if(index < size && index >=-1)
   {
    head = cyclecreation(&head,index);
    cycledetection(&head);
   }
   else{
    printf("Invalid Index!");
   }

   return 0;
}

