
//  Created by Cassidy Lyons on 10/22/17.
//  Copyright © 2017 Cassidy Lyons. All rights reserved.

#include "Cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Global to keep track of Reads VS. Writes
int typ;



int main(int argc, char **argv)
{
    int cacheSize = atoi(argv[1]),;
    int k_way = atoi(argv[2]);
    if(atoi(argv[3]) == 0)
        processLRUCache(argv[4], cacheSize, k_way);
    if(atoi(argv[3]) == 1)
        processFIFOCache(argv[4], cacheSize, k_way);

    return 0;
}
// Calculate all fields and then
// Open file and pass each address to LRU cache to process
int processLRUCache(char *filename, int cacheSize, int k_way)
{
    int blocks, blockSize = 64, setIndex, sets;
    errno = 0;
    char* end = 0, asString[25], opp[1];
    unsigned long long tagBits, res;

    blocks = cacheSize/blockSize;
    sets = blocks/k_way;

    LRU *lru = createLRU(sets, k_way);


    FILE *ifp = fopen(filename,"r");
    if(ifp == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }


    while(!feof(ifp))
    {

        fscanf(ifp,"%s %s\n", opp, asString);
        if(opp[0] == 82) //always will read R or W.. so if it's not 'R', its W
        {
            typ = 0;  //global  ..(typ == 0)? R : W
        }
        else
        {
            typ = 1;
        }
        res = strtoull(asString, &end, 16);//64bit data type holds built in funct that converts hex to decimal
        tagBits = (res/64)/sets;
        setIndex = (res/64)%sets;
        lru = LRU_Cache(lru, tagBits, setIndex, k_way);


    }
    fclose(ifp);

    // After Processing file, go calculate miss ratios
    getRatio(lru);



    return 0;
}
// Calculate address fields and then pass each address to FIFO Cache
int processFIFOCache(char *filename, int cacheSize, int k_way)
{
    int blocks, blockSize = 64, setIndex, sets;
    errno = 0;
    char* end = 0, asString[25], opp[1];
    unsigned long long tagBits, res;

    blocks = cacheSize/blockSize;
    sets = blocks/k_way;
    FIFO *fifo = createFIFO(sets, k_way);


    FILE *ifp = fopen(filename,"r");
    if(ifp == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }



    while(!feof(ifp))
    {

        fscanf(ifp,"%s %s\n", opp, asString);
        if(opp[0] == 82) //always will read R or W.. so if it's not 'R', its W
        {
            typ = 0;  //global  ..(typ == 0)? R : W
        }
        else
        {
            typ = 1;
        }
        res = strtoull(asString, &end, 16);//64bit data type holds built in funct that converts hex to decimal
        tagBits = (res/64)/sets;
        setIndex = (res/64)%sets;
        fifo = FIFO_Cache(fifo, tagBits, setIndex, k_way);


    }
    fclose(ifp);



    missRatio(fifo);



    return 0;
}
// Dynamically allocates and initializes pointer and fields for LRU struct
// according to sets previusly calculated and associativity that is passed in as argv[2]
LRU *createLRU(int sets,int k_way)
{
    int i;
    LRU *lru = malloc(sizeof(LRU) * 1);
    // dynamically allocate 'sets'- amount of unsigned long long pointer
    // data type to hold arrays containing 64 bit address
    lru->cacheArray = malloc(sizeof(unsigned long long*) * sets);


    for(i = 0; i < sets; i++)
    {

        lru->cacheArray[i] = malloc(sizeof(unsigned long long) * k_way);

    }
    lru->readMiss = 0;
    lru->writeMiss = 0;
    lru->read = 0;
    lru->write = 0;
    lru->sets = sets;
    lru->assoc = k_way;

    return lru;
}
// LRU cache that updates index 0 of each to keep track of least recently used
// and index (k_way - 1) keeps track of MRU block in each set
LRU *LRU_Cache(LRU *lru, unsigned long long tag, int setIndex, int k_way)
{
    int i, j = 0;
    if(typ == 0)
        lru->read++;
    else
        lru->write++;


    for(i = 0; i < k_way; i++)
    {

        if(lru->cacheArray[setIndex][i] == tag)
        {
            // If it's a hit, just move tag to MRU positon, no evictions
            for(j = i; j < k_way - 1; j++)
            {
                lru->cacheArray[setIndex][j] = lru->cacheArray[setIndex][j + 1];
            }
            lru->cacheArray[setIndex][k_way - 1] = tag;
            // break out so that address is only entered once upon access
            break;
        }
        // if i == k_way - 1, it has searched the entire set without a hit
        else if(i == (k_way - 1) && lru->cacheArray[setIndex][i] != tag)
        {
            // Evicts LRU, adds new tag to MRU postion
            for(j = 0; j < (k_way - 1); j++)
            {
                lru->cacheArray[setIndex][j] = lru->cacheArray[setIndex][j + 1];
            }
            lru->cacheArray[setIndex][k_way - 1] = tag;

            if(typ == 0)
                lru->readMiss++;
            else
                lru->writeMiss++;


        }
    }



    return lru;
}
// Dynamically allocates and initializes pointer and fields for FIFO struct
// according to sets previusly calculated and associativity that is passed in as argv[2]
FIFO *createFIFO(int sets, int k_way)
{
    int i;
    FIFO *fifo = malloc(sizeof(FIFO) * 1);
    fifo->cacheArray = malloc(sizeof(unsigned long long*) * sets);

    for(i = 0; i < sets; i++)
    {

        fifo->cacheArray[i] = malloc(sizeof(unsigned long long) * k_way);

    }

    fifo->readMiss = 0;
    fifo->writeMiss = 0;
    fifo->read = 0;
    fifo->write = 0;
    fifo->sets = sets;
    fifo->assoc = k_way;

    return fifo;
}
// Prosesses hits and misses using first in first out replacement, if Tag is found do nothing, count as hit. if not found,
// insert at MRU and evict first address that was added
FIFO *FIFO_Cache(FIFO *fifo, unsigned long long tag, int setIndex, int k_way)
{
    int i, j = 0;
    if(typ == 0)
        fifo->read++;
    else
        fifo->write++;


    for(i = 0; i < k_way; i++)
    {
        // No update if hit
        if(fifo->cacheArray[setIndex][i] == tag)
        {

            break;
        }
        // if i == k_way - 1, it has searched the entire set without a hit, evict first in, add to position (k_way - 1)
        else if(i == (k_way - 1) && fifo->cacheArray[setIndex][i] != tag)
        {
            for(j = 0; j < (k_way - 1); j++)
            {
                fifo->cacheArray[setIndex][j] = fifo->cacheArray[setIndex][j + 1];
            }
            fifo->cacheArray[setIndex][k_way - 1] = tag;

            if(typ == 0)
                fifo->readMiss++;
            else
                fifo->writeMiss++;


        }
    }


    return fifo;
}


// Calculates ratio of LRU based on values that have been updated throughout program
void getRatio(LRU *lru)
{
    printf("\nWrite Miss Ratio: %Lf\n",(lru->writeMiss/lru->write));

    printf("\nRead Miss Ratio: %Lf\n",(lru->readMiss/lru->read));

    printf("\nTotal Miss Ratio: %Lf\n",(lru->writeMiss + lru->readMiss)/(lru->write + lru->read));
    // Final step is to free memory
     destroyLRU(lru);

    return;
}
// Calculates ratio of FIFO based on values that have been updated throughout program
void missRatio(FIFO *fifo)
{
    printf("\nWrite Miss Ratio: %Lf\n",(fifo->writeMiss/fifo->write));

    printf("\nRead Miss Ratio: %Lf\n",(fifo->readMiss/fifo->read));

    printf("\nTotal Miss Ratio: %Lf\n",(fifo->writeMiss + fifo->readMiss)/(fifo->write + fifo->read));
    // Free memory
    destroyFIFO(fifo);

    return;
}
// Destroy(free) all memory that has been allocated
void destroyFIFO(FIFO * fifo)
{
    int set = fifo->sets,i;
    for(i = 0; i < set; i++)
    {
        // Defensive check against freeing already freed memory
        if(fifo->cacheArray[i] != NULL)
        free(fifo->cacheArray[i]);
        if(fifo->cacheArray[i] == NULL)
            return;

    }
    // Defensive check against freeing already freed memory
    if(fifo->cacheArray != NULL)
        free(fifo->cacheArray);
    if(fifo != NULL)
        free(fifo);
    return;
}
// Destroy(free) all memory that has been allocated
void destroyLRU(LRU *lru)
{
    int set = lru->sets,i;

    for(i = 0; i < set ; i++)
    {
        // Defensive check against freeing already freed memory
        if(lru->cacheArray[i] != NULL)
        free(lru->cacheArray[i]);
        if(lru->cacheArray[i] == NULL)
            return;

    }
    // Defensive check against freeing already freed memory
    if(lru != NULL)
        free(lru->cacheArray);
    if(lru != NULL)
        free(lru);



    return;
}
