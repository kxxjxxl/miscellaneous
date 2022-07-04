#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {

    int input = atoi(argv[1]);
    for(int i = 1; i <= input;i++)
    if(i%5==0 && i%3 == 0)
    printf("threefive\n");

    else if(i%3 == 0)
    printf("three\n");

    else if(i%5 == 0)
    printf("five\n");

    else
    printf("%d\n",i);
    
    return 0;
}
