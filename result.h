#include <iostream>

void printResult(
    int** DCacheMiss_in, int** ICacheMiss_in, int** L2CacheMiss_in,
    int** DCacheMiss_ex, int** ICacheMiss_ex, int** L2CacheMiss_ex,
    int** MemWrite_in, int** MemWrite_ex,
    int blockSize, int waysLength, int totalCachesLength, int lines
);