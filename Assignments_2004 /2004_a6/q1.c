#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>            // all the thread libraries
#include<Windows.h>           
#include<unistd.h>             
                               
int sum = 0;                   // initializing the sum variables
int read_val = 0;
int rw_bit = 1;
void *read_thd()
{
    do
    {
        while(!rw_bit)                       // looping condition
            Sleep(1000);
        int val;                           
        printf("Input a number : ");
        scanf("%d", &val);
        read_val = val;                      // storing values in shared memory
        rw_bit = 0;
        if(val < 0)
        {
            pthread_exit(NULL);
        }
    }
    while(1);
}
                                             
void *sum_thd()                              //initialising sum of the thread function
{
    do
    {
        while(rw_bit)                        //loop condition
            Sleep(1000);
        int val = read_val;
        if(val < 0)   // if the value is -ve, exit
        {
                                         
            pthread_exit(NULL);
        }
        sum += val; // sum=sum+val;
        rw_bit = 1;
    }
    while(1);
}
int main()
{
    pthread_t read_t_id;
    pthread_t sum_t_id;
                                                               // initialising the read thread
    pthread_create(&read_t_id, NULL, read_thd, NULL);
                                                               // starting sum of the thread
    pthread_create(&sum_t_id, NULL, sum_thd, NULL);
															  
    pthread_join(sum_t_id, NULL);
    printf("Sum is %d\n", sum);
    return 0;
}