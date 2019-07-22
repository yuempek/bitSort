#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "randomd.h"
     

#define BUFFER_LENGTH 1000000
#define EXAMPLE_ARRAY_LENGHT 1000
#define INT_SIZE 32

#define COUNT_INDEX 0
#define VALUE_INDEX 1

                
int exampleArray[EXAMPLE_ARRAY_LENGHT] = {0};

int ** freeIdx = 0;
int ** maxIdx = 0;
int ** root;

int leafCount = 0;
int nodeCount = 0;


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
# BitSort

Bit sort is a new sorting algorithm that uses binary tree to sort elements. Numbers are put in a binary tree. When reading they sorted already. Complexity is **O(nk)**. **k** is bit size of the number. BitSort is not recursive algorithm when sorting the values. But reading the tree is recursive.

the full size of binary tree for 3bit values:

                            ______________________|______________________                          
                           0                                             1                         
                __________/__________                           __________\__________              
               0                     1                         0                     1             
          ____/____               ____\____               ____/____               ____\____        
         0         1             0         1             0         1             0         1       
       _/_         _\_         _/_         _\_         _/_         _\_         _/_         _\_     
      /   \       /   \       /   \       /   \       /   \       /   \       /   \       /   \    
    cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value  cnt  value 
  
Example:
We suppose that we have 3 bit length numbers.

    array = {7, 3, 2, 5, 0, 7, 3, 2, 7};
    
    L   : level
    msb : most significant bit
    lsb : least significant bit
    
                  msb       lsb
                   L1   L2   L3
    7 = 111  -->    1    1    1
    3 = 011  -->    0    1    1
    2 = 010  -->    0    1    0
    5 = 101  -->    1    0    1
    0 = 000  -->    0    0    0
    7 = 111  -->    1    1    1
    3 = 011  -->    0    1    1
    2 = 010  -->    0    1    0
    7 = 111  -->    1    1    1

firstly binary tree has only root node.
    
                            ______________________|______________________                          
                           0                                             1                         


first number is added to binary tree using own bits from msb to lsb. 
(adding number: 7)


                                ______________________|______________________                          
    L1  -------------------->  0                                             1                         
                                                                    __________\__________              
    L2  -------------------------------------------------------->  0                     1             
                                                                                      ____\____        
    L3  ------------------------------------------------------------------------->   0         1       
                                                                                               _\_     
                                                                                              /   \    
                                                                                             1     7 

Then others sequently.
(adding numbers: 3, 2, 5, 0)


                            ______________________|______________________                          
                           0                                             1                         
                __________/__________                           __________\__________              
               0                     1                         0                     1             
          ____/____               ____\____               ____/____               ____\____        
         0         1             0         1             0         1             0         1       
       _/_                     _/_         _\_                     _\_                     _\_     
      /   \                   /   \       /   \                   /   \                   /   \    
    [1     0]               [1     2]   [1     3]               [1     5]                1     7 


if a number is already in tree, its count is increased 1.
(numbers: 7, 3, 2, 7)

                            ______________________|______________________                          
                           0                                             1                        
                __________/__________                           __________\__________              
               0                     1                         0                     1            
          ____/____               ____\____               ____/____               ____\____        
         0         1             0         1             0         1            0         1       
       _/_                     _/_         _\_                     _\_                     _\_     
      /   \                   /   \       /   \                   /   \                   /   \    
     1     0                [2]    2    [2]    3                 1     5                [3]    7 

When it is read recursively, can be get sorted array.

    sorted_array =  [1x0, 2x2, 2x3, 1x5, 3x7]
    sorted_array = [0, 2, 2, 3, 3, 5, 7, 7, 7]

*/
void bitSort(int * array, int arraySize) {
	int i, j;
	int ** addr;
    clock_t start, end;

    //create a buffer. root node is first node in buffer. 
	root = (int**) nextFreeIndex();

    //for every array element
    for (i = 0; i < arraySize; i++) {
            
        // start at root
        int ** activeNode = root;
    
        register int value = array[i];
        register int bit;

        //for every bit of value 
        for (j = INT_SIZE; j; j--){
            //from msb to lsb    
            bit = (0x80000000 & value) >> 31;
            value <<= 1;
            
            // get the related node from bit 
            addr = (int **) activeNode[bit];
            
            // if the node is not exists
            if (addr == 0) {
                // get next blank node from the buffer.
                addr = nextFreeIndex();

                //connect new node to previous node  
                activeNode[bit] = (int *) addr;
            }

            // jump to new node.
            activeNode = (int **) addr;
        }

        // moved on every bits in tree 
        if (*activeNode == 0) { 
            // but there is no value specified address(if cnt index is 0(zero))
            // set the value.
            activeNode[COUNT_INDEX] = 1;
            activeNode[VALUE_INDEX] = array + i;
            
            leafCount++; // count the leafs
        }
        else{
            // then if value set before, increase the count. 
            activeNode[COUNT_INDEX] = ((int)activeNode[COUNT_INDEX]) + 1;
        }     
             
    }
}


int main() {
    int *** leafAdresses;
	int i, count;
    int duration1, duration2;
	clock_t start, end;
    double cpu_time_used;


    //set an example array 
    printf("\n");
    printf("\n");
    printf("unsorted array(%d) : ", EXAMPLE_ARRAY_LENGHT); 
    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) {
        exampleArray[i] = randomd() * 50; //* 0x7FFFFFFF;
        printf("%d ", exampleArray[i]);
    }

    printf("\n");
    printf("\n");
    printf("Sorting Begin \n");
   
    start = clock();
        //sorting 
        bitSort(exampleArray, EXAMPLE_ARRAY_LENGHT);
	end = clock();
    duration1 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

    start = clock();
		//reading fom tree 
        leafAdresses = getSortedValueAddresses();
	end = clock();
    duration2 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

	printf("leafCount : %d\n", leafCount);
	printf("nodeCount : %d\n", nodeCount);
	printf("duration sort : %d us\n", duration1);
    printf("duration read : %d us\n", duration2);
   
    //print sorted array
    printf("\n");
    printf("\n");
    printf("sorted array(%d) : ", EXAMPLE_ARRAY_LENGHT); 
    for(i = 0; i < leafCount; i++){
		count = (int) leafAdresses[i][COUNT_INDEX];
		while(count--)
			printf("%d ", *(leafAdresses[i][VALUE_INDEX]));
    }
  
  
    printf("\n");
    printf("\n");
    printf("Done");
}
