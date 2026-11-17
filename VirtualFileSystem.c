#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

unsigned char virtualDisk[5000][512];
int totalBlocks = 0;
int allocateBlock(void);
void freeBlock(int index);

struct FreeBlock{
    int index;

    struct FreeBlock *next;
    struct FreeBlock *prev;
};

struct FreeBlock *freeHead = NULL;
struct FreeBlock *freeTail = NULL;

struct FileNode
{
    char name[51];
    bool isDirectory;

    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;
    struct FileNode *children;

    int *blockPointers;
    int blockCount;
    int contentSize;
};

struct FileNode *root = NULL;
struct FileNode *cwd = NULL;

void insertNode(struct FileNode *parent, struct FileNode *newNode)
{
    newNode->parent = parent;

    if(parent->children == NULL)
    {
        newNode->next = newNode;
        newNode->prev = newNode;

        parent->children = newNode;
    }
    else
    {
        struct FileNode *head = parent->children;
        struct FileNode *tail = head->prev;

        newNode->next = head;
        newNode->prev = tail;

        tail->next = newNode;
        head->prev = newNode;
    }
}

struct FileNode *createNode(char *name, bool isDirectory)
{
    struct FileNode *newNode = malloc(sizeof(struct FileNode));
    if(newNode == NULL)
    {
        return NULL;
    }
    newNode->isDirectory = isDirectory;
    strcpy(newNode->name,name);

    newNode->parent = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->children = NULL;
    newNode->blockPointers = NULL;
    newNode->blockCount = 0;
    newNode->contentSize = 0;

    return newNode;
}

struct FileNode *findEntry(struct FileNode *parent,char *name)
{
    if(parent->children == NULL)
    {
        return NULL;
    }
    struct FileNode *current = parent->children;
    do
    {
        if(strcmp(current->name,name) == 0)
        {
            return current;
        }
        current = current->next;
    } while (current != parent->children);

    return NULL;
}

void removeNode(struct FileNode *node)
{
    struct FileNode *nodeParent = node->parent;
    if(node->next == node)
    {
        nodeParent->children = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if(nodeParent->children == node)
        {
            nodeParent->children = node->next;
        }
    }
    free(node->blockPointers);
    free(node);
}

void mkdirCommand(char *name)
{
    if(name == NULL || strlen(name) == 0 || strlen(name) > 50)
    {
        printf("\nInvalid directory name!");
        return;
    }

   struct FileNode *newNode = findEntry(cwd,name);

   if(newNode == NULL)
   {
    newNode = createNode(name,true);

    if(newNode == NULL)
    {
        printf("\nMemory allocation failed!");
        return;
    }

    insertNode(cwd,newNode);
    printf("\nDirectory created Successfully!");
   }
   else
   {
    printf("\nFile Name Already Exists!");
    return;
   }
}

void createCommand(char *name)
{
    if(name == NULL || strlen(name) == 0 || strlen(name) > 50)
    {
        printf("\nInvalid file name!");
        return;
    }
   struct FileNode *newNode = findEntry(cwd,name);

   if(newNode == NULL)
   {
    newNode = createNode(name,false);

    if(newNode == NULL)
    {
        printf("\nMemory allocation failed!");
        return;
    }

    insertNode(cwd,newNode);
    printf("\nFile created Successfully!");
    
   }
   else
   {
    printf("\nFile Name Already Exists!");
    return;
   }
}

void lsCommand()
{
   if(cwd->children == NULL)
   {
    printf("\nEmpty List.");
    return;
   }

   struct FileNode *current = cwd->children;
    do
    {
        if(current->isDirectory == true)
        {
           printf("[DIR]  %s\n",current->name); 
        }
        else
        {
          printf("[FILE]  %s\n",current->name);
        }
        current = current->next;
    } while (current != cwd->children);
}


void cd(char *name)
{
    if(strcmp(name,"/") == 0)
    {
        cwd = root;
        printf("\nMoved to root dicrectory");
        return;
    }

    if(strcmp(name,"..") == 0)
    {
        if(cwd->parent != NULL)
        {
            cwd = cwd->parent;
        }
        printf("\nMoved to parent dicrectory");
        return;
    }

    if(strcmp(name,".") == 0)
    {
        return;
    }

    struct FileNode *newNode = findEntry(cwd,name);

    if(newNode == NULL || newNode->isDirectory == false)
    {
        printf("\nDirectory not found");
        return;
    }
    cwd = newNode;
    printf("\nDirectory changed successfully!");
}

void pwdCommand()
{
    struct FileNode *current = cwd;

    char path[100][51];
    int count =0;

    while(current != root && count< 100)
    {
        strcpy(path[count],current->name);
        count++;
    
        current = current->parent;
    }
    printf("/");

    for(int i=count-1;i>=0;i--)
    {
        printf("%s",path[i]);
        if(i != 0)
        {
            printf("/");
        }
    }
    printf("\n");
}

void deleteCommand(char *name)
{
   if(name == NULL || strlen(name) == 0)
   {
    printf("\nInvalid file name!");
    return;
   }

   struct FileNode *node = findEntry(cwd,name);

   if(node == NULL)
    {
        printf("\nFile not found!");
        return;
    }

    if(node->isDirectory)
    {
        printf("\nCannot delete a directory using delete!");
        return;
    }

    for(int i = 0; i < node->blockCount; i++)
    {
        freeBlock(node->blockPointers[i]);
    }
   removeNode(node);
   printf("\nNode Deleted Successfully!");
}

void rmdirCommand(char *name)
{
    if(name == NULL || strlen(name) == 0 || strlen(name)>50)
    {
        printf("\nTnvalid directory name!");
        return;
    }

    if(strcmp(name,"/") == 0)
    {
        printf("\nRoot directory cannot be deleted!");
        return;
    }

    struct FileNode *node = findEntry(cwd,name);

    if(node == NULL)
    {
        printf("\nDirectory not found!");
        return;
    }

    if(node->isDirectory == false)
    {
        printf("\nCannot remove a file using rmdir!");
        return;
    }

    if(node->children != NULL)
    {
        printf("\nDirectory is not empty!");
        return;
    }
    removeNode(node);
    printf("\nDirectory removed successfully!");
}

void cleanupDirectory(struct FileNode *parent)
{
    if(parent->children == NULL)
    {
        return;
    }

    struct FileNode *head = parent->children;
    struct FileNode *current = head;

    do{
        struct FileNode *nextNode = current->next;
        if(current->isDirectory)
        {
            cleanupDirectory(current);
        }

        free(current->blockPointers);
        free(current);
        current = nextNode;
    }while(current != head);

    parent->children = NULL;
}

void cleanup()
{
    if(root == NULL)
    {
        return;
    }

    cleanupDirectory(root);

    struct FreeBlock *current = freeHead;

    while(current != NULL)
    {
        struct FreeBlock *next = current->next;
        free(current);
        current = next;
    }

    freeHead = NULL;
    freeTail = NULL;

    free(root);

    root = NULL;
    cwd = NULL;

    printf("\nFile system memory released successfully!");
}

bool initializeDisk(int blocks)
{
    if(blocks<1 || blocks>5000)
    {
        printf("\nInvalid number of blocks!");
        return false;
    }

    totalBlocks = blocks;

    for(int i=0;i<totalBlocks;i++)
    {
        struct FreeBlock *newBlock = malloc(sizeof(struct FreeBlock));

        if(newBlock == NULL)
        {
            printf("\nMemory allocation failed!");

            struct FreeBlock *current = freeHead;

            while(current != NULL)
            {
                struct FreeBlock *next = current->next;
                free(current);
                current = next;
            }

            freeHead = NULL;
            freeTail = NULL;
            totalBlocks =0;

            return false;
        }

        newBlock->index = i;
        newBlock->next = NULL;
        newBlock->prev = freeTail;

        if(freeHead == NULL)
        {
            freeHead = newBlock;
            freeTail = newBlock;
        }
        else{
            freeTail->next = newBlock;
            freeTail = newBlock;
        }
    }
    return true;
}

int allocateBlock()
{
    if(freeHead == NULL)
    {
        printf("\nDisk is full!");
        return -1;
    }

    struct FreeBlock *temp = freeHead;
    int allocatedIndex = temp->index;

    freeHead = freeHead->next;

    if(freeHead == NULL)
    {
        freeTail = NULL;
    }
    else{
        freeHead->prev = NULL;
    }
    free(temp);
    return allocatedIndex;
}

void freeBlock(int index)
{
    struct FreeBlock *newBlock = malloc(sizeof(struct FreeBlock));

    if(newBlock == NULL)
    {
        printf("\nMemory allocation failed!");
        return;
    }

    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeTail;

    if(freeHead == NULL)
    {
        freeHead = newBlock;
        freeTail = newBlock;
    }
    else{
        freeTail->next = newBlock;
        freeTail = newBlock;
    }
}

void writeCommand(char *name,char *content)
{
    if(name == NULL || content == NULL)
    {
        printf("\nInvalid input!");
        return;
    }

    struct FileNode *node = findEntry(cwd,name);

    if(node == NULL)
    {
        printf("\nFile not found!");
        return;
    }
    if(node->isDirectory)
    {
        printf("\nCannot write into a directory!");
        return;
    }

    int contentSize = strlen(content);
    int neededBlocks = (contentSize + 511)/512;

    int freeCount =0;
    struct FreeBlock *current = freeHead;

    while(current != NULL)
    {
        freeCount++;
        current = current->next;
    }

    int reuseCount = neededBlocks;

    if(reuseCount > node->blockCount)
    {
        reuseCount = node->blockCount;
    }

    int additionalBlocks = neededBlocks - reuseCount;

    if(additionalBlocks>freeCount)
    {
        printf("\nNot enough disk space!");
        return;
    }

    int *newPointers = NULL;

    if(neededBlocks>0)
    {
        newPointers = malloc(neededBlocks*sizeof(int));
        if(newPointers == NULL)
        {
            printf("\nMemory allocation failed!");
            return;
        }
    }

    for(int i=0;i<reuseCount;i++)
    {
        newPointers[i] = node->blockPointers[i];
    }

    for(int i=reuseCount;i<neededBlocks;i++)
    {
        int allocatedIndex = allocateBlock();

        if(allocatedIndex == -1)
        {
            for(int j = reuseCount;j<i;j++)
            {
                freeBlock(newPointers[j]);
            }

            free(newPointers);
            printf("\nBlock allocation failed!");
            return;
        }
        newPointers[i] = allocatedIndex;
    }

    for(int i = 0; i < neededBlocks; i++)
    {
        int offset = i * 512;
        int remaining = contentSize - offset;
        int bytesToCopy = remaining > 512 ? 512 : remaining;

        memset(virtualDisk[newPointers[i]], 0, 512);

        memcpy(
            virtualDisk[newPointers[i]],
            content + offset,
            bytesToCopy
        );
    }

    for(int i=reuseCount;i<node->blockCount;i++)
    {
        freeBlock(node->blockPointers[i]);
    }
    free(node->blockPointers);

    node->blockPointers = newPointers;
    node->blockCount = neededBlocks;
    node->contentSize = contentSize;

    printf("\nFile written successfully!");
}

void readCommand(char *name)
{
    if(name == NULL || strlen(name) == 0)
    {
        printf("\nInvalid file name!");
        return;
    }

    struct FileNode *node = findEntry(cwd, name);

    if(node == NULL)
    {
        printf("\nFile not found!");
        return;
    }

    if(node->isDirectory)
    {
        printf("\nCannot read a directory!");
        return;
    }

    if(node->contentSize == 0)
    {
        printf("\nFile is empty.");
        return;
    }

    int remaining = node->contentSize;

    for(int i = 0; i < node->blockCount; i++)
    {
        int bytesToRead = remaining > 512 ? 512 : remaining;

        fwrite(
            virtualDisk[node->blockPointers[i]],
            sizeof(unsigned char),
            bytesToRead,
            stdout
        );

        remaining -= bytesToRead;
    }

    printf("\n");
}

void dfCommand()
{
    int freeCount = 0;
    struct FreeBlock *current = freeHead;

    while(current != NULL)
    {
        freeCount++;
        current = current->next;
    }

    int usedBlocks = totalBlocks-freeCount;

    printf("\n========== DISK INFORMATION ==========\n");

    printf("Total Blocks : %d\n", totalBlocks);
    printf("Used Blocks  : %d\n", usedBlocks);
    printf("Free Blocks  : %d\n", freeCount);

    printf("Total Space  : %d bytes\n", totalBlocks * 512);
    printf("Used Space   : %d bytes\n", usedBlocks * 512);
    printf("Free Space   : %d bytes\n", freeCount * 512);

    printf("======================================\n");
}

int main()
{
    root = malloc(sizeof(struct FileNode));
    if(root == NULL)
    {
        printf("Memory allocation failed!");
        return 1;
    }
    root->children = NULL;
    root->parent = NULL;
    root->next = root;
    root->prev = root;
    root->isDirectory = true;
    root->blockPointers = NULL;
    root->blockCount = 0;
    root->contentSize = 0;
    strcpy(root->name,"/");

    cwd = root;

    int blocks;

    printf("Enter total number of blocks (1-5000): ");
    if(scanf("%d", &blocks) != 1)
    {
        printf("\nInvalid Input");
        cleanup();
        return 1;
    }


    if(!initializeDisk(blocks))
    {
        cleanup();
        return 1;
    }

    char input[2048];

    printf("\nVirtual File System Started!\n");
    printf("Type 'exit' to close the system.\n");

    while(1)
    {
        printf("\n%s> ",cwd->name);

        if(fgets(input,sizeof(input),stdin) == NULL)
        {
            break;
        }

        input[strcspn(input,"\n")] = '\0';

        char *ptr = input;

        while(*ptr == ' ' || *ptr == '\t')
        {
            ptr++;
        }
        if(*ptr == '\0')
        {
            continue;
        }

        char *command = ptr;

        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\t')
        {
            ptr++;
        }

        if(*ptr != '\0')
        {
            *ptr = '\0';
            ptr++;
        }
        while(*ptr == ' ' || *ptr == '\t')
        {
            ptr++;
        }

        char *arg1 = ptr;

        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\t')
        {
            ptr++;
        }

        char *content = NULL;

        if(*ptr != '\0')
        {
            *ptr = '\0';
            ptr++;

            while(*ptr == ' ' || *ptr == '\t')
            {
                ptr++;
            }
            content = ptr;
        }

        if(strcmp(command,"mkdir") == 0)
        {
            if(strlen(arg1) == 0)
            {
                printf("\nUsage: mkdir directory_name");
            }
            else{
                mkdirCommand(arg1);
            }
        }
         else if(strcmp(command, "create") == 0)
        {
            if(strlen(arg1) == 0)
                printf("\nUsage: create file_name");
            else
                createCommand(arg1);
        }

        else if(strcmp(command, "write") == 0)
        {
            if(strlen(arg1) == 0 || content == NULL)
                printf("\nUsage: write file_name content");
            else
                writeCommand(arg1, content);
        }

        else if(strcmp(command, "read") == 0)
        {
            if(strlen(arg1) == 0)
                printf("\nUsage: read file_name");
            else
                readCommand(arg1);
        }

        else if(strcmp(command, "delete") == 0)
        {
            if(strlen(arg1) == 0)
                printf("\nUsage: delete file_name");
            else
                deleteCommand(arg1);
        }

        else if(strcmp(command, "rmdir") == 0)
        {
            if(strlen(arg1) == 0)
                printf("\nUsage: rmdir directory_name");
            else
                rmdirCommand(arg1);
        }

        else if(strcmp(command, "cd") == 0)
        {
            if(strlen(arg1) == 0)
                printf("\nUsage: cd directory_name");
            else
                cd(arg1);
        }

        else if(strcmp(command, "ls") == 0)
        {
            lsCommand();
        }

        else if(strcmp(command, "pwd") == 0)
        {
            pwdCommand();
        }

        else if(strcmp(command, "df") == 0)
        {
            dfCommand();
        }

        else if(strcmp(command, "exit") == 0)
        {
            printf("\nClosing Virtual File System...");
            break;
        }

        else
        {
            printf("\nUnknown command!");
        }
    }

    cleanup();
    return 0;
}