#ifndef HOLE_H
#define HOLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h> 
#include <math.h>

#define MEMORYSIZE 128
#define PARSESIZE 256

typedef struct Process
{
	char processID;
	int processSize;
	int timesSwapped;
    int time; 
    int firstIndex;
    int endIndex;
    struct Process* next; 
}Process;


Process *createProcess(char IDChar, int memory, int swaps, int time, int start, int end);
Process *insertBack(Process *list, Process *process);
Process *insertProcess(Process *list, Process *node, int clock, int firstIndex, int endIndex);
void loadProcess(Process **inMem, Process **list, int memory[], double *processes, double *holes, double *cumulativeMem, int *numOfLoads, int i, int firstIndex, int clock);
void removeProcess(Process **inMem, Process **list, int memory[]);
void calculateMemusage(Process *list, int memoryArray[], int *numOfProcesses, int *numOfHoles, double *memusage);
void initializeMem(int memory[MEMORYSIZE]);
void initializeArray(char string[], int length);
void destroyList(Process *list);
Process* pop(Process** headRef);
void firstFit(Process *process);
void bestFit(Process *process);
void worseFit(Process *process);
void nextFit(Process *process);
Process *parseFile(char fileName[]);

#endif