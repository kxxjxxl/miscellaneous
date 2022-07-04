#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main()
{
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd1[2]; 
    int fd2[2]; 
    int i;
    pid_t pid;

    /* create the pipe */
    if (pipe(fd1) == -1)
    {
        fprintf(stderr,"Pipe failed\n");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr,"Pipe failed\n");
        return 1;
    }

    printf("Enter a string : ");
    fgets(read_msg, BUFFER_SIZE, stdin);
    read_msg[strlen(read_msg)-1]='\0';
    pid = fork();

    /* fork a child process */
    if (pid < 0)/* error occurred */
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }

    if (pid > 0){ /* the parent process */
        /* closing the unused end of the pipe */
        close(fd1[READ_END]); 
        /* writing to the pipe */
        write(fd1[WRITE_END], read_msg, strlen(read_msg)+1);
        close(fd1[WRITE_END]);
        wait(NULL);
        /* closing the WRITE_END of the pipe */
        close(fd2[WRITE_END]);
        read(fd2[READ_END], read_msg, BUFFER_SIZE);
        printf("%s\n", read_msg);
        close(fd2[READ_END]);
    }
    else{/* child process */
        /* closing the unused end of the pipe */
        close(fd1[WRITE_END]);
        /* reading from the pipe */
        read(fd1[READ_END], read_msg, BUFFER_SIZE);
        /* closing the READ_END of the pipe */
        close(fd1[READ_END]);
        close(fd2[READ_END]);
        printf("read %s\n", read_msg);
        for(i = 0; i < strlen(read_msg); i++)
        {
            if(islower(read_msg[i])) //lower case letter being converted to upper case
            {
            read_msg[i]=toupper(read_msg[i]);
            }
        else if(isupper(read_msg[i])) //upper case letter being converted to lower case
        {
        read_msg[i]=tolower(read_msg[i]);
        }
    }
    write(fd2[WRITE_END], read_msg, strlen(read_msg)+1);
    close(fd2[WRITE_END]);
    exit(0);
    }
}