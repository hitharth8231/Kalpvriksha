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

Node *reverseLinkedlist(Node **head)
{
    Node *previous = NULL;
    while(*head)
    {
        Node *head_next = (*head)->next;
        (*head)->next = previous;
        previous = *head;
        *head = head_next;
    }
    return previous;
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
   if(head == 1)
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

   head = reverseLinkedlist(&head);

   current = head;
   printf("\nReversed LinkedList: ");
   while(current)
   {
     printf("%d ",current->data);
     current = current->next;
   }

   current = head;
   while(head)
   {
    head = head->next;
    free(current);
    current = head;
   }

   return 0;
}

