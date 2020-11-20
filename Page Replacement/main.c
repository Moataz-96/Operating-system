#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue) ;
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int dequeue(struct Queue* queue);
int rear(struct Queue* queue);
int front(struct Queue* queue);

int main()
{
    int numOfPages;
    char policy[100];  // change policy from here
    int pages[100];
    int faults[100];
    scanf("%d",&numOfPages);
    scanf("%s",&policy);
    int counter = -1 ;
    do{
        counter ++;
        scanf("%d",&pages[counter]);
    }while(pages[counter] != -1);
    if(counter == 0){
        return 0;
    }

    int i=0,j=0,page=0;

    if(strcmp(policy,"FIFO") == 0)
{
    printf("Replacement Policy = FIFO\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    struct Queue* queue = createQueue(1000);
        int **fifoAlgorithm;
        fifoAlgorithm = (int**)  malloc(numOfPages*sizeof(int*));
        fifoAlgorithm[0]=(int*)malloc(numOfPages*sizeof(int));
	fifoAlgorithm[1]=(int*)malloc(numOfPages*sizeof(int));
        int checkExists = 0,firstElement = 0,numOfFaults = 0;
        i = 0;
        while(pages[i] != -1)
        {
            fifoAlgorithm[i]=(int*)malloc(numOfPages*sizeof(int));
            if(i == 0)
            {
                fifoAlgorithm[0][0] = pages[0];
                enqueue(queue,0);
                for(page = 1 ; page < numOfPages ; page++)
                {
                    fifoAlgorithm[0][page] = -1;
                }
                i++;
                continue;
            }
            /*
                there are 3 cases
                1- last row has empty slot == slot number (0 or 1 or 2 or .....
                2- new page is already existed in last row (page hit) == -1
                3- new page is not existed (page fault)  == -2
            */
            checkExists = -2;
            for(page = 0 ; page < numOfPages ; page++)
            {
                //check first case
                if(fifoAlgorithm[i-1][page] == -1)
                {
                    checkExists = page;
                    break;
                }
                //check second case
                else if(pages[i] == fifoAlgorithm[i-1][page])
                {
                    checkExists = -1;
                    break;
                }
            }
            if(checkExists == -2)
            {
                firstElement = dequeue(queue);
                numOfFaults++ ;
                faults[numOfFaults-1] = i;
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(page == firstElement)
                    {
                        fifoAlgorithm[i][firstElement] = pages[i];
                        enqueue(queue, page);
                        continue;
                    }
                    fifoAlgorithm[i][page] = fifoAlgorithm[i-1][page] ;
                }
            }
            else if(checkExists == -1)
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    fifoAlgorithm[i][page] = fifoAlgorithm[i-1][page] ;
                }
            }
            else
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(page == checkExists)
                    {
                        fifoAlgorithm[i][checkExists] = pages[i];
                        enqueue(queue, page);
                        continue;
                    }
                    fifoAlgorithm[i][page] = fifoAlgorithm[i-1][page] ;
                }
            }
            i++;
        }

        int faultsCounter = 0 ;
for(i = 0 ; i < counter ; i ++)
        {
            if(pages[i] <= 9){
                printf("0%d ",pages[i]);
            }else{
                printf("%d ",pages[i]);
            }
            if(faults[faultsCounter] == i  ){
                printf("F   ");
                faultsCounter ++;
            }else{
                printf("    ");
            }

            for(j = 0 ; j < numOfPages ; j ++)
            {
                if(fifoAlgorithm[i][j] == -1)
                {
                    break;
                }
                if(fifoAlgorithm[i][j] <= 9){
                    printf("0%d ", fifoAlgorithm[i][j]);
                }else{
                    printf("%d ", fifoAlgorithm[i][j]);
                }
            }
            printf("\n");
        }
        printf("-------------------------------------");
        printf("\nNumber of page faults = %d\n",numOfFaults);


    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    else if(strcmp(policy,"LRU") == 0)
{
    printf("Replacement Policy = LRU\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int **lruAlgorithm;
    lruAlgorithm = (int**)  malloc(numOfPages*sizeof(int*));
        lruAlgorithm[0]=(int*)malloc(numOfPages*sizeof(int));
        lruAlgorithm[1]=(int*)malloc(numOfPages*sizeof(int));
        int checkExists = 0,firstElement = 0,numOfFaults = 0;

        i = 0;
        while(pages[i] != -1)
        {
            lruAlgorithm[i]=(int*)malloc(numOfPages*sizeof(int));
            if(i == 0)
            {
                lruAlgorithm[0][0] = pages[0];
                for(page = 1 ; page < numOfPages ; page++)
                {
                    lruAlgorithm[0][page] = -1;
                }
                i++;
                continue;
            }
            checkExists = -2;
            for(page = 0 ; page < numOfPages ; page++)
            {
                //check first case
                if(lruAlgorithm[i-1][page] == -1)
                {
                    checkExists = page;
                    break;
                }
                //check second case
                else if(pages[i] == lruAlgorithm[i-1][page])
                {
                    checkExists = -1;
                    break;
                }
            }

            if(checkExists == -2)
            {
                numOfFaults++ ;
                faults[numOfFaults-1] = i;
                int x = 0;
                int checkFlag =0;
                int *flag = (int*)malloc(numOfPages*sizeof(int));
                for(x = 0 ; x < numOfPages ; x++){
                    flag[x] = -1;
                }
                x = 0;
                for(j = i-1 ; j >= 0 ; j --)
                {
                    checkFlag = 0;
                    for(page = 0 ; page < numOfPages ; page++)
                    {
                        if(flag[x] == pages[j])
                        {
                            checkFlag = 1;
                        }
                    }
                    if(checkFlag == 1)
                    {
                        continue;
                    }
                    if(x == numOfPages)
                    {
                        break;
                    }
                    for(page = 0 ; page < numOfPages ; page++)
                    {
                        if(pages[j] == lruAlgorithm[i-1][page])
                        {
                            flag[x] = pages[j];
                            x++;
                        }
                    }
                }
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(flag[numOfPages-1] == lruAlgorithm[i-1][page])
                    {
                        lruAlgorithm[i][page] = pages[i];
                    }
                    else
                    {
                        lruAlgorithm[i][page] = lruAlgorithm[i-1][page]  ;
                    }
                }

            }
            else if(checkExists == -1)
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    lruAlgorithm[i][page] = lruAlgorithm[i-1][page] ;
                }
            }
            else
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(page == checkExists)
                    {
                        lruAlgorithm[i][checkExists] = pages[i];
                        continue;
                    }
                    lruAlgorithm[i][page] = lruAlgorithm[i-1][page] ;
                }
            }

            i++;
        }
        int faultsCounter = 0 ;
      for(i = 0 ; i < counter ; i ++)
        {
            if(pages[i] <= 9){
                printf("0%d ",pages[i]);
            }else{
                printf("%d ",pages[i]);
            }
            if(faults[faultsCounter] == i  ){
                printf("F   ");
                faultsCounter ++;
            }else{
                printf("    ");
            }

            for(j = 0 ; j < numOfPages ; j ++)
            {
                if(lruAlgorithm[i][j] == -1)
                {
                    break;
                }
                if(lruAlgorithm[i][j] <= 9){
                    printf("0%d ", lruAlgorithm[i][j]);
                }else{
                    printf("%d ", lruAlgorithm[i][j]);
                }
            }
            printf("\n");
        }
        printf("-------------------------------------");
        printf("\nNumber of page faults = %d\n",numOfFaults);

    }
    //-----------------------------------------
    // CLOCK
    else if(strcmp(policy,"CLOCK") == 0)
{
    printf("Replacement Policy = CLOCK\n");
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int **clockAlgorithm;
    clockAlgorithm = (int**)  malloc(numOfPages*sizeof(int*));
        clockAlgorithm[0]=(int*)malloc(numOfPages*sizeof(int));
	clockAlgorithm[1]=(int*)malloc(numOfPages*sizeof(int));
        int checkExists = 0,firstElement = 0,numOfFaults = 0;
        int *clockCycle = (int*)malloc(numOfPages*sizeof(int));
        for(i = 0 ; i < numOfPages ; i++){
            clockCycle[i] =0;
        }

        int startPoints = 0,flag = 0;
        i = 0;
        while(pages[i] != -1)
        {
            clockAlgorithm[i]=(int*)malloc(numOfPages*sizeof(int));
            if(i == 0)
            {
                clockAlgorithm[0][0] = pages[0];
                for(page = 1 ; page < numOfPages ; page++)
                {
                    clockAlgorithm[0][page] = -1;
                }
                i++;
                continue;
            }
            checkExists = -2;
            for(page = 0 ; page < numOfPages ; page++)
            {
                //check first case
                if(clockAlgorithm[i-1][page] == -1)
                {
                    checkExists = page;
                    if(page == numOfPages -1)
                    {
                        startPoints = 1;
                    }
                    break;
                }
                //check second case
                else if(pages[i] == clockAlgorithm[i-1][page])
                {
                    checkExists = -1;
                    break;
                }
            }

            if(checkExists == -2)
            {
                //    startPoints = 1;
                flag = 0 ;
                for(j = 0 ; j < numOfPages ; j++)
                {
                    if(clockCycle[j] == 0)
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0)
                {
                    for(j = 0 ; j < numOfPages ; j++)
                    {
                        clockCycle[j] = 0;
                    }
                }
                for(j = 0 ; j < numOfPages ; j++)
                {
                    if(clockCycle[j] == 0)
                    {
                        clockAlgorithm[i][j] = pages[i];
                        numOfFaults ++;
                        faults[numOfFaults-1] = i;
                        flag = j;
                        clockCycle[j] = 1;
                        break;
                    }
                }
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(flag == page)
                    {
                        continue;
                    }
                    clockAlgorithm[i][page] = clockAlgorithm[i-1][page] ;
                }

            }
            else if(checkExists == -1)
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(startPoints == 1 && pages[i] == clockAlgorithm[i-1][page])
                    {
                        clockCycle[page] = 1;
                    }
                    clockAlgorithm[i][page] = clockAlgorithm[i-1][page] ;
                }
            }
            else
            {
                for(page = 0 ; page < numOfPages ; page++)
                {
                    if(page == checkExists)
                    {
                        clockAlgorithm[i][checkExists] = pages[i];
                        continue;
                    }
                    clockAlgorithm[i][page] = clockAlgorithm[i-1][page] ;
                }
            }

            i++;
        }
        int faultsCounter = 0 ;
        for(i = 0 ; i < counter ; i ++)
        {
            if(pages[i] <= 9){
                printf("0%d ",pages[i]);
            }else{
                printf("%d ",pages[i]);
            }
            if(faults[faultsCounter] == i  ){
                printf("F   ");
                faultsCounter ++;
            }else{
                printf("    ");
            }

            for(j = 0 ; j < numOfPages ; j ++)
            {
                if(clockAlgorithm[i][j] == -1)
                {
                    break;
                }
                if(clockAlgorithm[i][j] <= 9){
                    printf("0%d ", clockAlgorithm[i][j]);
                }else{
                    printf("%d ", clockAlgorithm[i][j]);
                }
            }
            printf("\n");
        }
        printf("-------------------------------------");
        printf("\nNumber of page faults = %d\n",numOfFaults);
    }
    return 0;
}


// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}
