#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    //intialzing the number to 0
        int num = 0
        pid_t p;
        do
        {   //getting the number
            printf ( "Enter a number greater than 0: ");
            scanf("ed", &num);
        } while (num <=0);
        //intialziing the parent id to fork
        p = fork();
        //if parent id is equal to 0
        if (p == 0){
            printf("Child is working....\n");
            printf("%d\n", num);
            //checking and printing all the positive numbers that reach 1
            while(num!=1){
                if (num%2 == 0){
                     num = num/2;

                }
                else if(num%2 == 1){
                    num = 2 * (num) +1;


                }
                            }
}