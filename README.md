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
