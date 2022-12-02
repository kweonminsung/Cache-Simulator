#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "result.h"

using namespace std;

int now;

class CacheBlock {
    public:
        bool vaild = false;
        bool isEmpty = true;
        int tag;
        int time;
        CacheBlock() {}
        CacheBlock(bool vaild, int tag, int time) : vaild(vaild), tag(tag), time(time) {}
        void set(bool vaild, bool isEmpty, int tag, int time) {
            this->vaild = vaild;
            this->isEmpty = isEmpty;
            this->tag = tag;
            this->time = time;
        }
};

int sizeofCacheSet(CacheBlock* cacheSet, int way) {
    int emptyBlock = 0;
    for(int i = 0; i < way; i++) cacheSet[i].isEmpty && emptyBlock++;
    return way - emptyBlock;
}

void addCacheBlock(CacheBlock* cacheSet, int way, CacheBlock cacheBlock) {
    if(sizeofCacheSet(cacheSet, way) == way) {
        int tempIndex = 0, tempMin = cacheSet[0].time;
        for(int i = 0; i < way && !cacheSet[i].isEmpty; i++) if(cacheSet[i].time < tempMin) {
            tempIndex = i;
            tempMin = cacheSet[i].time;
        }
        cacheSet[tempIndex].set(true, false, cacheBlock.tag, now);
        return;
    }
    for(int i = 0; i < way; i++) if(cacheSet[i].isEmpty) {
        cacheSet[i].set(true, false, cacheBlock.tag, now);
        break;
    }
}

void removeCacheBlock(CacheBlock* cacheSet, int index) {
    cacheSet[index].isEmpty = false;
}

int log2i(int number) {
    int result = 0;
    while(number != 1) {
        result++;
        number /= 2;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Invalid Input" << endl;
        return 1;
    }
    fstream fs(argv[1]);
    int lines;

    int blockSizes[2] = {4, 16};
    int ways[4] = {1, 2, 4, 8};
    int totalCaches[5] = {256, 512, 1024, 2048, 4096};

    int waysLength = sizeof(ways) / sizeof(int), totalCachesLength = sizeof(totalCaches) / sizeof(int);
    
    for(int blockSize : blockSizes) {
        int** ICacheMiss_in = new int*[waysLength];
        int** DCacheMiss_in = new int*[waysLength];
        int** L2CacheMiss_in = new int*[waysLength];
        int** ICacheMiss_ex = new int*[waysLength];
        int** DCacheMiss_ex = new int*[waysLength];
        int** L2CacheMiss_ex = new int*[waysLength];
        int** MemWrite_in = new int*[waysLength];
        int** MemWrite_ex = new int*[waysLength];
        for(int i = 0; i < waysLength; i++) {
            ICacheMiss_in[i] = new int[totalCachesLength]; fill_n(ICacheMiss_in[i], totalCachesLength, 0);
            DCacheMiss_in[i] = new int[totalCachesLength]; fill_n(DCacheMiss_in[i], totalCachesLength, 0);
            L2CacheMiss_in[i] = new int[totalCachesLength]; fill_n(L2CacheMiss_in[i], totalCachesLength, 0);
            ICacheMiss_ex[i] = new int[totalCachesLength]; fill_n(ICacheMiss_ex[i], totalCachesLength, 0);
            DCacheMiss_ex[i] = new int[totalCachesLength]; fill_n(DCacheMiss_ex[i], totalCachesLength, 0);
            L2CacheMiss_ex[i] = new int[totalCachesLength]; fill_n(L2CacheMiss_ex[i], totalCachesLength, 0);
            MemWrite_in[i] = new int[totalCachesLength]; fill_n(MemWrite_in[i], totalCachesLength, 0);
            MemWrite_ex[i] = new int[totalCachesLength]; fill_n(MemWrite_ex[i], totalCachesLength, 0);
        }
        
        for(int i = 0; i < waysLength; i++) {
            for(int j = 0; j < totalCachesLength; j++) {
                now = 0; lines = 0;

                int totalSetsL1 = totalCaches[j] / (ways[i] * blockSize);
                int totalSetsL2 = 4096 / (8 * blockSize);

                CacheBlock** ICache_in = new CacheBlock*[totalSetsL1];
                CacheBlock** DCache_in = new CacheBlock*[totalSetsL1];
                CacheBlock** L2Cache_in = new CacheBlock*[totalSetsL2];

                CacheBlock** ICache_ex = new CacheBlock*[totalSetsL1];
                CacheBlock** DCache_ex = new CacheBlock*[totalSetsL1];
                CacheBlock** L2Cache_ex = new CacheBlock*[totalSetsL2];

                for(int k = 0; k < totalSetsL1; k++) {
                    ICache_in[k] = new CacheBlock[ways[i]];
                    DCache_in[k] = new CacheBlock[ways[i]];
                    ICache_ex[k] = new CacheBlock[ways[i]];
                    DCache_ex[k] = new CacheBlock[ways[i]];
                }
                for(int k = 0; k < totalSetsL2; k++) {
                    L2Cache_in[k] = new CacheBlock[8];
                    L2Cache_ex[k] = new CacheBlock[8];
                }

                for(string line; getline(fs, line);) {
                    lines++;

                    int temp, address, type = line.at(0) - '0';
                    stringstream ss;
                    ss << hex << line.substr(2);
                    ss >> address;
                    address >>= (2 + log2i(blockSize));

                    temp = address;
                    int indexL1 = temp % totalSetsL1;
                    int tagL1 = (totalSetsL1 != 0) ? temp >> log2i(totalSetsL1) : temp;

                    temp = address;
                    int indexL2 = temp % totalSetsL2;
                    int tagL2 =  (totalSetsL2 != 0) ? temp >> log2i(totalSetsL2) : temp;
                    // cout << "time " << now << "|type " << type << "|index " << indexL1 << "|tag " << tagL1;

                    bool Ihit_in = false, Dhit_in = false, L2hit_in = false;
                    bool Ihit_ex = false, Dhit_ex = false, L2hit_ex = false;
                    switch(type) {
                        case 0:
                        case 1: {
                            bool isWrite = type == 1 ? true : false;
                            for(int k = 0; k < ways[i]; k++) if(!DCache_in[indexL1][k].isEmpty && DCache_in[indexL1][k].vaild && DCache_in[indexL1][k].tag == tagL1) {
                                Dhit_in = true;
                                DCache_in[indexL1][k].time = now;
                            }
                            for(int k = 0; k < ways[i]; k++) if(!DCache_ex[indexL1][k].isEmpty && DCache_ex[indexL1][k].vaild && DCache_ex[indexL1][k].tag == tagL1) {
                                Dhit_ex = true;
                                DCache_ex[indexL1][k].time = now;
                            }
                            // D-Cache miss (inclusive)
                            if(!Dhit_in) {
                                DCacheMiss_in[i][j]++;
                                for(int k = 0; k < 8; k++) {
                                    if(!L2Cache_in[indexL2][k].isEmpty && L2Cache_in[indexL2][k].vaild && L2Cache_in[indexL2][k].tag == tagL2) {
                                        L2hit_in = true;
                                        L2Cache_in[indexL2][k].time = now;
                                        break;
                                    }
                                }
                                // L2 Cache miss
                                if(!L2hit_in) {
                                    L2CacheMiss_in[i][j]++;
                                    addCacheBlock(L2Cache_in[indexL2], 8, CacheBlock(true, tagL2, now));
                                    if(isWrite) MemWrite_in[i][j]++;
                                }
                                addCacheBlock(DCache_in[indexL1], ways[i], CacheBlock(true, tagL1, now));
                            }
                            // D-Cache miss (exclusive)
                            if(!Dhit_ex) {
                                DCacheMiss_ex[i][j]++;
                                for(int k = 0; k < 8; k++) {
                                    if(!L2Cache_ex[indexL2][k].isEmpty && L2Cache_ex[indexL2][k].vaild && L2Cache_ex[indexL2][k].tag == tagL2) {
                                        L2hit_ex = true;
                                        removeCacheBlock(L2Cache_ex[indexL2], k);
                                        break;
                                    }
                                }
                                if(!L2hit_ex) {
                                    L2CacheMiss_ex[i][j]++;
                                    if(isWrite) MemWrite_ex[i][j]++;
                                }
                                addCacheBlock(DCache_ex[indexL1], ways[i], CacheBlock(true, tagL1, now));
                            }
                            break;
                        }
                        case 2:
                            for(int k = 0; k < ways[i]; k++) if(!ICache_in[indexL1][k].isEmpty && ICache_in[indexL1][k].vaild && ICache_in[indexL1][k].tag == tagL1) {
                                Ihit_in = true;
                                ICache_in[indexL1][k].time = now;
                            }
                            for(int k = 0; k < ways[i]; k++) if(!ICache_ex[indexL1][k].isEmpty && ICache_ex[indexL1][k].vaild && ICache_ex[indexL1][k].tag == tagL1) {
                                Ihit_ex = true;
                                ICache_ex[indexL1][k].time = now;
                            }
                            // I-Cache miss (inclusive)
                            if(!Ihit_in) {
                                ICacheMiss_in[i][j]++;
                                for(int k = 0; k < 8; k++) {
                                    if(!L2Cache_in[indexL2][k].isEmpty && L2Cache_in[indexL2][k].vaild && L2Cache_in[indexL2][k].tag == tagL2) {
                                        L2hit_in = true;
                                        L2Cache_in[indexL2][k].time = now;
                                        break;
                                    }
                                }
                                // L2 Cache miss
                                if(!L2hit_in) {
                                    L2CacheMiss_in[i][j]++;
                                    addCacheBlock(L2Cache_in[indexL2], 8, CacheBlock(true, tagL2, now));
                                }
                                addCacheBlock(ICache_in[indexL1], ways[i], CacheBlock(true, tagL1, now));
                                if(DCache_in) MemWrite_ex[i][j]++;
                            }
                            // I-Cache miss (exclusive)
                            if(!Ihit_ex) {
                                ICacheMiss_ex[i][j]++;
                                for(int k = 0; k < 8; k++) {
                                    if(!L2Cache_ex[indexL2][k].isEmpty && L2Cache_ex[indexL2][k].vaild && L2Cache_ex[indexL2][k].tag == tagL2) {
                                        L2hit_ex = true;
                                        removeCacheBlock(L2Cache_ex[indexL2], k);
                                        break;
                                    }
                                }
                                if(!L2hit_ex) L2CacheMiss_ex[i][j]++;
                                addCacheBlock(ICache_ex[indexL1], ways[i], CacheBlock(true, tagL1, now));
                            }
                    }
                    now++;
                }
                fs.clear();
                fs.seekg(0);
            }
        }

        printResult(
            DCacheMiss_in, ICacheMiss_in, L2CacheMiss_in,
            DCacheMiss_ex, ICacheMiss_ex, L2CacheMiss_ex,
            MemWrite_in, MemWrite_ex,
            blockSize, waysLength, totalCachesLength, lines
        );
    }
    return 0;
}