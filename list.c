#include "hole.h"

Process *createProcess(char IDChar, int memory, int swaps, int time, int start, int end)
{

    Process *current;
    //Malloc memory for the Process node
    current = malloc(sizeof(Process));

    //If the malloc fails, do not create the node
    if (current == NULL)
    {
        free(current);
        return NULL;
    }

    current->processID = IDChar;
    current->processSize = memory;
    current->timesSwapped = swaps;
    current->time = time;
    current->firstIndex = start;
    current->endIndex = end;
    current->next = NULL;

    return current;
}
Process *insertBack(Process *list, Process *process)
{
    //If no list exists
    if (list == NULL)
    {
        return process;
    }
    //Go to the end of the list
    else if (list->next != NULL)
    {
        insertBack(list->next, process);
    }
    //Add to the back of the list
    else
    {
        list->next = process;
    }
    return list;
}
Process *insertProcess(Process *list, Process *node, int clock, int firstIndex, int endIndex)
{
    node->time = clock;
    node->firstIndex = firstIndex;
    node->endIndex = endIndex;
    node->next = NULL;
    list = insertBack(list, node);
    return list;
}
//removes the front node, and returns the process
Process *pop(Process **headRef)
{
    // underflow condition
    if (*headRef == NULL)
    {
        return NULL;
    }

    Process *head = *headRef;
    (*headRef) = (*headRef)->next;
    return head;
}
void loadProcess(Process **inMem, Process **list, int memory[], double *processes, double *holes, double *cumulativeMem, int *numOfLoads, int i, int firstIndex, int clock)
{

    int numOfProcesses = 0;
    int numOfHoles = 0;
    double memusage = 0;

    //Pop process off the list
    Process *node = pop(list);

    //Insert process into the list of processes in memory
    *inMem = insertProcess(*inMem, node, clock, firstIndex, i);
    *numOfLoads = *numOfLoads + 1;

    //Mark which bytes are occupied
    for (int j = firstIndex; j <= i; j++)
    {
        memory[j] = 1;
    }
    //Determine stats for the newly loaded process
    calculateMemusage(*inMem, memory, &numOfProcesses, &numOfHoles, &memusage);

    *processes = *processes + numOfProcesses;
    *holes = *holes + numOfHoles;
    *cumulativeMem = *cumulativeMem + memusage;

    printf("%c loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative%%mem = %d\n", node->processID, numOfProcesses, numOfHoles,(int)memusage, (int)round(*cumulativeMem / *numOfLoads));
}

void removeProcess(Process **inMem, Process **list, int memory[])
{

    //Pop process off
    Process *process = pop(inMem);

    //Mark memory that used to be occupied as now free
    for (int z = process->firstIndex; z <= process->endIndex; z++)
    {
        memory[z] = 0;
    }

    //If the process has already been swapped 3 times do not re-add it to the list of processes waiting to go into memory
    if (process->timesSwapped != 2)
    {
        process->timesSwapped = process->timesSwapped + 1;
        process->next = NULL;
        *list = insertBack(*list, process);
    }
    else
    {
        free(process);
    }
}

void calculateMemusage(Process *list, int memoryArray[], int *numOfProcesses, int *numOfHoles, double *memusage)
{
    int isHole = 1;
    double memCount = 0;

    //Calculate the number of processes currently in memory
    while (list != NULL)
    {
        *numOfProcesses = *numOfProcesses + 1;
        list = list->next;
    }

    //Count number of holes and total memory usage
    int i = 0;
    while (i < MEMORYSIZE)
    {
        //Hole count
        if (memoryArray[i] == 0 && isHole == 1)
        {
            isHole = 0;
            *numOfHoles = *numOfHoles + 1;
        }
        //Memory count
        else if (memoryArray[i] == 1)
        {
            isHole = 1;
            memCount = memCount + 1;
        }
        i += 1;
    }

    //Determine memory util
    *memusage = roundf((memCount / MEMORYSIZE) * 100);
}

void initializeMem(int memory[MEMORYSIZE])
{
    for (int i = 0; i < MEMORYSIZE; i++)
    {
        memory[i] = 0;
    }
}
void initializeArray(char string[], int length)
{

    //Set entire array to null terminators
    for (int j = 0; j < length; j++)
    {

        string[j] = '\0';
    }
}
void destroyList(Process *list)
{

    //Go to the very end of the list, then start freeing the nodes
    if (list->next != NULL)
    {
        destroyList(list->next);
    }

    free(list);
}