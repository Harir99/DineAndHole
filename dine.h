#ifndef DINE_H
#define DINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h> 

typedef enum { THINKING, HUNGRY, EATING } State;

typedef struct Chopstick {
    int id;
    pthread_mutex_t mutex;
}Chopstick;

typedef struct Philosopher {
    int id;
    State state;
    int eatCount;
    int forks;
}Philosopher;


void* philospher(void* num);
int getleftChopstick(int id,int numOfPhilosophers);
void initializeData(Philosopher** self,Chopstick*** chopsticks,int i, int numOfTimesToEatm,int numOfPhilosophers);

#endif // !DINE_H
