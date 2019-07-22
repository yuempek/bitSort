#BitSort

Bit sort is a new sorting algorithm that uses binary tree to sort elements. Numbers are put in a binary tree. When reading they sorted already. Complexity is O(nk). k is bit size of the number. BitSort is not recursive algorithm when sorting the values. But reading the tree is recursive.

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
    
    L : level
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
    L1                         0                                             1                         
                                                                    __________\__________              
    L2                                                             0                     1             
                                                                                      ____\____        
    L3                                                                                         1       
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
