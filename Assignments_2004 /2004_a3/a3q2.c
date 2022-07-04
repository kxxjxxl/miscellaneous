/** <a3q2.c>
* Submitted by: Emad Salim
* Student #: 202020905
*   
* ASSINMENT 3, QUESTION 2
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;
int num;
int n;
int max = -9999999;
int min = 9999999;
int i;
float avg;

void* avg_thread(int arr[]) //thread for calculating the average value
{
    int i;
    for(i = 0; i < n; i++)
    {
        sum += arr[i];
        avg = sum/n;
    }
    pthread_exit(0);
}

void* min_thread(int arr[]) //thread for calculating the minimun value
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(i == 0)
        {
            min = arr[i];
        }
        else if (min > arr[i])
            min = arr[i];
    }
    pthread_exit(0);
}

void* max_thread(int arr[]) //thread for calculating the maximum value
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(i ==0)
        {
            max = arr[i];
        }
    else if(max < arr[i])
        max = arr[i];
    }
    pthread_exit(0);
}


int main(int argc, char **argv)
{
    scanf("%d",&n);
    int arr[n],i; //array declared to hold numbers
    printf("Enter the numbers\n");
    
    for(i=0;i<n;i++){ //reading array values in main itself
        scanf("%d",&arr[i]);
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);//initializing the threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread1, &attr, avg_thread, arr);//creating the threads;
    pthread_create(&thread2, &attr, min_thread, arr);
    pthread_create(&thread3, &attr, max_thread, arr);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("The average value is: %f\n", avg);
    printf("The minimum value is: %d\n", min);
    printf("The maximum value is: %d\n", max);
return 0;
}
