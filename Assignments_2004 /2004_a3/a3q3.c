/** <a3q2.c>
* Submitted by: Emad Salim
* Student #: 202020905
*   
* ASSINMENT 3, QUESTION 3
*
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#define MIN_PID 300
#define MAX_PID 5000
#define TRUE 1
#define FALSE 0
#define NUM_THREADS argv[1]
sem_t SEM;

struct PidTable {
    int PID;
    int isAvailable;
}*Pid;

int allocate_map()
{
    int x;
    Pid=(struct PidTable *)calloc((MAX_PID-MIN_PID+1),sizeof(struct PidTable));
    if(Pid == NULL)
        return -1; //returning -1 if initiaziling data structures fail
    Pid[0].PID=MIN_PID;
    Pid[0].isAvailable=TRUE;
    for(x=1; x < MAX_PID-MIN_PID+1; x++){
        Pid[x].PID = Pid[x-1].PID+1;
        Pid[x].isAvailable = TRUE;
    }
return 1; //returning 1 if initiaziling data structures successful
}

int allocate_pid()
{
    int x ;
    for( x=0; x<MAX_PID-MIN_PID+1; x++){ //allocating a pid
        if(Pid[x].isAvailable == TRUE){
            Pid[x].isAvailable = FALSE;
            return Pid[x].PID; //returning a pid
        }
    }
    if(x == MAX_PID - MIN_PID + 1) 
    return -1; //returning -1 if unable to allocate a pid
}

void release_pid(int pid){
    Pid[pid - MIN_PID].isAvailable = TRUE;
}

void *processStart(void *id){
        int pid,executionTime;
        sem_wait(&SEM);
        pid=allocate_pid();

        usleep(int argv[2]);
        sem_post(&SEM);
        if(pid!=-1){
                printf("New Process Allocated Pid= %d \n",pid);
                executionTime=rand()%10;
                sleep(executionTime);
                printf("Process %d releasing pid \n",pid);
                release_pid(pid);
        }
        pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    allocate_map();
    srand(time(NULL));
    void *status;int i;
    int ret=0;pthread_t thread[100];
    sem_init(&SEM,0,1);
    for(i=0;i < NUM_THREADS;i++){
        ret=pthread_create(&thread[i],NULL,processStart,NULL);
        if(ret){printf("Error creating thread\n");exit(1);}
    }
    for(i=0; i < NUM_THREADS; i++) {
      ret = pthread_join(thread[i], &status);
      if (ret) {
         printf("ERROR; return code from pthread_join() is %d\n", ret);
         exit(-1);
         }
    }
    return 0;
}