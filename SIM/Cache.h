//
//  Header file to contain all structs and prototypes necessary for program execution
// Cassidy Lyons
// Ca 264412



#ifndef Cache_h
#define Cache_h
// First in first out struct
typedef struct FIFO
{
    // To hold 64 bit memory address
    unsigned long long **cacheArray;
    // Long double to maintain precision wehn calculating miss ratio
    long double read, readMiss;
    // Long double to maintain precision wehn calculating miss ratio
    long double write, writeMiss;
    // To identify how much memory to allocate for pointers in 2D array
    int sets;
    // To identify how many blocks to allocate for each set
    int assoc;
    
}FIFO;




typedef struct LRU
{
    // To hold 64 bit memory address
    unsigned long long **cacheArray;
    // Long double to maintain precision wehn calculating miss ratio
    long double read, readMiss;
    // Long double to maintain precision wehn calculating miss ratio
    long double write, writeMiss;
    // To identify how much memory to allocate for pointers in 2D array
    int sets;
    // To identify how many blocks to allocate for each set
    int assoc;

    
}LRU;

int processLRUCache(char *filename, int cacheSize, int k_way);

int processFIFOCache(char *filename, int cacheSize, int k_way);

LRU *createLRU(int set, int assoc);

FIFO *createFIFO(int set, int assoc);

LRU *LRU_Cache(LRU *lru, unsigned long long tag, int setIndex, int k_way);

FIFO *FIFO_Cache(FIFO *fifo,unsigned long long tag, int setIndex, int k_way);

void getRatio(LRU *lru);

void missRatio(FIFO *fifo);

void destroyFIFO(FIFO *fifo);

void destroyLRU(LRU *lru);



#endif
 
