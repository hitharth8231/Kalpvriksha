#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Block{
    int start;
    int size;
    int allocation_id;
    int is_allocated;
};


int firstfit(struct Block metadata[], int count,int requestedSize)
{
    for(int i=0;i<count;i++)
    {
        if(metadata[i].is_allocated == 0 && metadata[i].size>= requestedSize)
        {
            return i;
        }
    }
    return -1;
}

int bestfit(struct Block metadata[], int count,int requestedSize)
{ int min = -1;
    for(int i=0;i<count;i++)
    {
        if(metadata[i].is_allocated == 0 && metadata[i].size>= requestedSize)
        {
            if(min == -1 || metadata[i].size < metadata[min].size)
            {
               min = i;
            }
        }
    }
    return min;
}

int worstfit(struct Block metadata[], int count,int requestedSize)
{ int max = -1;
    for(int i=0;i<count;i++)
    {
        if(metadata[i].is_allocated == 0 && metadata[i].size>= requestedSize)
        {
            if(max == -1 || metadata[i].size > metadata[max].size)
            {
               max = i;
            }
        }
    }
    return max;
}


int ensureCapacity(struct Block **metadata,int *capacity,int required)
{
    if(required<= *capacity)
    {
        return 1;
    }
    int newCapacity = *capacity;

    while(newCapacity<required)
    {
        newCapacity *= 2;
    }
    struct Block *temp = realloc(*metadata,newCapacity*sizeof(struct Block));
    if(temp == NULL)
    {
        return 0;
    }
    *metadata = temp;
    *capacity = newCapacity;

    return 1;
}

int insertBlock(struct Block **metadata, int *count, int *capacity, int index, struct Block newBlock)
{
    if(!ensureCapacity(metadata,capacity,*count+1))
    {
        return 0;
    }
    for(int i= *count;i>index;i--)
    {
        (*metadata)[i] = (*metadata)[i-1];
    }
    (*metadata)[index] = newBlock;
    (*count)++;
    return 1;
}

void removeBlock(struct Block metadata[],int *count,int index)
{
    for(int i=index;i<*count-1;i++)
    {
        metadata[i] = metadata[i+1];
    }
    (*count)--;
}

void mergeFreeBlocks(struct Block metadata[],int *count)
{
    int i=0;
    while(i<*count-1)
    {
        if(metadata[i].is_allocated == 0 && metadata[i+1].is_allocated == 0)
        {
            metadata[i].size += metadata[i+1].size;

            removeBlock(metadata,count,i+1);
        }
        else{
            i++;
        }
    }
}

int allocateAtIndex(struct Block **metadata,int *count,int *capacity,int index,int requestedSize, int allocationId)
{
    int oldStart = (*metadata)[index].start;
    int oldSize = (*metadata)[index].size;

    if(oldSize>requestedSize)
    {
        struct Block freeBlock;

        freeBlock.start = oldStart + requestedSize;
        freeBlock.size = oldSize - requestedSize;
        freeBlock.allocation_id =0;
        freeBlock.is_allocated =0;

        if(!insertBlock(metadata,count,capacity,index+1,freeBlock))
        {
            return 0;
        }
    }

    (*metadata)[index].start = oldStart;
    (*metadata)[index].size = requestedSize;
    (*metadata)[index].allocation_id = allocationId;
    (*metadata)[index].is_allocated = 1;

    return 1;
}
void allocatememory(unsigned char *memory,struct Block **metadata,int *count, 
    int *capacity, int poolSize,int strategy)
{
   int requestedSize;
   int allocationId;

   printf("Enter Allocation Size: ");
   scanf("%d", &requestedSize);

   if(requestedSize<=0 || requestedSize>poolSize)
   {
    printf("Invalid Allocation Size.");
    return;
   }

   printf("Enter Allocation Id: ");
   scanf("%d",&allocationId);

   for(int i=0;i<*count;i++)
   {
    if((*metadata)[i].is_allocated == 1 && (*metadata)[i].allocation_id == allocationId)
    {
        printf("Duplicate Allocation Id.");
        return;
    }
   }


   int index =-1;

   if(strategy == 1)
   {
    index = firstfit(*metadata, *count, requestedSize);
   }
   else if(strategy == 2)
   {
     index = bestfit(*metadata, *count, requestedSize);
   }
   else if(strategy == 3)
   {
     index = worstfit(*metadata, *count, requestedSize);
   }
   else{
    printf("Invalid strategy Input.");
    return;
   }

   int totalFree = 0;

    for (int i = 0; i < *count; i++)
    {
        if ((*metadata)[i].is_allocated == 0)
        {
            totalFree += (*metadata)[i].size;
        }
    }

    if (index == -1)
    {
        if (totalFree >= requestedSize)
        {
            printf("Allocation failed: External Fragmentation!\n");
        }
        else
        {
            printf("Allocation failed: Insufficient Memory!\n");
        }

        return;
    }

    if((*metadata)[index].size > requestedSize && *count == *capacity)
    {
        int newCapacity = (*capacity)*2;

        struct Block *temp = realloc(*metadata,newCapacity*sizeof(struct Block));

        if(temp == NULL)
        {
            printf("Metadata rsizing failed!\n");
            return;
        }

        *metadata= temp;
        *capacity = newCapacity;
    }

    int oldStart = (*metadata)[index].start;
    int oldSize = (*metadata)[index].size;

    if(oldSize > requestedSize)
    {
        for(int i=*count;i>index+1;i--)
        {
            (*metadata)[i] = (*metadata)[i-1];
        }

        (*metadata)[index + 1].start = oldStart + requestedSize;
        (*metadata)[index + 1].size = oldSize - requestedSize;
        (*metadata)[index + 1].allocation_id = 0;
        (*metadata)[index + 1].is_allocated = 0;

        (*count)++;
    }

    (*metadata)[index].start = oldStart;
    (*metadata)[index].size = requestedSize;
    (*metadata)[index].allocation_id = allocationId;
    (*metadata)[index].is_allocated = 1;


    printf("Memory allocated successfully!\n");
    printf("Allocation ID: %d\n",allocationId);
    printf("Starting Address: %d\n",oldStart);
    printf("Allocated Size: %d bytes\n",requestedSize);
}

void releasememory(struct Block *metadata,int *count)
{
   int allocationId;
   int found =0;

   printf("Enter Allocation ID to release: ");
   scanf("%d",&allocationId);

   for(int i=0;i<*count;i++)
   {
    if(metadata[i].allocation_id == allocationId && metadata[i].is_allocated == 1)
    {
        metadata[i].is_allocated =0;
        metadata[i].allocation_id =0;
        found =1;
        break;
    }
   }

   if(found == 0)
   {
    printf("Allocation ID not found!\n");
    return;
   }

   mergeFreeBlocks(metadata,count);
   printf("Allocation released successfully!\n");
   
}

void resizeAllocation(unsigned char *memory,struct Block **metadata,int *count, int *capacity,int poolSize, int strategy)
{
    int allocationId;
    int newSize;
    int index = -1;

    printf("Enter Allocation ID: ");
    scanf("%d",&allocationId);

    printf("Enter New Size: ");

    if(scanf("%d",&newSize) != 1)
    {
        return;
    }
    if(newSize <= 0 || newSize> poolSize)
    {
        printf("Invalid Size!\n");
        return;
    }

    for(int i=0;i<*count;i++)
    {
        if((*metadata)[i].is_allocated == 1 && (*metadata)[i].allocation_id == allocationId)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("Allocation ID not found!\n");
        return;
    }

    int oldSize =(*metadata)[index].size;
    int oldStart =(*metadata)[index].start;

    if(newSize == oldSize)
    {
        printf("New size is same as old size.\n");
        return;
    }

    if(newSize<oldSize)
    {
        struct Block freeBlock;
        freeBlock.start = oldStart + newSize;
        freeBlock.size = oldSize - newSize;
        freeBlock.allocation_id = 0;
        freeBlock.is_allocated = 0;
        
        if(!insertBlock(metadata,count,capacity,index+1,freeBlock))
        {
            printf("Metadata resizing failed!\n");
            return;
        }
        (*metadata)[index].size = newSize;

        mergeFreeBlocks(*metadata,count);
        printf("Allocation resized successfully!\n");
        return;
    }

    int extra = newSize-oldSize;
    if(index+1<*count && (*metadata)[index+1].is_allocated == 0 && (*metadata)[index+1].size>=extra)
    {
        if((*metadata)[index+1].size == extra)
        {
            removeBlock(*metadata,count,index+1);
        }
        else{
            (*metadata)[index+1].start += extra;
            (*metadata)[index+1].size -= extra;
        }
        (*metadata)[index].size = newSize;

        printf("Expanded in place successfully!\n");
        return;
    }

        int newIndex =-1;

         if (strategy == 1)
         {
           newIndex= firstfit(metadata, *count, newSize);
         }

        if (strategy == 2)
            {newIndex= bestfit(metadata, *count, newSize);}

        if (strategy == 3)
           { newIndex= worstfit(metadata, *count, newSize);}

        
            if(newIndex == -1)
            {
                printf("Resize failed: No suitable free block!\n");
                printf("Original allocation remains unchanged.\n");
                return;
            }

            unsigned char *temp = malloc(oldSize);

            if(temp == NULL)
            {
                printf("Temporary memory allocation failed!\n");
                return;
            }

            memcpy(temp,memory+oldStart,oldSize);
            int newStart = (*metadata)[newIndex].start;

            if(!allocateAtIndex(metadata,count,capacity,newIndex,newSize,allocationId))
            {
                free(temp);
                printf("Resize failed: Metadata allocation failed!\n");
                return;
            }
            memcpy(memory+newStart,temp,oldSize);
            free(temp);

            for(int i=0;i<*count;i++)
            {
                if((*metadata)[i].start == oldStart && (*metadata)[i].allocation_id == allocationId && 
                (*metadata)[i].is_allocated == 1 )
                {
                    (*metadata)[i].is_allocated =0;
                    (*metadata)[i].allocation_id =0;
                    break;
                }
            }

            mergeFreeBlocks(*metadata,count);
            printf("Allocation moved and resized successully!\n");
            printf("New Starting address: %d\n",newStart);
            printf("New Size: %d bytes\n",newSize);
    }


void displayMemoryMap(struct Block metadata[],int count, int poolSize)
{
    printf("\n-----Memory Map------\n");
    printf("Memory Pool: %d bytes \n",poolSize);

    for(int i=0;i<count;i++)
    {
        if(metadata[i].is_allocated == 1)
        {
            printf("[Allocated #%d: %d bytes, Start: %d]\n",
                    metadata[i].allocation_id,
                    metadata[i].size,
                    metadata[i].start);
        }
        else{
            printf("[Free: %d bytes,Start: %d]\n",metadata[i].size,metadata[i].start);
        }
    }
}

void displayStatistics(struct Block metadata[],
                       int count, int poolSize)
{
    int allocatedBytes = 0;
    int freeBytes = 0;

    int allocatedBlocks = 0;
    int freeBlocks = 0;

    int largestFreeBlock = 0;

    for (int i = 0; i < count; i++)
    {
        if (metadata[i].is_allocated == 1)
        {
            allocatedBytes += metadata[i].size;
            allocatedBlocks++;
        }
        else
        {
            freeBytes += metadata[i].size;
            freeBlocks++;

            if (metadata[i].size > largestFreeBlock)
                largestFreeBlock = metadata[i].size;
        }
    }

    printf("\n------ MEMORY STATISTICS ------\n");

    printf("Total Memory: %d bytes\n", poolSize);
    printf("Allocated Memory: %d bytes\n", allocatedBytes);
    printf("Free Memory: %d bytes\n", freeBytes);

    printf("Allocated Blocks: %d\n", allocatedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);

    printf("Largest Free Block: %d bytes\n", largestFreeBlock);
}

void changeStrategy(int * strategy)
{
   int choice;

    printf("\n1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");

    printf("Select Strategy: ");

    if (scanf("%d", &choice) != 1)
        return;

    if (choice < 1 || choice > 3)
    {
        printf("Invalid Strategy!\n");
        return;
    }

    *strategy = choice;

    printf("Strategy changed successfully!\n"); 
}
int main()
{
    int poolSize;
    unsigned char *memory;
    struct Block *metadata;
    int choice;
    int count=1;
    int capacity=1;
    int strategy =1;

    printf("Enter Pool Size: ");
    scanf("%d",&poolSize);

    if(poolSize<1 || poolSize >1000000)
    {
        printf("Invalid pool size\n");
        return 1;
    }

    memory = malloc(poolSize*sizeof(unsigned char));
    if(memory == NULL)
    {
        printf("Memory allocation failed");
        return 1;
    }

    metadata = malloc(capacity*sizeof(struct Block));
    if(metadata == NULL)
    {
        printf("Metadata Allocation Failed");
        free(memory);
        return 1;
    }

    metadata[0].start =0;
    metadata[0].size =poolSize;
    metadata[0].allocation_id =0;
    metadata[0].is_allocated =0;

    do
    {
    printf("\n-----MENU----\n");
    printf("1.Allocate Memory\n");
    printf("2.Release Memory\n");
    printf("3.Resize an allocation\n");
    printf("4.Display Memory Map\n");
    printf("5.Display Memory Status\n");
    printf("6.Change allocation Strategy\n");
    printf("7.Exit\n\n");

    printf("Enter Choice: ");
    scanf("%d",&choice);

      switch (choice)
        {
            case 1:
                allocatememory(memory, &metadata,
                               &count, &capacity,
                               poolSize, strategy);
                break;

            case 2:
                releasememory(metadata, &count);
                break;

            case 3:
                resizeAllocation(memory, &metadata,
                                 &count, &capacity,
                                 poolSize, strategy);
                break;

            case 4:
                displayMemoryMap(metadata, count, poolSize);
                break;

            case 5:
                displayStatistics(metadata, count, poolSize);
                break;

            case 6:
                changeStrategy(&strategy);
                break;

            case 7:
                printf("Exiting Memory Manager...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }
    }while(choice != 7);

    free(metadata);
    free(memory);
}

