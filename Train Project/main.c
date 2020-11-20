#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define MAX_STATION_NUMBER 250
#define NUM_OF_PASSENGERS 250
#define NUM_OF_TRAINS 10
pthread_mutex_t passengerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trainMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t boardMutex = PTHREAD_MUTEX_INITIALIZER;
int trainID = 0;
time_t t;
typedef struct{
    int numOfPassengers;
    int newComers;
    int availableSeats;
}station;
void station_load_train(station *station , int count);
void *station_wait_for_train(station *station);
void station_on_board(station *station);
void *train(station *station);
void *generatePassengersThread(station *station);
void *generateTrainsThread(station *station);
int main()
{
    station s;
    srand(time(NULL));
    s.numOfPassengers = 0;
    pthread_mutex_lock(&boardMutex);
    pthread_mutex_lock(&passengerMutex);

    sleep(2);
    //----------------------------

    pthread_t generatePassengers;
    pthread_t generateTrains;
    pthread_create(&generatePassengers,NULL,generatePassengersThread,&s);
    pthread_create(&generateTrains,NULL,generateTrainsThread,&s);
    pthread_join(generatePassengers,NULL);
    pthread_join(generateTrains,NULL);
    //----------------------------

    return 0;
}
void *generatePassengersThread(station *station){
    pthread_t passenger;
    int totalStation = NUM_OF_PASSENGERS;
    station->numOfPassengers  = 0;
    station->newComers  = 0;
    while(1){
        if(station->numOfPassengers < MAX_STATION_NUMBER){
        //srand((unsigned int) time(NULL));

        for(int i = 0 ; i < station->newComers; i++){
            pthread_create(&passenger,NULL,station_wait_for_train,station);}

        }else{
            return;
        }
    }
    /*
    for(int i = 0 ; i < NUM_OF_PASSENGERS+1 ; i++){
        pthread_create(&passenger,NULL,station_wait_for_train,station);}
    for(int i = 0 ; i < NUM_OF_PASSENGERS+1 ; i++){
        pthread_join(passenger,NULL);}
    */
}
void *generateTrainsThread(station *station){
    pthread_t trains;
    for(int i = 0 ; i < NUM_OF_TRAINS ; i++){
        pthread_create(&trains,NULL,train,station);}

    for(int i = 0 ; i < NUM_OF_TRAINS ; i++){
        pthread_join(trains,NULL);}

    exit(0);
}
void *station_wait_for_train(station *station){
    back_thread_to_lock_mutex:
    pthread_mutex_lock(&boardMutex);
    if((station->availableSeats > 0) && (station->numOfPassengers > 0)){
        station_on_board(station);
        pthread_mutex_unlock(&boardMutex);
    }else{
        printf("Total Passengers in station After Train is :%d\n\n\n",station->numOfPassengers);
        pthread_mutex_unlock(&trainMutex);
        pthread_mutex_unlock(&passengerMutex);
        
        if(station->numOfPassengers > 0){ //this is passenger back him to station
            goto back_thread_to_lock_mutex;
        }else{ //this is advisor of station
         //   pthread_mutex_destroy(&boardMutex);
          //  pthread_mutex_destroy(&passengerMutex);
           // pthread_mutex_destroy(&trainMutex);
        }
    }
    return NULL;
}
void *train(station *station){
    pthread_mutex_lock(&trainMutex); // to ensure that no 2 trains will be in the same station
  //  srand((unsigned int) time(NULL));
    station->newComers = rand() % 100;;
    int oldPass = station->numOfPassengers;
    station->numOfPassengers += station->newComers;
    printf("Total Passengers Before Train[%d] = Old Passenger[%d] + New Comers[%d]  :\n",station->numOfPassengers,oldPass,station->newComers);
    station->availableSeats = rand() % 100;
    trainID ++;

    printf("Train[%d] is comming with available seats is :%d... \n",trainID,station->availableSeats);
    station_load_train(station,station->availableSeats);
    pthread_mutex_lock(&passengerMutex); // to ensure that train will not run before passengers jump
    sleep(1);
    return NULL;
}
void station_load_train(station *station , int count){
    pthread_mutex_unlock(&boardMutex);
}
void station_on_board(station *station){
    station->numOfPassengers--;
    station->availableSeats--;
}
