#include<stdio.h>

struct Booking
{
   char name[40];
   int ticket_count;
   int category_choice;
};

int main()
{
   struct Booking booking;

   // Taking input from user
   printf("Input: \n");
   printf("Name: ");
   if( scanf("%39s",booking.name)!= 1)
   {
      printf("Invalid input\n");
      return 1;
   } 
   printf("Tickets: ");
   if( scanf("%d",&booking.ticket_count)!= 1)
   {
      printf("Invalid input\n");
      return 1;
   } 
   printf("Category: ");
   if( scanf("%d",&booking.category_choice)!= 1)
   {
      printf("Invalid input\n");
      return 1;
   } 

   //For valid ticket count
    if(booking.ticket_count < 1 || booking.ticket_count >20)
   {
     printf("Invalid Ticket input");
      return 1;
   }
   //For valid category
   if(booking.category_choice < 1 || booking.category_choice >3)
   {
     printf("Invalid Category input");
      return 1;
   }

   printf("Output: \n\n");
   printf("Customer: %s\n",booking.name);
   float subtotal = booking.ticket_count;
   const int Standard = 150;
   const int Premium = 250;
   const int Recliner = 400;

   // switch case for category identifying  
   switch(booking.category_choice)
   {
     case 1:
        printf("Ticket Category: Standard\n");
        subtotal = subtotal *Standard;
        break;

    case 2:
        printf("Ticket Category: Premium\n");
        subtotal = subtotal *Premium;
        break;

    case 3:
        printf("Ticket Category: Recliner\n");
        subtotal = subtotal *Recliner;
        break;

   }

      printf("Tickets: %d\n",booking.ticket_count);
      printf("Subtotal: %.2f\n",subtotal);

      float discount = 0.00;

      if(booking.ticket_count >=5)
      {
         discount = subtotal*(0.10);
      }

      printf("Discount: %.2f\n",discount);

      float final = subtotal - discount;
      printf("Final Bill: %.2f\n",final);
      return 0;
}