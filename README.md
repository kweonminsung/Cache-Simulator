# Cache Simualtor

Computer Architecture Fall 2022  
Assignment 2 - Cache Simulator

```shell
$ make
$ ./main samples/trace1.din
```

### Overview

Implementation of a cache simulator. The simulator handles the following characteristics:

- Split Cache : I-cache and D-cache
- Cache Sizes : 1024, 2048, 4096, 8192, 16384 bytes
- Block Sizes : 16 bytes, 64 bytes
- Associativities : Direct Mapped, 2-way, 4-way, 8-way
- Replacement Policies : LRU(Least Recently Used)
- Write and Allocate Policy : Write Back and Write Allocate for D-cache
- Multi-level Cache : L1 (I-cache, D-cache), L2 (Unified cache)
- L2 cache configuration : 8-way, same block size as L1 cache, 16384bytes size
- L2 cache also uses the Write Back and Write Allocate policy.
- Multi-level Cache Policy: inclusive, exclusive
