#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
   //integer arrays for x,y coordinates of a maxsize 30
   int x[30], y[30];
   int n=0; // variable to store points
   FILE *file = fopen("points.csv", "r"); 


   // pairing x and y in array till the last line
   while(fscanf(file, "%d, %d", &x[n],&y[n])== 2){
       n++; 
   }

   int i=0; 
   int totalDistance = 0; // will store total distance if , there is a path between points
   for(i=1;i<n;i++){
       // checking if there is the one x or y value of (i - 1)th and ith index is
       // same or not to verify if there is a path or not 
       if ((x[i] == x[i-1])|| (y[i] == y[i-1])) {
           // calculating the distance between the two points
           int P_dist = sqrt(pow((x[i] - x[i-1]), 2) + pow((y[i] - y[i-1]), 2));
           // print points, if path the path is on point 
           printf("%d, %d - %d, %d : %d\n", x[i-1], y[i-1], x[i], y[i], P_dist);
           totalDistance += P_dist; // add distance to total_distance
       }
   }
   printf("Total path: %d", totalDistance); // print the total distance
   return 0;
}