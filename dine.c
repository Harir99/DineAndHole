
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "dine.h"

Chopstick **chopsticks;

void *philospher(void *num)
{
    Philosopher *self = (Philosopher *)num;
    int i = 0, left = 0, right = 0;
    int id = self->id;
    printf("Philosopher %d is thinking\n", id + 1);
    while (i < self->eatCount)
    {
        right = id;
        left = getleftChopstick(id,self->forks);

        // pick up chopsticks
        pthread_mutex_lock(&chopsticks[right]->mutex);
        pthread_mutex_lock(&chopsticks[left]->mutex);
        printf("Philosopher %d is eating.\n", id + 1);
        sleep(1);

        // release chopsticks
        pthread_mutex_unlock(&chopsticks[right]->mutex);
        pthread_mutex_unlock(&chopsticks[left]->mutex);
        printf("Philosopher %d is thinking\n", id + 1);
        sleep(1);
        i++;
    }
    return NULL;
}
int getleftChopstick(int id,int numOfPhilosophers)
{
    return (id + 1 == numOfPhilosophers) ? 0 : id + 1;
}
void initializeData(Philosopher **self, Chopstick ***chopsticks, int i, int numOfTimesToEat, int numOfPhilosophers)
{
    *self = malloc(sizeof(Philosopher));
    (*self)->id = i;
    (*self)->state = THINKING;
    (*self)->eatCount = numOfTimesToEat;
    (*self)->forks = numOfPhilosophers;
    (*chopsticks)[i] = malloc(sizeof(Chopstick));
    (*chopsticks)[i]->id = i;
}

int main(int argc, char *argv[])
{
    int numOfPhilosophers = atol(argv[1]);
    int numOfTimesToEat = atol(argv[2]);
    pthread_t thread_id[numOfPhilosophers];
    chopsticks = malloc(sizeof(Chopstick) * numOfPhilosophers);

    for (int i = 0; i < numOfPhilosophers; i++)
    {
        Philosopher *self;
        initializeData(&self, &chopsticks, i, numOfTimesToEat,numOfPhilosophers);
        pthread_mutex_init(&chopsticks[i]->mutex, NULL);
        pthread_create(&thread_id[i], NULL, philospher, (void *)self);
    }

    for (int i = 0; i < numOfPhilosophers; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    pthread_exit(NULL);
}