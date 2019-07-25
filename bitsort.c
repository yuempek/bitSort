#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <time.h>
#include <stdlib.h>


void sleep(double t){
    char time[100];
    sprintf(time, "sleep %f", t);
    fflush(stdout);            
    system(time);
}

double randomd()   /* uniform distribution [0..1] */
{
  int i;
  unsigned long x = 0;

  srand(clock());
  for(i = 0; i < 8; i++)
      x = (x << 8) | (rand() & 0xFF);

  return 1.0*x / (unsigned long)(-1);
}


typedef union Block Block;

typedef union Block {
    struct {
        int cnt;
        int value;
    } leaf;

    Block* node[2];
} Block;

#define OUTPUT
#define EXAMPLE_ARRAY_LENGHT 100
#define BUFFER_LENGTH EXAMPLE_ARRAY_LENGHT
#define INT_SIZE 32

                
int exampleArray[EXAMPLE_ARRAY_LENGHT] = {0};

int freeIdxCnt = 0;

Block* freeIdx = 0;
Block* root;

int leafCount = 0;
int nodeCount = 0;
int mem = 0;

Block** sortedValueBlockAddresses;
int indx = 0;

void recurse(Block* currentNode, char digit) {
    if(digit == 0) { //this is leaf
        sortedValueBlockAddresses[indx] = currentNode;
        indx++;
        return;
    }
    
    if (currentNode->node[0]) recurse(currentNode->node[0], digit-1);
    if (currentNode->node[1]) recurse(currentNode->node[1], digit-1);  
}

Block** getSortedValueBlockAddresses(){
    if(sortedValueBlockAddresses) free(sortedValueBlockAddresses);
    sortedValueBlockAddresses = malloc(leafCount * sizeof(Block*));
    recurse(root, INT_SIZE);
    return sortedValueBlockAddresses;
}

Block* initBlockBuffer(){
    freeIdx = malloc((BUFFER_LENGTH) * sizeof(Block));
    freeIdxCnt = BUFFER_LENGTH - 1;
    mem += BUFFER_LENGTH * sizeof(Block);
    return freeIdx;
}

Block* nextFreeBlock(){
    
    if(freeIdxCnt--) 
        freeIdx++;
    else 
        initBlockBuffer();

    freeIdx->node[0] = 0;
    freeIdx->node[1] = 0;
    
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
         0         1             0         1             0         1             0         1       
       _/_                     _/_         _\_                     _\_                     _\_     
      /   \                   /   \       /   \                   /   \                   /   \    
     1     0                [2]    2    [2]    3                 1     5                [3]    7 
When it is read recursively, can be get sorted array.
    sorted_array =  [1x0, 2x2, 2x3, 1x5, 3x7]
    sorted_array = [0, 2, 2, 3, 3, 5, 7, 7, 7]
*/
void bitSort(int * array, int arraySize) {
	int i, j;
	Block* block;
    clock_t start, end;

    //create a buffer. root node is first node in buffer. 
	root = initBlockBuffer();
    const unsigned long long digit = ((unsigned long long) 1) << (INT_SIZE - 1);
    //for every array element
    for (i = 0; i < arraySize; i++) {
            
        // start at root
        Block* activeBlock = root;
    
        register int value = array[i];
        register int bit;

        //printf("\n");

        //for every bit of value 
        for (j = 0; j < INT_SIZE; j++){
            //from msb to lsb    
            bit = (digit & (value << j)) >> (INT_SIZE - 1);

            // get the related node from bit 
            block = activeBlock->node[bit];
            
            
            // if the node is not exists
            if (block == 0) {
                // get next blank node from the buffer.
                block = nextFreeBlock();

                //connect new node to previous node  
                activeBlock->node[bit] = block;
            }

            // jump to new node.
            activeBlock = block;
        }

        // moved on every bits in tree 
        if (activeBlock->leaf.cnt == 0) { 
            // but there is no value specified address(if cnt index is 0(zero))
            // set the value.
            activeBlock->leaf.cnt = 1;
            activeBlock->leaf.value = array[i];
            
            leafCount++; // count the leafs
        }
        else{
            // then if value set before, increase the count. 
            activeBlock->leaf.cnt++;
        }     
             
    }
}


int main() {
    Block** leafAdresses;
	int i, count;
    int duration1, duration2;
	clock_t start, end;
    double cpu_time_used;


    //set an example array 
    printf("\n");
    printf("\n");
    printf("unsorted array(%d) : ", EXAMPLE_ARRAY_LENGHT); 
    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) {
        exampleArray[i] = randomd() * 50;//0x7FFFFFFF;
#ifdef OUTPUT
       printf("%d ", exampleArray[i]);
#endif
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
        leafAdresses = getSortedValueBlockAddresses();
	end = clock();
    duration2 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

	printf("leafCount : %d\n", leafCount);
	printf("nodeCount : %d\n", nodeCount);
	printf("memory : %d\n", mem);
	printf("sizeof(Block) : %ld\n", sizeof(Block));
	printf("duration sort : %d us\n", duration1);
    printf("duration read : %d us\n", duration2);
   
    //print sorted array
    printf("\n");
    printf("\n");
    printf("sorted array(%d) : ", EXAMPLE_ARRAY_LENGHT); 

#ifdef OUTPUT
    for(i = 0; i < leafCount; i++){
		count = leafAdresses[i]->leaf.cnt;
		while(count--)
			printf("%d ", leafAdresses[i]->leaf.value);
    }
#endif
  
  
    printf("\n");
    printf("\n");
    printf("Done");
}

