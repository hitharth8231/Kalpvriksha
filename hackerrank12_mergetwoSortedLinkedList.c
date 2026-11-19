#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
}Node;

Node* createlinkedlist(int size)
{
    if(size<1)
    {
        return NULL;
    }
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

Node* mergeTwoLists(Node *head1, Node *head2)
{
    Node dummy;
    dummy.next = NULL;

    Node *tail = &dummy;

    while(head1 != NULL && head2 != NULL)
    {
        if(head1->data <= head2->data)
        {
            tail->next = head1;
            head1 = head1->next;
        }
        else
        {
            tail->next = head2;
            head2 = head2->next;
        }

        tail = tail->next;
    }

    if(head1 != NULL)
        tail->next = head1;
    else
        tail->next = head2;

    return dummy.next;
}

int main()
{
   int size1;
   printf("Enter size of LinkedList1: ");
   scanf("%d",&size1);

    int size2;
   printf("Enter size of LinkedList2: ");
   scanf("%d",&size2);

   if(size1 < 0 || size2<0)
   {
    return 1;
   }

   Node *head1 = createlinkedlist(size1);

   Node *head2 = createlinkedlist(size2);

   printf("Linked List1: ");
   Node *current = head1;
   while(current)
   {
    printf("%d ",current->data);
    current = current->next;
   }

   printf("Linked List2: ");
   current = head2;
   while(current)
   {
    printf("%d ",current->data);
    current = current->next;
   }

   Node *mergedHead = mergeTwoLists(head1, head2);

    printf("\nMerged Linked List: ");

    current = mergedHead;

    while(current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }


    current = mergedHead;

    while(current != NULL)
    {
        Node *head_next = current->next;
        free(current);
        current = head_next;
    }
   return 0;
}

