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

