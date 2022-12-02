#include <cmath>
#include "result.h"

using namespace std;

void printResult(
    int** DCacheMiss_in, int** ICacheMiss_in, int** L2CacheMiss_in,
    int** DCacheMiss_ex, int** ICacheMiss_ex, int** L2CacheMiss_ex,
    int** MemWrite_in, int** MemWrite_ex,
    int blockSize, int waysLength, int totalCachesLength, int lines
) {
    cout << "Cache Misses (block size = " << blockSize << "B, inclusive, D-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)DCacheMiss_in[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Cache Misses (block size = " << blockSize << "B, inclusive, I-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)ICacheMiss_in[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Cache Misses (block size = " << blockSize << "B, inclusive, L2-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)L2CacheMiss_in[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Memory Writes (block size = " << blockSize << "B, inclusive)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << MemWrite_in[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Cache Misses (block size = " << blockSize << "B, exclusive, D-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)DCacheMiss_ex[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Cache Misses (block size = " << blockSize << "B, exclusive, I-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)ICacheMiss_ex[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Cache Misses (block size = " << blockSize << "B, exclusive, L2-Cache)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << round((double)L2CacheMiss_ex[i][j] / lines * 10000) / 100 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Memory Writes (block size = " << blockSize << "B, exclusive)" << endl;
    for(int i = 0; i < waysLength; i++) {
        for(int j = 0; j < totalCachesLength; j++) {
            cout << MemWrite_ex[i][j] << " ";
        }
        cout << endl;
    }
    cout << "====================================================================================" << endl;
}