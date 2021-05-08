#include "hole.h"

int main(int argc, char *argv[])
{
   if(argc != 3) {
    	printf("correct format : ./hole filename scheduleType\n");
    	return 1;
    }
    Process *head = parseFile(argv[1]);

    if (strcmp(argv[2], "first") == 0)
        firstFit(head);
    if (strcmp(argv[2], "best") == 0)
        bestFit(head);
    if (strcmp(argv[2], "worse") == 0)
        worseFit(head);
    if (strcmp(argv[2], "next") == 0)
        nextFit(head);
}
void firstFit(Process *process)
{
    int memory[MEMORYSIZE];
    initializeMem(memory);

    Process *inMem = NULL;
    int clock = 0, numOfLoads = 0;
    double processes = 0, holes = 0, cumulativeMem = 0;

    while (process != NULL)
    {
        int firstIndex = -1, holeSize = 0;
        int noHole = 0;
        int i = 0;
        while (i < MEMORYSIZE)
        {
            if (memory[i] == 0)
            {
                //If this is the first free byte in the current hole, save its index position
                if (firstIndex == -1)
                    firstIndex = i;
                holeSize += 1;

                //If there is enough room to add the next process, add it to memory
                if (holeSize >= process->processSize)
                {
                    loadProcess(&inMem, &process, memory, &processes, &holes, &cumulativeMem, &numOfLoads, i, firstIndex, clock);
                    noHole = 1;
                    break;
                }
            }
            else
            {
                holeSize = 0;
                firstIndex = -1;
            }
            i += 1;
        }
        //If there is no space to add the next process, remove the oldest process currently in memory
        if (noHole == 0)
        {
            removeProcess(&inMem, &process, memory);
        }
        clock += 1;
    }
    processes = processes / numOfLoads;
    holes = holes / numOfLoads;

    printf("Total loads = %d, average #processes = %.2lf, average #holes = %.1lf, cumulative mem = %d\n", numOfLoads, processes, holes, (int)cumulativeMem / numOfLoads);

    //Free all processes that are still in memory before exiting the program
    if (inMem != NULL)
    {

        destroyList(inMem);
    }
}
void bestFit(Process *process)
{
    int memory[MEMORYSIZE];
    initializeMem(memory);

    Process *inMem = NULL;
    int clock = 0, numOfLoads = 0;
    double processes = 0, holes = 0, cumulativeMem = 0;

    while (process != NULL)
    {
        int firstIndex = 0, holeSize = 0, smallestHole = 200, smallestFirstIndex = 0, lastIndex = 0;
        int noHole = 0;
        int i = 0;
        while (i < MEMORYSIZE)
        {
            //If this is the first free byte in the current hole and we are not at the last byte of memory, save its index position
            if (memory[i] == 0 && i != MEMORYSIZE - 1)
            {
                if (firstIndex == -1)
                    firstIndex = i;
                holeSize += 1;
            }
            //Check if we can add the process in the memory that has already been "scanned"
            else
            {
                if (memory[i] == 0 && i == MEMORYSIZE - 1)
                    holeSize += 1;

                //If the open hole is the smallest hole we have found, save the starting index position
                if (holeSize < smallestHole && holeSize >= process->processSize)
                {
                    smallestHole = holeSize;
                    noHole = 1;
                    smallestFirstIndex = firstIndex;
                }
                holeSize = 0;
                firstIndex = -1;
            }
            i += 1;
        }
        //If there is no space to add the next process, remove the oldest process currently in memory
        if (noHole == 0)
        {
            removeProcess(&inMem, &process, memory);
        }
        //Add the process to memory
        else
        {
            lastIndex = smallestFirstIndex + process->processSize - 1;
            loadProcess(&inMem, &process, memory, &processes, &holes, &cumulativeMem, &numOfLoads, lastIndex, smallestFirstIndex, clock);
            noHole = 0;
        }
        clock += 1;
    }
    processes = processes / numOfLoads;
    holes = holes / numOfLoads;

    printf("Total loads = %d, average #processes = %.2lf, average #holes = %.1lf, cumulative mem = %d\n", numOfLoads, processes, holes,(int) cumulativeMem / numOfLoads);

    //Free all processes that are still in memory before exiting the program
    if (inMem != NULL)
    {

        destroyList(inMem);
    }
}
void worseFit(Process *process)
{
    int memory[MEMORYSIZE];
    initializeMem(memory);

    Process *inMem = NULL;
    int clock = 0, numOfLoads = 0;
    double processes = 0, holes = 0, cumulativeMem = 0;

    while (process != NULL)
    {
        int firstIndex = 0, holeSize = 0, biggestHole = -200, biggestFirstIndex = 0, lastIndex = 0;
        int noHole = 0;
        int i = 0;
        while (i < MEMORYSIZE)
        {
            //If this is the first free byte in the current hole and we are not at the last byte of memory, save its index position
            if (memory[i] == 0 && i != MEMORYSIZE - 1)
            {
                if (firstIndex == -1)
                    firstIndex = i;
                holeSize += 1;
            }
            //Check if we can add the process in the memory that has already been "scanned"
            else
            {
                if (memory[i] == 0 && i == MEMORYSIZE - 1)
                    holeSize += 1;

                //If the open hole is the smallest hole we have found, save the starting index position
                if (holeSize > biggestHole && holeSize >= process->processSize)
                {
                    biggestHole = holeSize;
                    noHole = 1;
                    biggestFirstIndex = firstIndex;
                }
                holeSize = 0;
                firstIndex = -1;
            }
            i += 1;
        }
        //If there is no space to add the next process, remove the oldest process currently in memory
        if (noHole == 0)
        {
            removeProcess(&inMem, &process, memory);
        }
        //Add the process to memory
        else
        {
            lastIndex = biggestFirstIndex + process->processSize - 1;
            loadProcess(&inMem, &process, memory, &processes, &holes, &cumulativeMem, &numOfLoads, lastIndex, biggestFirstIndex, clock);
            noHole = 0;
        }
        clock += 1;
    }
    processes = processes / numOfLoads;
    holes = holes / numOfLoads;

    printf("Total loads = %d, average #processes = %.2lf, average #holes = %.1lf, cumulative mem = %d\n", numOfLoads, processes, holes, (int)cumulativeMem / numOfLoads);

    //Free all processes that are still in memory before exiting the program
    if (inMem != NULL)
    {

        destroyList(inMem);
    }
}
void nextFit(Process *process)
{
    int memory[MEMORYSIZE];
    initializeMem(memory);

    Process *inMem = NULL;
    int clock = 0, numOfLoads = 0;
    double processes = 0, holes = 0, cumulativeMem = 0;

    while (process != NULL)
    {
        int firstIndex = -1, holeSize = 0, lastIndex = 0;
        int noHole = 0;
        int oneLoop = 0;
        int i = lastIndex;
        while (i < MEMORYSIZE)
        {
            if (memory[i] == 0)
            {
                //If this is the first free byte in the current hole, save its index position
                if (firstIndex == -1)
                    firstIndex = i;
                holeSize += 1;

                //If there is enough room to add the next process, add it to memory
                if (holeSize >= process->processSize)
                {
                    loadProcess(&inMem, &process, memory, &processes, &holes, &cumulativeMem, &numOfLoads, i, firstIndex, clock);
                    noHole = 1;
                    lastIndex = i;
                    break;
                }
            }
            else
            {
                holeSize = 0;
                firstIndex = -1;
            }
            if (i == MEMORYSIZE - 1 && noHole == 0 && oneLoop == 0)
            {
                i = 0;
                firstIndex = -1;
                holeSize = 0;
                oneLoop = 1;
            }
            i += 1;
        }
        //If there is no space to add the next process, remove the oldest process currently in memory
        if (noHole == 0)
        {
            removeProcess(&inMem, &process, memory);
        }
        clock += 1;
    }
    processes = processes / numOfLoads;
    holes = holes / numOfLoads;

    printf("Total loads = %d, average #processes = %.2lf, average #holes = %.1lf, cumulative mem = %d\n", numOfLoads, processes, holes, (int)cumulativeMem / numOfLoads);

    //Free all processes that are still in memory before exiting the program
    if (inMem != NULL)
    {

        destroyList(inMem);
    }
}

Process *parseFile(char fileName[])
{

    FILE *fp = NULL;
    Process *list = NULL;
    char input[PARSESIZE];
    char memory[PARSESIZE];
    int counter = 0;

    initializeArray(input, PARSESIZE);
    initializeArray(memory, PARSESIZE);

    fp = fopen(fileName, "r");
    if(fp == NULL){
    	printf("Error - file does not exist\n");
    	exit(0);
    } 

    //Read in each line of the file
    while (fgets(input, PARSESIZE, fp) != NULL)
    {

        Process *process = NULL;

        //Memory values are all characters after the second character in each line
        for (int i = 2; i < strlen(input); i++)
        {
            memory[counter] = input[i];
            counter = counter + 1;
        }

        //Create a process and add it to the end of the list
        process = createProcess(input[0], atoi(memory), 0, 0, 0, 0);
        list = insertBack(list, process);

        initializeArray(memory, PARSESIZE);
        counter = 0;
    }

    fclose(fp);
    return list;
}
