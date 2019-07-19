#include <stdio.h>
#include <stdbool.h>

#define BUFFER_LENGTH 10000
#define EXAMPLE_ARRAY_LENGHT 10
#define INT_SIZE 32

#define COUNT_INDEX 0
#define VALUE_INDEX 1

                
int exampleArray[EXAMPLE_ARRAY_LENGHT] = {0};

void * freeIdx = 0;
void * maxIdx = 0;
void ** root;

int leafCount = 0;

int ** sortedArrayValueIndexes;
int indx = 0;

void recurse(void ** currentNode, int digit) {
    if(digit == 0) { //this is leaf
        sortedArrayValueIndexes[indx] = (int *)currentNode[VALUE_INDEX];
        indx++;
        return;
    }
    
    if (currentNode[0]) recurse(currentNode[0], digit-1);
    if (currentNode[1]) recurse(currentNode[1], digit-1);  
}

int ** getSortedValueAddresses(){
    sortedArrayValueIndexes = (int **) malloc(leafCount * sizeof(int*));
    // todo recursive binary tree search for all leaf
    recurse(root, INT_SIZE);
    return sortedArrayValueIndexes;
}
            

void * nextFreeIndex(){
    if((freeIdx + 2*sizeof(void *)) > maxIdx) {
        
        void ** newBuffer = (void **) malloc(BUFFER_LENGTH * 2 * sizeof(void*));
        
        for (int i = 0; i < BUFFER_LENGTH; i++) 
            newBuffer[BUFFER_LENGTH] = 0;
        
        freeIdx = newBuffer;
        maxIdx = newBuffer + BUFFER_LENGTH;
    } else {
        freeIdx += 2*sizeof(void *);
    }
    
    return freeIdx;
} 

void bitSort(int * array, int arraySize) {
    root = nextFreeIndex();

    for (int i = 0; i < arraySize; i++) {
        void ** activeNode = root;
        int value = array[i];
    
        int digit = 1 << (INT_SIZE-1);
        int bit;

        for (int j = INT_SIZE; j > 0 && digit != 0; j--){
            bit = (digit & value) ? 1 : 0;
            
            void * addr = activeNode[bit];
            
            if (addr == 0) {
                addr = nextFreeIndex();
                activeNode[bit] = addr;
            }
            
            activeNode = addr;
            digit = digit >> 1;
        }
        
        if (activeNode == 0) {
            activeNode[COUNT_INDEX] = (void *) 1;
            activeNode[VALUE_INDEX] = array + i;
            leafCount++;
        }
        else{
            activeNode[COUNT_INDEX]++;
        }      
    }
}

int main() {
   for (int i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) exampleArray[i] = i;
   
   printf("Begin \n");
   
   bitSort(exampleArray, EXAMPLE_ARRAY_LENGHT);
   
   int ** valueadresses = getSortedValueAddresses();

   printf("leafCount : %d \n", leafCount);
   
   for(int i = 0; i < leafCount; i++)
      printf("%d-", *(valueadresses[i]));
   
   printf("End");
}


