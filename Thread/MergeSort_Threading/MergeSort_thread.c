#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void printArray(int *list, int size);  //print elements
int* getElements(FILE *inputFile);	//get elements from file
void merge(int *a,int low, int high);	// conquer the 2 arrays
void* merge_thread(void *arg);		// divide to 2 arrays using thread
void* merge_normal(void *arg);		// divide to 2 arrays without using threads
static int numOfElements;
//struct as a parameter to thread
struct MergedArray
{
    int low;
    int high;
    int *arry;
};
//default functions its exist in library
//struct timespec {                                                                                     
//    time_t   tv_sec;        /* seconds */                                                             
//    long     tv_nsec;       /* nanoseconds */                                                         
//};  

int main(){
    FILE *inputFile = fopen("input.txt","r"); //file name = input.txt
    if (inputFile == NULL){
        printf("Please Put input file\n");
        return 0;
        }
    // start calculate time
    struct timespec start, finish; 
    clock_gettime(CLOCK_REALTIME, &start); 


    struct MergedArray mergeArry;
    mergeArry.arry = getElements(inputFile);
    mergeArry.low = 0;
    mergeArry.high = numOfElements - 1;
    fclose(inputFile);
    printArray(mergeArry.arry,numOfElements); // print elements
    //now low index is 0 , high is Number of elements -1
    //Merge Using Thread
    pthread_t pid; // create object of thread
    pthread_create(&pid,NULL,merge_thread,&mergeArry); //create new thread
    pthread_join(pid,NULL);	// wait untill all thread finished
    //Merge in normal way
    //merge_normal(&mergeArry);
    printArray(mergeArry.arry,numOfElements); // print elements


    // end calculating time
    clock_gettime(CLOCK_REALTIME, &finish); 

    long seconds = finish.tv_sec - start.tv_sec; 
    long ns = finish.tv_nsec - start.tv_nsec; 
    
    if (start.tv_nsec > finish.tv_nsec) { // clock underflow 
	--seconds; 
	ns += 1000000000; 
    } 

    printf("Threading Mergesort took: %e\n", (double)seconds + (double)ns/(double)1000000000); 
    return 0;
}
void merge(int *a,int low, int high)
{
    int mid = (low+high)/2;	//get mid element
    int* left = (int*)malloc((mid - low + 1)*sizeof(int)); // casting left array
    int* right = (int*)malloc((high - mid)*sizeof(int));	// casting right array
    int n1 = mid - low + 1, n2 = high - mid, i, j;
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];
    int k = low;
    i = j = 0;
    // fill left array
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }
    //fill right array
    while (i < n1) {
        a[k++] = left[i++];
    }
    //combine 2 arrays
    while (j < n2) {
        a[k++] = right[j++];
    }
}

void* merge_thread(void *arg){

    struct MergedArray *element = (struct MergedArray*)arg;
    struct MergedArray leftArry;
    struct MergedArray rightArry;
    int mid = (element->low + element->high)/2;
    leftArry.low = element->low;
    leftArry.high = mid;
    leftArry.arry = element->arry;
    rightArry.low = mid +1 ;
    rightArry.high = element->high;
    rightArry.arry = element->arry;
    if(element->low >= element->high){
        return;
    }
    pthread_t divideThread1;
    pthread_t divideThread2;
    pthread_create(&divideThread1,NULL,merge_thread,&leftArry); //recursivly left thread
    pthread_create(&divideThread2,NULL,merge_thread,&rightArry);	//recursivly right thread
    pthread_join(divideThread1,NULL); 	//wait untill both all recursived
    pthread_join(divideThread2,NULL);	//wait untill both all recursived
    merge(element->arry,element->low,element->high);	//merge
    pthread_exit(NULL);	//return null and clean thread
}
void* merge_normal(void *arg){

    struct MergedArray *element = (struct MergedArray*)arg;
    struct MergedArray leftArry;
    struct MergedArray rightArry;
    int mid = (element->low + element->high)/2;
    leftArry.low = element->low;
    leftArry.high = mid;
    leftArry.arry = element->arry;
    rightArry.low = mid +1 ;
    rightArry.high = element->high;
    rightArry.arry = element->arry;
    if(element->low >= element->high){
        return;
    }
    merge_normal(&leftArry);
    merge_normal(&rightArry);
    merge(element->arry,element->low,element->high);
}
void printArray(int *list, int size) {
    int i;
    for (i=0; i<size-1; i++)
        printf("%d, ", list[i]);
    printf("%d\n", list[i]);
}
int* getElements(FILE *inputFile)
{

    fscanf(inputFile, "%d", &numOfElements);
    int *inputArray;
    inputArray = (int*)malloc((numOfElements)*sizeof(int));
    int i;

    for (i = 0; i < numOfElements; i++)
    {
        fscanf(inputFile, "%d", &inputArray[i]);
    }

    return inputArray;

}
