This project is a cache simulator that is capable of imitating cache performance of any reasonable size and associativity using LRU policy OR FIFO policy

Please be sure the included "Cache.h" header file is in the current working directory.
The working directory should also include a file what contains a test bench. The contents should be of hexadecimal value.

To run from command line call the program with 4 arguments
1. Cache size to simulate (int)
2. Associativity (int)
3. 0 to simulate Least Recently Used (LRU) policy OR 1 to simulate First In First Out (FIFO) policy
4. The name of the file that contains the Hexadecimal values to run the simulation


NOTE: This project was a homework assignment and use to include two very large files to use as a test bench, each that contained cache log files( XSBENCH.t and MINIFE.t file).
However, due to how large they were, I deleted them and no longer have them. To easily test this you will need to provide your own test bench. 
