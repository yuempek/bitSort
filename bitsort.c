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
int nodeCount = 0;

double duration_nextFreeIndex = 0;
	

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
    if((freeIdx + 2) < maxIdx) {
        freeIdx = freeIdx + 2;
    } else {
        int ** newBuffer = (int **) malloc(BUFFER_LENGTH * sizeof(int*));
        int i;

        freeIdx = newBuffer;
        maxIdx = newBuffer + BUFFER_LENGTH;
    }
    
    freeIdx[0] = 0;
    freeIdx[1] = 0;
    
    nodeCount++;

    return freeIdx;
} 
/*
Bit sort is a new sorting algorithm that uses binary tree to sort elements. Numbers are put in a binary tree. When reading they sorted already. Complexity is O(nk). k is bit size of the number. BitSort is not recursive algorithm when sort the values. But reading the tree is recursive.

the full size of binary tree:
                        ______________________x______________________                          
                      0/                                             \1                        
            __________x__________                           __________x__________              
          0/                     \1                       0/                     \1            
      ____x____               ____x____               ____x____               ____x____        
    0/         \1           0/         \1           0/         \1           0/         \1      
   _x_         _x_         _x_         _x_         _x_         _x_         _x_         _x_     
 0/   \1     0/   \1     0/   \1     0/   \1     0/   \1     0/   \1     0/   \1     0/   \1   
cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value 
  
Example:
if 
array = {};

*/
void bitSort(int * array, int arraySize) {
	int i, j;
	int ** addr;
    clock_t start, end;
  
	root = (int**) nextFreeIndex();

    for (i = 0; i < arraySize; i++) {
            
        
        int ** activeNode = root;
    
        register int value = array[i];
        register int bit;

        for (j = INT_SIZE; j; j--){
                
            bit = (0x80000000 & value) >> 31;
            value <<= 1;
            
            addr = (int **) activeNode[bit];
            
            if (addr == 0) {
                addr = nextFreeIndex();
                activeNode[bit] = (int *) addr;
            }

            activeNode = (int **) addr;
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


//start = clock();
//duration_nextFreeIndex += ((double) (clock() - start)) / CLOCKS_PER_SEC * 1000000;

int main() {
    int *** leafAdresses;
	int i, count;
    int duration1, duration2;
	clock_t start, end;
    double cpu_time_used;
     

    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) 
        exampleArray[i] = randomd() * 0x7FFFFFFF;
   
    printf("Begin \n");
   
    start = clock();
	    bitSort(exampleArray, EXAMPLE_ARRAY_LENGHT);
	end = clock();
    duration1 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

    start = clock();
		leafAdresses = getSortedValueAddresses();
	end = clock();
    duration2 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("leafCount : %d\n", leafCount);
	printf("nodeCount : %d\n", nodeCount);
	printf("duration sort : %d us\n", duration1);
    printf("duration read : %d us\n", duration2);
    printf("duration nextFreeIndex : %d us\n", (int)duration_nextFreeIndex);
   
  /*
  for(i = 0; i < leafCount; i++){
		count = (int) leafAdresses[i][COUNT_INDEX];
		while(count--)
			printf("%d-", *(leafAdresses[i][VALUE_INDEX]));
	}
  */
  
  printf("End");
}
