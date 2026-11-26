int getTwiceMedian(int count[],int d)
{
    int cumulative = 0;
    int first = -1;
    int second = -1;
    
    for(int i=0;i<=200;i++)
    {
        cumulative += count[i];
        if(first == -1 && cumulative >= (d+1)/2)
        {
            first = i;
        }
        
        if(cumulative >= d/2 +1)
        {
            second = i;
            break;
        }
    }
    return first+second;
}
int activityNotifications(int expenditure_count, int* expenditure, int d) {

     int count[201] = {0};
     int notifications = 0;
     
     for(int i=0;i<d;i++)
     {
        count[expenditure[i]]++;
     }
     
     for(int i=d;i<expenditure_count;i++)
     {
        int twiceMedian = getTwiceMedian(count,d);
        
        if(expenditure[i] >= twiceMedian)
        {
            notifications++;
        }
        
        count[expenditure[i-d]]--;
        count[expenditure[i]]++;
     }
     return notifications;
}

