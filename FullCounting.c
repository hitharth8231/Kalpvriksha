void countSort(int arr_rows, int arr_columns, char*** arr) {
      int count[100] = {0};
      int pos[100] = {0};
      
      char **buckets[100];
      
      for(int i=0;i<arr_rows;i++)
      {
        int key = atoi(arr[i][0]);
        count[key]++;
      }
      
      for(int i=0;i<100;i++)
      {
        buckets[i] = malloc(count[i]*sizeof(char*));
      }
      
      for(int i=0;i<arr_rows;i++)
      {
        int key = atoi(arr[i][0]);
        
        if(i<arr_rows/2)
        {
            buckets[key][pos[key]++] = "-";
        }
        else{
            buckets[key][pos[key]++] = arr[i][1];
        }
      }
      
      for(int i=0;i<100;i++)
      {
        for(int j=0;j<count[i];j++)
        {
            printf("%s ",buckets[i][j]);
        }
        free(buckets[i]);
      }
      printf("\n");
}

