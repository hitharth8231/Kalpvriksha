#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Players_data.h"

typedef struct PlayerNode
{
   int PlayerId;
   char name[51];
   char TeamName[51];
   char Role[101];
   long TotalRuns;
   float BattingAverage;
   float StrikeRates;
   int Wickets;
   float EconomyRate;
   float PerformanceIndex;

   struct PlayerNode *next;
}PlayerNode;

typedef struct Team
{
   int teamId;
   char name[51];
   int totalPlayers;
   float averageBattingStrikeRate;
    
    PlayerNode *batsmanHead;
    PlayerNode *bowlerHead;
    PlayerNode *allRounderHead;
}Team;

Team team[10];


float calculatePerformanceIndex(PlayerNode *p)
{
    if(strcmp(p->Role,"Batsman")==0)
    {
        return (p->BattingAverage * p->StrikeRates)/100.0;
    }
    else if(strcmp(p->Role,"Bowler")==0)
    {
        return (p->Wickets*2) +(100- p->EconomyRate);
    }
    else{
          return ((p->BattingAverage * p->StrikeRates)/100.0)+(p->Wickets * 2);
    }
}

Team *searchTeam(int teamId)
{
    int low =0;
    int high = 9;

    while(low<=high)
    {
        int mid = low + (high-low)/2 ;
        if(team[mid].teamId == teamId)
        {
            return &team[mid];
        }
        else if(team[mid].teamId<teamId)
        {
            low = mid+1;
        }
        else{
            high = mid-1;
        }
    }
    return NULL;
}

void insertSorted(PlayerNode **head,PlayerNode *newNode)
{
    newNode->next = NULL;

    if(*head == NULL || newNode->PerformanceIndex > (*head)->PerformanceIndex)
    {
        newNode->next = *head;
        *head = newNode;
    }
    else{
        PlayerNode *temp = *head;
        while(temp->next != NULL && temp->next->PerformanceIndex >= newNode->PerformanceIndex)
        {
            temp = temp->next;
        }

        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void insertPlayer(Team *t,PlayerNode *newNode)
{
    if(strcmp(newNode->Role,"Batsman") == 0)
    {
        insertSorted(&t->batsmanHead,newNode);
    }
    else if(strcmp(newNode->Role,"Bowler") == 0)
    {
        insertSorted(&t->bowlerHead,newNode);
    }
    else
    {
        insertSorted(&t->allRounderHead,newNode);
    }
    t->totalPlayers++;
}

void calculateTeamAverage(Team *t)
{
    float sum =0;
    int count =0;

    PlayerNode *temp = t->batsmanHead;

    while(temp != NULL)
    {
        sum += temp->StrikeRates;
        count++;
        temp = temp->next;
    }

    temp = t->allRounderHead;

    while(temp != NULL)
    {
        sum += temp->StrikeRates;
        count++;
        temp = temp->next;
    }

    if(count == 0)
    {
        t->averageBattingStrikeRate = 0;
    }
    else{
        t->averageBattingStrikeRate = sum/count;
    }
}

void initializeTeams()
{
    for(int i=0;i<10;i++)
    {
        team[i].teamId = i+1;
        strcpy(team[i].name,teams[i]); 

        team[i].totalPlayers = 0;
        team[i].allRounderHead= NULL;
        team[i].batsmanHead= NULL;
        team[i].bowlerHead= NULL;
        team[i].averageBattingStrikeRate = 0.0;
    }
}

int findTeamByName(char name[])
{
    int low = 0,high = 9;

    while(low<=high)
    {
        int mid = low + (high-low)/2;
        int cmp = strcmp(team[mid].name,name);

        if(cmp == 0)
        {
            return mid;
        }
        else if(cmp<0)
        {
            low = mid+1;
        }
        else{
            high = mid-1;
        }
    }
    return -1;
}

void initializePlayer()
{
    for(int i=0;i<playerCount;i++)
    {
        PlayerNode *newNode = malloc(sizeof(PlayerNode));

        if(newNode == NULL)
        {
            printf("Memory Not Allocated!");
            return;
        }

        newNode->PlayerId = players[i].id;
        strcpy(newNode->name, players[i].name);
        strcpy(newNode->TeamName, players[i].team);
        strcpy(newNode->Role, players[i].role);

        newNode->TotalRuns = players[i].totalRuns;
        newNode->BattingAverage = players[i].battingAverage;
        newNode->StrikeRates = players[i].strikeRate;
        newNode->Wickets = players[i].wickets;
        newNode->EconomyRate = players[i].economyRate;

        newNode->PerformanceIndex = calculatePerformanceIndex(newNode);
        newNode->next = NULL;

        int index = findTeamByName(newNode->TeamName);
        if(index != -1)
        {
            insertPlayer(&team[index],newNode);
        }
        else{
            printf("No Team Found!\n");
            free(newNode);
        }
    }

    for(int i=0;i<10;i++)
    {
        calculateTeamAverage(&team[i]);
    }
}

void displayPlayer(PlayerNode *p)
{
     printf("%-6d %-25s %-15s %-8ld %-8.2f %-8.2f "
           "%-6d %-8.2f %-10.2f\n",

           p->PlayerId,
           p->name,
           p->Role,
           p->TotalRuns,
           p->BattingAverage,
           p->StrikeRates,
           p->Wickets,
           p->EconomyRate,
           p->PerformanceIndex);
}

void displayHeader()
{
    printf("========================================================================================\n");

    printf("%-6s %-25s %-15s %-8s %-8s %-8s %-6s %-8s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR",
           "Wkts", "ER", "Perf.Index");

    printf("========================================================================================\n");
}

void displayList(PlayerNode *head)
{
    PlayerNode *temp = head;

    while(temp != NULL)
    {
        displayPlayer(temp);
        temp = temp->next;
    }
}

void addPlayer()
{
    int teamId;

    printf("Enter Team Id to add player: ");
    scanf("%d",&teamId);

    Team *t = searchTeam(teamId);

    if(t == NULL)
    {
        printf("Invalid Team Id!\n");
        return;
    }

    PlayerNode *newNode = malloc(sizeof(PlayerNode));

    if(newNode == NULL)
    {
        printf("Memory not Allocated!\n");
        return;
    }

    newNode->next = NULL;
    newNode->PlayerId = 0;

    printf("Enter Player Details:\n");
    printf("Player Id: ");
    scanf("%d",&newNode->PlayerId);

    if(newNode->PlayerId < 1 || newNode->PlayerId > 1000)
    {
        printf("Player Id must be between 1 and 1000!\n");
        free(newNode);
        return;
    }

    for(int i=0;i<10;i++)
    {
        PlayerNode *temp = team[i].batsmanHead;

        while(temp != NULL)
        {
            if(temp->PlayerId == newNode->PlayerId)
            {
                printf("Player Id already exists!\n");
                free(newNode);
                return;
            }
            temp = temp->next;
        }
        temp = team[i].bowlerHead;

        while(temp != NULL)
        {
            if(temp->PlayerId == newNode->PlayerId)
            {
                printf("Player ID already exists!\n");
                free(newNode);
                return;
            }
            temp = temp->next;
        }

        temp = team[i].allRounderHead;

        while(temp != NULL)
        {
            if(temp->PlayerId == newNode->PlayerId)
            {
                printf("Player Id aready exists!\n");
                free(newNode);
                return;
            }
            temp = temp->next;
        }
    }
    
    printf("Name: ");
    getchar();
    fgets(newNode->name, sizeof(newNode->name), stdin);
    newNode->name[strcspn(newNode->name, "\n")] = '\0';

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int roleChoice;
    scanf("%d", &roleChoice);

    if(roleChoice == 1)
        strcpy(newNode->Role, "Batsman");

    else if(roleChoice == 2)
        strcpy(newNode->Role, "Bowler");

    else if(roleChoice == 3)
        strcpy(newNode->Role, "All-rounder");

    else
    {
        printf("Invalid Role!\n");
        free(newNode);
        return;
    }

    strcpy(newNode->TeamName, t->name);

    printf("Total Runs: ");
    scanf("%ld", &newNode->TotalRuns);

    printf("Batting Average: ");
    scanf("%f", &newNode->BattingAverage);

    printf("Strike Rate: ");
    scanf("%f", &newNode->StrikeRates);

    printf("Wickets: ");
    scanf("%d", &newNode->Wickets);

    printf("Economy Rate: ");
    scanf("%f", &newNode->EconomyRate);

    newNode->PerformanceIndex =
        calculatePerformanceIndex(newNode);

    insertPlayer(t, newNode);
    calculateTeamAverage(t);

    printf("\nPlayer added successfully to Team %s!\n", t->name);
}

void displaySpecificTeam()
{
   int teamId;

   printf("Enter Team Id: ");
   scanf("%d",&teamId);

   Team *t = searchTeam(teamId);

   if(t == NULL)
   {
    printf("Invalid Team Id!\n");
    return;
   }

    printf("\nPlayers of Team %s:\n", t->name);

    displayHeader();

    printf("\n--- Batsmen ---\n");
    displayList(t->batsmanHead);

    printf("\n--- Bowlers ---\n");
    displayList(t->bowlerHead);

    printf("\n--- All-Rounders ---\n");
    displayList(t->allRounderHead);

    printf("========================================================================================\n");

    printf("Total Players: %d\n", t->totalPlayers);

    printf("Average Batting Strike Rate: %.2f\n",
           t->averageBattingStrikeRate);
}

void displayTeamsByAverageSR()
{
    Team temp[10];

      for(int i = 0; i < 10; i++)
        temp[i] = team[i];

    for(int i = 1; i < 10; i++)
    {
        Team key = temp[i];
        int j = i - 1;

        while(j >= 0 &&
              temp[j].averageBattingStrikeRate <
              key.averageBattingStrikeRate)
        {
            temp[j + 1] = temp[j];
            j--;
        }

        temp[j + 1] = key;
    }

    printf("\nTeams Sorted by Average Batting Strike Rate\n");

    printf("===============================================================\n");

    printf("%-5s %-20s %-15s %-10s\n",
           "ID", "Team Name", "Avg Bat SR", "Players");

    printf("===============================================================\n");

    for(int i = 0; i < 10; i++)
    {
        printf("%-5d %-20s %-15.2f %-10d\n",
               temp[i].teamId,
               temp[i].name,
               temp[i].averageBattingStrikeRate,
               temp[i].totalPlayers);
    }

    printf("===============================================================\n");

}

void displayTopK()
{
    int teamId, roleChoice, k;

    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    Team *t = searchTeam(teamId);

    if(t == NULL)
    {
        printf("Invalid Team ID!\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    printf("Enter number of players: ");
    scanf("%d", &k);

    if(k <= 0)
    {
        printf("Invalid K!\n");
        return;
    }

    PlayerNode *head;

    if(roleChoice == 1)
        head = t->batsmanHead;

    else if(roleChoice == 2)
        head = t->bowlerHead;

    else if(roleChoice == 3)
        head = t->allRounderHead;

    else
    {
        printf("Invalid Role!\n");
        return;
    }

    displayHeader();

    PlayerNode *temp = head;
    int count = 0;

    while(temp != NULL && count < k)
    {
        displayPlayer(temp);
        temp = temp->next;
        count++;
    }

    if(count == 0)
        printf("No players found!\n");

    printf("========================================================================================\n");
}

typedef struct {
    PlayerNode *node;
    int teamIndex;
}HeapEntry;

HeapEntry heap[10];
int heapSize = 0;

int compareHeap(HeapEntry a,HeapEntry b)
{
    return a.node->PerformanceIndex > b.node->PerformanceIndex;
}

void swapHeap(HeapEntry *a,HeapEntry *b)
{
    HeapEntry temp = *a;
    *a = *b;
    *b = temp;
}

void heapInsert(HeapEntry entry)
{
    int i = heapSize++;
    heap[i] = entry;

    while(i>0)
    {
        int parent = (i-1)/2;
        if(compareHeap(heap[i],heap[parent]))
            swapHeap(&heap[parent],&heap[i]);
        else
            break;
        i = parent;
    }
}

HeapEntry heapRemove()
{
    HeapEntry result = heap[0];

    heap[0] = heap[--heapSize];

    int i = 0;

    while(1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if(left < heapSize &&
           compareHeap(heap[left], heap[largest]))
            largest = left;

        if(right < heapSize &&
           compareHeap(heap[right], heap[largest]))
            largest = right;

        if(largest == i)
            break;

        swapHeap(&heap[i], &heap[largest]);
        i = largest;
    }

    return result;
}

void displayAllPlayersRole()
{
    int roleChoice;

    printf("Enter Role(1-Batsman,2-Bowler,3-Allrounder): ");
    scanf("%d",&roleChoice);

    if(roleChoice<1 || roleChoice >3)
    {
        printf("Invalid Role!\n");
        return;
    }

    heapSize = 0;

    for(int i=0;i<10;i++)
    {
        PlayerNode *head;

        if(roleChoice == 1)
            head = team[i].batsmanHead;

        else if(roleChoice == 2)
            head = team[i].bowlerHead;

        else
            head = team[i].allRounderHead;

        if(head != NULL)
        {
            HeapEntry entry;

            entry.node = head;
            entry.teamIndex = i;

            heapInsert(entry);
        }
    }

     printf("\nPlayers Across All Teams Sorted by Performance Index\n");

    printf("========================================================================================================\n");

    printf("%-6s %-25s %-15s %-15s %-8s %-8s %-6s %-8s %-10s\n",
           "ID", "Name", "Team", "Role", "Runs",
           "Avg", "SR", "Wkts", "Perf.Index");

    printf("========================================================================================================\n");

    while(heapSize > 0)
    {
        HeapEntry top = heapRemove();

        PlayerNode *p = top.node;

        printf("%-6d %-25s %-15s %-15s %-8ld %-8.2f %-6.2f %-8d %-10.2f\n",
               p->PlayerId,
               p->name,
               p->TeamName,
               p->Role,
               p->TotalRuns,
               p->BattingAverage,
               p->StrikeRates,
               p->Wickets,
               p->PerformanceIndex);

        PlayerNode *next = p->next;

        if(next != NULL)
        {
            HeapEntry entry;

            entry.node = next;
            entry.teamIndex = top.teamIndex;

            heapInsert(entry);
        }
    }

    printf("========================================================================================================\n");
}

void freeList(PlayerNode *head)
{
    PlayerNode *temp = head;

    while(temp != NULL)
    {
        PlayerNode *next = temp->next;
        free(temp);
        temp = next;
    }
}

void freeMemory()
{
    for(int i = 0; i < 10; i++)
    {
        freeList(team[i].batsmanHead);
        freeList(team[i].bowlerHead);
        freeList(team[i].allRounderHead);
    }
}


int main()
{

  initializeTeams();
    initializePlayer();

    int choice;

    do
    {
        printf("\n");
        printf("==============================================================================\n");
        printf("                 ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");

        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");

        printf("==============================================================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addPlayer();
                break;

            case 2:
                displaySpecificTeam();
                break;

            case 3:
                displayTeamsByAverageSR();
                break;

            case 4:
                displayTopK();
                break;

            case 5:
                displayAllPlayersRole();
                break;

            case 6:
                printf("Exiting Program...\n");
                freeMemory();
                break;

            default:
                printf("Invalid Choice!\n");
        }

    }while(choice != 6);

    return 0;

}

