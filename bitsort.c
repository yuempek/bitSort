#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "randomd.h"
     

#define BUFFER_LENGTH 1000000
#define EXAMPLE_ARRAY_LENGHT 1000000
#define INT_SIZE 32

#define COUNT_INDEX 0
#define VALUE_INDEX 1

                
int exampleArray[EXAMPLE_ARRAY_LENGHT] = {0};

int ** freeIdx = 0;
int ** maxIdx = 0;
int ** root;

int leafCount = 0;

int *** sortedArrayValueIndexes;
int indx = 0;

void recurse(int ** currentNode, int digit) {
    if(digit == 0) { //this is leaf
        sortedArrayValueIndexes[indx] = currentNode;
        indx++;
        return;
    }
    
    if (currentNode[0]) recurse((int **) currentNode[0], digit-1);
    if (currentNode[1]) recurse((int **) currentNode[1], digit-1);  
}

int *** getSortedValueAddresses(){
    sortedArrayValueIndexes = (int ***) malloc(leafCount * sizeof(int**));
    // todo recursive binary tree search for all leaf
    recurse(root, INT_SIZE);
    return sortedArrayValueIndexes;
}
            

int ** nextFreeIndex(){
    if((freeIdx + 2) >= maxIdx) {
        
        int ** newBuffer = (int **) malloc(BUFFER_LENGTH * sizeof(int*));
        int i;
        for (i = 0; i < BUFFER_LENGTH; i++) 
            newBuffer[i] = 0;
        
        freeIdx = newBuffer;
        maxIdx = newBuffer + BUFFER_LENGTH;
    } else {
        freeIdx = freeIdx + 2;
    }
    
    return freeIdx;
} 

void bitSort(int * array, int arraySize) {
    int i, j;
    int ** addr;

    root = (int**) nextFreeIndex();

    for (i = 0; i < arraySize; i++) {
        int ** activeNode = root;
        int value = array[i];
    
        unsigned int digit = 1 << (INT_SIZE-1);
        int bit;

        for (j = INT_SIZE; j > 0 && digit != 0; j--){
            bit = (digit & value) ? 1 : 0;
            
            addr = (int **) activeNode[bit];
            
            if (addr == 0) {
                addr = nextFreeIndex();
                activeNode[bit] = (int *) addr;
            }
            
            activeNode = (int **) addr;
            digit = digit >> 1;
        }
        
        if (*activeNode == 0) {
            activeNode[COUNT_INDEX] = 1;
            activeNode[VALUE_INDEX] = array + i;
            leafCount++;
        }
        else{
            activeNode[COUNT_INDEX] = ((int)activeNode[COUNT_INDEX]) + 1;
        }      
    }
}

int main() {
  int *** leafAdresses;
  int i, count;

  clock_t start, end;
  double cpu_time_used;

  for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) 
      exampleArray[i] = randomd() * 0x7FFFFFFF;
   
  printf("Begin \n");
   
  start = clock();
    bitSort(exampleArray, EXAMPLE_ARRAY_LENGHT);
    leafAdresses = getSortedValueAddresses();
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("leafCount : %d\n", leafCount);
  printf("duration  : %d ms\n", (int)(cpu_time_used * 1000));
   
  /*
  for(i = 0; i < leafCount; i++){
        count = (int) leafAdresses[i][COUNT_INDEX];
        while(count--)
            printf("%d-", *(leafAdresses[i][VALUE_INDEX]));
    }
  */
  
  printf("End");
}
