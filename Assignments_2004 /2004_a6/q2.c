
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

//Initialize the variables
const int NORTH_FARMERS = 20;
const int SOUTH_FARMERS = 20;
const int MAX_CROSS_BRIDGE_TIME = 5;
//Create pthread class's objects
static pthread_mutex_t sMutex;
static pthread_cond_t sCond;
//Declare variabels
static int s_NORTH_FARMERS;
static int s_SOUTH_FARMERS;
static int s_NORTH_FARMERS_waiting;
static int s_SOUTH_FARMERS_waiting;

//Define enum type of the direction
enum Direction
{
NORTH,
SOUTH,
};
static enum Direction s_direction;
//Function prototypes
static int initSynch(pthread_mutex_t *pmutex, pthread_cond_t *pcond);
static void destroySynch(pthread_mutex_t *pmutex, pthread_cond_t *pcond);
static int createThreads(pthread_attr_t *attr,
pthread_t *northIDs,
pthread_t *southIDs,
int *numsNorth,
int *numsSouth,
const int NORTH_FARMERS,
const int SOUTH_FARMERS);
static void destroyThreads(pthread_t *northIDs,
pthread_t *southIDs,
int *numsNorth,
int *numsSouth,
const int NORTH_FARMERS,
const int SOUTH_FARMERS);
static void *northBoundRunner(void *param);
static void *southBoundRunner(void *param);
static void northBoundEnter(void);
static void northBoundExit(void);
static void southBoundEnter(void);
static void southBoundExit(void);

int main(void)
{
//Creating pthread_attr_t class object
pthread_attr_t p;

//Creating an array variables
pthread_t northIDs[NORTH_FARMERS];
pthread_t southIDs[SOUTH_FARMERS];
int numsNorth[NORTH_FARMERS];
int numsSouth[SOUTH_FARMERS];
// Initializing the synchronization variables
if (initSynch(&sMutex, &sCond) < 0)
  return 1;

// Initializing pthreads
if (pthread_attr_init(&p) != 0)
{
  fputs("pthread_attr_init didn't work\n",
   stderr);
  // Calling the method destroySynch
  destroySynch(&sMutex, &sCond);
  return 2;
}
//Random number generator
srand(time(NULL));

// No farmers are present on the bridge
s_NORTH_FARMERS = s_SOUTH_FARMERS = 0;
s_NORTH_FARMERS_waiting = s_SOUTH_FARMERS_waiting = 0;
// Setting the initial direction to north
s_direction = NORTH;
// Creating the threads
if (createThreads(&p,
  northIDs, southIDs,
  numsNorth, numsSouth,
  NORTH_FARMERS, SOUTH_FARMERS) < 0)
{
  fputs("Couldn't create threads\n", stderr);

  // De-initializing pthreads
  pthread_attr_destroy(&p);

  // Destroying the synchronization variables
  destroySynch(&sMutex, &sCond);
  return 3;
}
destroyThreads(northIDs, southIDs,
  numsNorth, numsSouth,
  NORTH_FARMERS, SOUTH_FARMERS);

// De-initializing pthreads
pthread_attr_destroy(&p);

// Destroying the synchronization variables
destroySynch(&sMutex, &sCond);
return 0;
}
//Defining method of initSynch
int initSynch(pthread_mutex_t *pmutex,
pthread_cond_t *pcond)
{
if (!pmutex || !pcond)
  return -1;

// Initializing the mutex
if (pthread_mutex_init(pmutex, NULL) != 0) {
  fputs("pthread_mutex_init didn't work\n",
   stderr);
  return -2;
}

// Initializing the condition variable
if (pthread_cond_init(pcond, NULL) != 0) {
  fputs("pthread_cond_init blew\n", stderr);

  // Destroying the mutex
  pthread_mutex_destroy(&sMutex);
  return -3;
}
return 0;
}
//Defining method of destroySynch
void destroySynch(pthread_mutex_t *pmutex,
pthread_cond_t *pcond)
{
if (pcond)
  pthread_cond_destroy(pcond);
if (pmutex)
  pthread_mutex_destroy(pmutex);
}
//Defining method of createThreads
int createThreads(pthread_attr_t *attr,
pthread_t *northIDs,
pthread_t *southIDs,
int *numsNorth,
int *numsSouth,
const int NORTH_FARMERS,
const int SOUTH_FARMERS)
    {
int index;
int ret;
if (!attr || !northIDs || !southIDs
  || !numsNorth || !numsSouth
  || NORTH_FARMERS < 0 || SOUTH_FARMERS < 0)
  return -1;
for (index = 0; index < NORTH_FARMERS
  || index < SOUTH_FARMERS; index++)
{
  if (index < NORTH_FARMERS)
  {
   numsNorth[index] = index + 1;
   ret = pthread_create(&northIDs[index],
    attr, northBoundRunner,
    numsNorth + index);
   if (ret != 0)
   {
    printf("Couldn't create north thread "
     "number %d\n", index + 1);
    northIDs[index] = -1;
   }
  }
  if (index < SOUTH_FARMERS)
  {
   numsSouth[index] = index + 1;
   ret = pthread_create(southIDs + index,
    attr, southBoundRunner,
    numsSouth + index);
   if (ret != 0)
   {
    printf("Couldn't create south thread "
     "number %d\n", index + 1);
    southIDs[index] = -1;
   }
  }
}
return 0;
}
//Defining method of destroyThreads
void destroyThreads(pthread_t *northIDs,
pthread_t *southIDs,
int *numsNorth,
int *numsSouth,
const int NORTH_FARMERS,
const int SOUTH_FARMERS)
{
int index;
int ret;
if (!northIDs || !southIDs
  || !numsNorth || !numsSouth)
  return;
for (index = 0; index < NORTH_FARMERS
  || index < SOUTH_FARMERS; index++)
{
  if (index < NORTH_FARMERS && northIDs[index] >= 0)
  {
   ret = pthread_join(northIDs[index], NULL);
   if (ret != 0)
    printf("pthread_join didn't work for "
     "north thread num %2d\n",
     numsNorth[index]);
  }
  if (index < SOUTH_FARMERS && southIDs[index] >= 0)
  {
   ret = pthread_join(southIDs[index], NULL);
   if (ret != 0)
    printf("pthread_join didn't work for "
     "south thread num %2d\n",
     numsSouth[index]);
  }
}
}
//Method definition of *northBoundRunner
void *northBoundRunner(void *param)
{
int num = *((__typeof__(&num)) param);
printf("Farmer #%2d from North Tunbridge "
  "waiting...\n", num);
northBoundEnter();
printf("Farmer #%2d from North Tunbridge "
  "crossing the bridge...\n", num);
sleep(rand() % MAX_CROSS_BRIDGE_TIME + 1);
northBoundExit();
printf("Farmer #%2d from North Tunbridge "
  "crossed the bridge\n", num);
pthread_exit(0);
}
//Method definition of southBoundRunner
void *southBoundRunner(void *param)
{
int num = *((__typeof__(&num)) param);
printf("Farmer #%2d from South Tunbridge "
  "waiting...\n", num);
southBoundEnter();
printf("Farmer #%2d from South Tunbridge "
  "crossing the bridge...\n", num);
sleep(rand() % MAX_CROSS_BRIDGE_TIME + 1);
southBoundExit();
printf("Farmer #%2d from South Tunbridge "
  "crossed the bridge\n", num);
pthread_exit(0);
}
//Method definition of northBoundEnter
void northBoundEnter(void)
{
pthread_mutex_lock(&sMutex);
s_NORTH_FARMERS_waiting++;
while (s_SOUTH_FARMERS > 0
  || (s_SOUTH_FARMERS_waiting > 0
   && s_direction == SOUTH))
  pthread_cond_wait(&sCond, &sMutex);
s_direction = NORTH;
s_NORTH_FARMERS_waiting--;
s_NORTH_FARMERS++;
pthread_mutex_unlock(&sMutex);
}
//Defining method of northBoundExit
void northBoundExit(void)
{
pthread_mutex_lock(&sMutex);
s_NORTH_FARMERS--;
s_direction = SOUTH;
if (s_NORTH_FARMERS == 0)
  pthread_cond_broadcast(&sCond);
pthread_mutex_unlock(&sMutex);
}
//Defining method of southBoundEnter
void southBoundEnter(void)
{
pthread_mutex_lock(&sMutex);
s_SOUTH_FARMERS_waiting++;
while (s_NORTH_FARMERS > 0
  || (s_NORTH_FARMERS_waiting > 0
   && s_direction == NORTH))
  pthread_cond_wait(&sCond, &sMutex);
s_direction = SOUTH;
s_SOUTH_FARMERS_waiting--;
s_SOUTH_FARMERS++;
pthread_mutex_unlock(&sMutex);
}
//Defining method of southBoundExit
void southBoundExit(void)
{
pthread_mutex_lock(&sMutex);
s_SOUTH_FARMERS--;
s_direction = NORTH;
if (s_SOUTH_FARMERS == 0)
  pthread_cond_broadcast(&sCond);
pthread_mutex_unlock(&sMutex);
}