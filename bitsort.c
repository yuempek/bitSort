/*
MIT License

Copyright (c) 2019 yuempek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


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
    int cnt[2];

    struct {
        int cnt;
        int value;
    } leaf;

    Block* node[2];
} Block;

typedef int ARRAY_ELEMENT_TYPE;

#define OUTPUT
#define EXAMPLE_ARRAY_LENGHT 100
#define BUFFER_LENGTH 2928 // = EXAMPLE_ARRAY_LENGHT*(31-3.3*LOG(EXAMPLE_ARRAY_LENGHT)) * 1.20
#define ARRAY_ELEMENT_TYPE_SIZE sizeof(ARRAY_ELEMENT_TYPE)*8
#define ARRAY_ELEMENT_TYPE_SIZE_1 ARRAY_ELEMENT_TYPE_SIZE-1

                
ARRAY_ELEMENT_TYPE exampleArray[EXAMPLE_ARRAY_LENGHT] = {0};

int freeIdxCnt = 0;

Block* freeIdx = 0;
Block* root;

int leafCount = 0;
int nodeCount = 0;
int mem = 0;

Block** sortedValueBlockAddresses;
int indx = 0;

void recurse(ARRAY_ELEMENT_TYPE * destArray, 
             Block* currentNode, 
             register char currentDigit, 
             register ARRAY_ELEMENT_TYPE numberOfNode) {
    int cnt, bit;
    
    if(currentDigit) { //if currentDigit>0, currentNode is node
        if (currentNode->node[0]) recurse(destArray, currentNode->node[0], currentDigit-1, numberOfNode << 1);
        if (currentNode->node[1]) recurse(destArray, currentNode->node[1], currentDigit-1, (numberOfNode << 1) | 1);
    }else { //currentNode is leaf
        for(bit = 0; bit < 2; bit++){ //for 0 and 1 branch 
            cnt = (int)currentNode->cnt[bit]; //get counts
            while(cnt--) destArray[indx++] = (numberOfNode << 1) | bit; //calculate the leaf value
        }
    }
}

void getSortedValues(ARRAY_ELEMENT_TYPE * arrayPtr, Block* root){
    ARRAY_ELEMENT_TYPE* destArray       = arrayPtr;
    Block*              currentNode     = root; 
    char                currentDigit    = ARRAY_ELEMENT_TYPE_SIZE_1; 
    ARRAY_ELEMENT_TYPE  numberOfNode = 0;
    
    //recurse(destArray, currentNode, currentDigit, numberOfNode);
    // to support negative numbers write below instead of above
    if (currentNode->node[1]) recurse(destArray, currentNode->node[1], currentDigit-1, (numberOfNode << 1) | 1);
    if (currentNode->node[0]) recurse(destArray, currentNode->node[0], currentDigit-1, numberOfNode << 1);
    
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

                            0_____________________|_____________________1                          
                           /                                             \                         
                0_________/_________1                           0_________\_________1              
               /                     \                         /                     \             
          0___/___1               0___\___1               0___/___1               0___\___1        
         /         \             /         \             /         \             /         \       
       cnt         cnt         cnt         cnt         cnt         cnt         cnt         cnt     
   
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
    
                            0_____________________|_____________________1                          
                           /                                             \                         

first number is added to binary tree using own bits from msb to lsb. 
(adding number: 7)

                                0_____________________|_____________________1                          
    L1  -------------------->                                                \                         
                                                                    0_________\_________1              
    L2  -------------------------------------------------------->                        \             
                                                                                      0___\___1        
    L3  ------------------------------------------------------------------------->             \       
                                                                                               [1]     

Then others sequently.
(adding numbers: 3, 2, 5, 0)

                            0_____________________|_____________________1                          
                           /                                             \                         
                0_________/_________1                           0_________\_________1              
               /                     \                         /                     \             
          0___/___1               0___\___1               0___/___1               0___\___1        
         /                       /         \                       \                       \       
       [1]                     [1]         [1]                     [1]                      1     

if a number is already in tree, its count is increased 1.
(numbers: 7, 3, 2, 7)

                            0_____________________|_____________________1                          
                           /                                             \                        
                0_________/_________1                           0_________\_________1              
               /                     \                         /                     \            
          0___/___1               0___\___1               0___/___1               0___\___1        
         /                       /         \             /         \             /         \       
       [1]                     [2]         [2]                     [1]                     [3]     

When it is read recursively, can be get sorted array.

    sorted_array =  [1x0, 2x2, 2x3, 1x5, 3x7]
    sorted_array = [0, 2, 2, 3, 3, 5, 7, 7, 7]
    
## Analyse

Duration & Memory is directly related to:
- N : Unique Element count
- D : Distribution of the numbers on tree.
- B : Bit size of the number.

### N (Unique Element count)
Uniq element count effects to node count. Every node creation is writing operation to Ram. Therefore number count effects duration and memory. The **worst case** is that N is perfectly distributed(minimum same branch on binary tree between numbers)(minimum intersection of bits). If the numbers perfectly distributed, node creation increase. Therefore durations also increase too.
Some samples for N uniform distributed 32 bit integers :

| N: element count  | C: node count  | C/N      | delta C/N    | log(N)   | delta logN  | (Delta C/N) / (delta logN)  |
|-------------------|----------------|----------|--------------|----------|-------------|-----------------------------|
| 1                 | 31             | 31       |  0           | 0        | -           | -                           |
| 10                | 275            | 27.5     | -3.5         | 1        | 1           | -3.5                        |
| 100               | 2445           | 24.45    | -3.05        | 2        | 1           | -3.05                       |
| 1000              | 21250          | 21.25    | -3.2         | 3        | 1           | -3.2                        |
| 10000             | 178200         | 17.82    | -3.43        | 4        | 1           | -3.43                       |
| 100000            | 1450000        | 14.5     | -3.32        | 5        | 1           | -3.32                       |
| 1000000           | 11178000       | 11.178   | -3.322       | 6        | 1           | -3.322                      |

    average((Delta C/N) / (delta logN)) = -3.3

every 10 times increase of N, C/N decreasing 3.3, so we can say :

    C/N = 31 - 3.3*LOG10(N)
    C = N * (31 - 3.3*LOG10(N))
    f_C(N) = C

| N: element count | C: average node count | f_C(N): formula |
|------------------|-----------------------|-----------------|
| 1                | 31                    | 31              |
| 10               | 275                   | 277             |
| 100              | 2445                  | 2440            |
| 1000             | 21250                 | 21100           |
| 10000            | 178200                | 178000          |
| 100000           | 1450000               | 1450000         |
| 1000000          | 11178000              | 11200000        |

When it will nears to limit of the integer, C/N nears to 2. Because, max N distinct element, fills the whole tree. And number of leaf is N, number of their parent N/2, then number of parent of parent N/4 and so on. 
total number of node is:  
    
    total_node = N + N/2 + N/4 + ... + 1
    total_node = N * (1 + 1/2 + 1/4 ... 1/N)
    
    N -> infinite
    total_node ~= N * 2


### D (Distribution of the numbers on tree)
"Distribution" effects duration, because tree is growing if the numbers don't use the branchs already exists.
The **best case** is that whole array is same number because of that distribution is minimum.
The **worst case** is that whole array is different and N is perfectly distributed in integer space.


### B (Bit size of the number)
Bit size changes the size of "number space" and "depth of the tree(Level)". Therefore intersect of the branchs of numbers will be maximum. So distrubution is decreasing and node creation will be minimum. 

## Comparison
if we create an array has 1000000(one million) integer number as:
- Same number
- Increasing from 1 to 1000000
- Random uniform distribution

Same number
---------------------------------------

    Leaf Count    : 1
    Block Count   : 31
    Block Size    : 16 byte
    Total Memory  : 512 byte
    Duration Sort : 178721 us (0.02s)
    Duration Read : 4994 us (0.005s)


Increasing from 1 to 1000000
---------------------------------------

    Leaf Count    : 1000000
    Block Count   : 1000018
    Block Size    : 16 byte
    Total Memory  : 16000304 byte (16MB)
    Duration Sort : 218556 us (0.2s)
    Duration Read : 14321 us (0.01s)


Random uniform distribution
---------------------------------------

    Leaf Count    : 999768 (uniq numbers, >%0,02 repetition)
    Block Count   : 11181318
    Block Size    : 16 byte
    Total Memory  : 178913456 byte (179MB)
    Duration Sort : 1460578 us (1.4s)
    Duration Read : 666933 us (0.7s)


# Order
Order **not** effects the sorting.


# What can be do?
## Change bitsize
Before sorting, can be find the max element as bit. And bitsize of algorithm can be set to max bit size. If the values are small, it can be increase performance because of low tree depth.

## Float/Double number sorting
Before sorting, can be changed of position of bits from MSB to LSB. It will work. **Important thing of this algorithm is bit order must be MSB to LSB. Value is not important.** 

## String sorting
The hardest thing is. But it is posible. Fistly, strings can be sorting by length of string, after that same size strings can be sort byte by byte.

*/
void bitSort(int * array, int arraySize) {
	int i, j;
	Block* block;
    clock_t start, end;

    //create a buffer. root node is first node in buffer. 
	root = initBlockBuffer();
    const unsigned long long digit = ((unsigned long long) 1) << (ARRAY_ELEMENT_TYPE_SIZE_1);
    //for every array element
    for (i = 0; i < arraySize; i++) {
            
        // start at root
        Block* activeBlock = root;
    
        register int value = array[i];
        register int bit;

        //printf("\n");

        //for every bit of value 
        for (j = 0; j < ARRAY_ELEMENT_TYPE_SIZE_1; j++){
            //from msb to lsb    
            bit = (digit & (value << j)) >> (ARRAY_ELEMENT_TYPE_SIZE_1);

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

        if(activeBlock->cnt[value & 1] == 0) leafCount++;  
        // moved on every bits in tree 
        activeBlock->cnt[value & 1]++;  
        
    }
}


int main() {
    Block** leafAdresses;
	int i, count;
    int duration1, duration2;
	clock_t start, end;
    double cpu_time_used;

    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) {
        exampleArray[i] = 0x7FFFFFFF; // same numbers
        //exampleArray[i] = i; //increasing
        //exampleArray[i] = randomd() * 100 - 50; // recurrent
        //exampleArray[i] = randomd() * 0x7FFFFFFF - 0x3FFFFFFF; // uniform distributed
    }

    //set an example array 
    printf("\n");
    printf("\n");
    printf("unsorted array(%d) : \n", EXAMPLE_ARRAY_LENGHT); 
#ifdef OUTPUT
    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) {
       printf("%d\n", exampleArray[i]);
    }
#endif

    printf("\n");
    printf("\n");
    printf("Sorting Begin \n");
   
    start = clock();
        //sorting 
        bitSort(exampleArray, EXAMPLE_ARRAY_LENGHT);
	end = clock();
    duration1 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

    start = clock();
		//reading from tree 
        getSortedValues(exampleArray, root);
	end = clock();
    duration2 = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000;

	printf("Leaf Count    : %d\n", leafCount);
	printf("Block Count   : %d\n", nodeCount);
	printf("Block Size    : %ld byte\n", sizeof(Block));
	printf("Total Memory  : %d byte\n", mem);
	printf("Duration Sort : %d us\n", duration1);
    printf("Duration Read : %d us\n", duration2);
   
    //print sorted array
    printf("\n");
    printf("\n");
    printf("sorted array(%d) : \n", EXAMPLE_ARRAY_LENGHT); 

#ifdef OUTPUT
    for (i = 0; i < EXAMPLE_ARRAY_LENGHT; i++) {
       printf("%d\n", exampleArray[i]);
    }
#endif
  
  
    printf("\n");
    printf("\n");
    printf("Done");
}

