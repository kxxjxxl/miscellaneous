#include <stdio.h>
#include <stdlib.h>

#define MIN 300
#define MAX 5000
#define False 0
#define True 1


int allocate_map(void);
int allocate_pid(void);
void release_pid(int);


typedef struct linked_list{
    struct linked_list *next;       
    int pid;                        
}node;

node *head = NULL;
node *tail = NULL;

int allocate_map(void){
    head = (node *)malloc(sizeof(node));
    tail = (node *)malloc(sizeof(node));
    if ((head == NULL) || (tail == NULL)){
        return -1;
    }else{
        return 1;
    }
}
int allocate_pid(void){
    node *temp = (node *)malloc(sizeof(node));
    int to_allocate = MIN;

    if (head == NULL){      
        printf("No Data Structure found for storing PIDs.\ninitiate a PID Map before allocating a PID.\n");
        return -1;
    }else if(head->pid == 0){    
        head->pid = MIN;
        head->next = NULL;
        tail = head;
        return head->pid;
    }else if(head->pid != MIN){   
        temp->pid = MIN;
        temp->next = head;
        head = temp;
        return temp->pid;
    }else{
        node *curr = head;
        node *prev = NULL;
        int found = False;  
        
        
        while(curr != NULL && (found != True)){
            if(curr->pid >= MAX){   
                printf("No Memory Left\n");
                return -1;
            }else if(prev == NULL){  
                prev = curr;         
                curr = curr->next;  
                ++to_allocate;
            }else if(curr->pid != (prev->pid + 1)){ 
                to_allocate = prev->pid + 1;      
                found = True;                       
            }else{
                prev = curr;
                curr = curr->next;
                ++to_allocate;
            }
        }
    }
    
    
    int iterator = MIN;     
    node *iter = head;
    node *previous = NULL;
    // Iterate till the "iter" node has not reached the position before which the new PID node is to be inserted. */
    while(iterator < to_allocate){
        previous = iter;
        iter = iter->next;
        iterator++;
    }
    
    temp->pid = to_allocate;    
    temp->next = iter;
    previous->next = temp;
    return temp->pid;
}


void release_pid(int pid){
    node *temp = head;
    node *prev = NULL;
    
    if(head->pid == pid){
        head = temp->next;
        free(temp);
        return;
    }else{
        while((temp != NULL) && (temp->pid != pid)){
            prev = temp;
            temp = temp->next;
        }
    }
    
    if (temp == NULL){
        printf("The pid does not exist\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void print_map(void){
    node *iter = head;
    while(iter != NULL){
        printf("%d  ", iter->pid);
        iter = iter->next;
    }
}

int main(void){
    allocate_map();

    printf("Allocating PIDs: \n");
    for(int i = 0; i <= 25; ++i){
        allocate_pid();
    }
    print_map();

    printf("\nReleasing all the even PIDs: \n");
    for(int i = MIN; i <= MIN + 25; i += 2){
        release_pid(i);
    }

    print_map();
    printf("\nRe-allocating-PID:\n");
    for(int i = MIN; i <= MIN + 25; i += 2){
        allocate_pid();
    }

    print_map();
    printf("\n");
    return 0;
}
