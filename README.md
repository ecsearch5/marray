MARRAY
=====

A logarithmic multi-array. MARRAY is a very simple Marray which has a FIXED SIZE memory. A strong guarantee that it will not use more than the fixed size memory given.
 
 * Pros:
    * No garbage will be left for environmental GC.
    * MARRAY performs insertions and retrieval in logarithmic time and constant in typical cases.
    * MARRAY supports up to `2^64` items on storage.
        * It is a often case in practice when the data size is less than or equal to 2^64 (so 64-bit indexes).
        * In this fixed case, the time for all operations should be O(64) ~ constant.
 * Cons:
    * Marray does not support deletions, but it support overwrites through `reset_to(indice)` function.
    * When the fixed size is reached, no new data will be inserted, so you must reset the pointer to overwrite data.
 * Note that a MARRAY will not allocate any new bytes during its lifetime.

MARRAY has been designed for our specific purposes since several years ago, so don't ask WHY WE DID TALK ABOUT MARRAY!
We only re-implemented it based on our shallow memory.
Just know pros and cons as described above and adapt it to your projects!

* This implementation is not thread-safe. But you can adapt it easily.

## Examples

You can see [`test_library`](./test_library.cpp):

```shell script
$ cmake .
$ make all -j8
$ ./test_library
KEY-LOM was already in the MARRAY
```