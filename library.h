#ifndef MARRAY_LIBRARY_H
#define MARRAY_LIBRARY_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include <functional>

/**
 * Binary search with return the position of most up to a val in a sorted sequence.
 *
 * Ex:  bin_search<int>([0, 1, 3, 4, 6, 9, 10], 7, 5) = 3;
 * Ex:  bin_search<int>([0, 1, 3, 4, 6, 9, 10], 7, 0) = 0;
 * Ex:  bin_search<int>([0, 1, 3, 4, 6, 9, 10], 7, -2) = -1;
 * Ex:  bin_search<int>([0, 1, 3, 4, 6, 9, 10], 7, 12) = 6;
 * @tparam Dtype
 * @tparam Compare
 * @param a
 * @param n
 * @param val
 * @param comp
 * @return
 */
template <typename Dtype, class Compare>
inline int bin_search(Dtype * a, const int &n, const Dtype & val, Compare comp) {
    if (n < 1) {
        return -1;
    }
    if (n == 1) {
        if (comp(a[0], val)) return 0;
        else return -1;
    }
    if (n == 2) {
        if (comp(a[1], val)) {
            return 1;
        } else if (comp(a[0], val) && comp(val, a[1])) {
            return 0;
        } else  return -1;
    }
    int m = (int)(n/2);
    if (comp(a[m], val)) {
        return m + bin_search<Dtype, Compare>(a+m, n-m, val, comp);
    } else {
        return bin_search<Dtype, Compare>(a, m, val, comp);
    }
}

inline bool comp_t(size_t a, size_t b) {
    return (a<=b);
}

/**
 * A very simple Marray which has a FIXED SIZE memory. A strong guarantee that it will not use more than the fixed size memory given.
 * No garbage will be left for environmental GC.
 * IT performs insertions and retrieval in logarithmic time.
 * It is a often case in practice when the data size is less than or equal to 2^64 (so 64-bit indexes).
 * In this fixed case, the time for all operations should be O(64) ~ constant.
 * Marray does not support deletions, but it support overwrites through `reset_to(indice)` function.
 * When the fixed size is reached, no new data will be inserted, so you must reset the pointer to overwrite data.
 * Note that a MARRAY will not allocate any new bytes during its lifetime.
 *
 * MARRAY has been designed for our specific purposes since several years ago, so don't ask WHY WE DID TALK ABOUT MARRAY!
 * We only re-implemented it based on our shallow memory.
 * Just know pros and cons as described above and adapt it to your projects!
 *
 * This implementation is not thread-safe. But you can adapt it easily.
 */
class Marray {
public:
    explicit Marray(const size_t &size, const unsigned int & data_size) {
        this->data_size = data_size;
        this->size = size;
        try {
            data = (unsigned char *)::operator new(data_size * size);
        } catch (std::bad_alloc & e) {
            std::cerr << e.what() << std::endl;
//            exit(1);
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
//            exit(1);
        }

        try {
            ptr = (size_t *)::operator new(size * sizeof(size_t));
        } catch (std::bad_alloc & e) {
            std::cerr << e.what() << std::endl;
//            exit(1);
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
//            exit(1);
        }

        if (ptr != nullptr) memset(ptr, 0, size * sizeof(size_t));
        if (data != nullptr) memset(data, 0, data_size * size);
    }

    ~Marray() {
        ::delete ptr;
        ptr = nullptr;
        ::delete data;
        data = nullptr;
    }

    /**
     * Insert a key-value into Marray.
     * In average case, it is a logarithmic algorithm.
     * When the size of Marray is <= 2^64, we expect it is <= O(64), then in this case it should be near constant.
     *
     * @param id the KEY -LOM
     * @param code the code data array, must have length equal to `data_size`
     */
    void put(const char * id, const unsigned char * code) {
        if (current_pos >= size) {
            std::cout << "No insertion should be done. FULL MARRAY" << std::endl;
            return;
        }
        std::string s = std::string(id);
        auto h = std::hash<std::string>{}(s);
        auto pos = bin_search<size_t>(ptr, current_pos, h, comp_t); // O(log(current_pos))
        if (pos < 0) {
            std::memcpy(ptr+1, ptr, current_pos*sizeof(size_t));
            std::memcpy(data+data_size, data, current_pos*data_size);
            ptr[0] = h;
            std::memcpy(data, code, data_size);
        } else if (pos < current_pos) {
            if (ptr[pos] == h) {
                std::cerr << "KEY-LOM was already in the MARRAY" << std::endl;
                return;
            }
            std::memcpy(ptr+pos+2,ptr+pos+1,(current_pos-pos) * sizeof(size_t));
            std::memcpy(data+(pos+2)*data_size, data+(pos+1)*data_size, (current_pos-pos)*data_size);
            ptr[pos+1] = h;
            std::memcpy(data + (pos+1)*data_size, code, data_size);
        } else {
            ptr[current_pos] = h;
            std::memcpy(data + current_pos*data_size, code, data_size);
        }
        current_pos++;
    }

    void reset_to(int pos) {
        // reset to a prior position to overwrite data if you need.
        if (pos < size && pos >= 0) {
            current_pos = pos;
        }
    }

    /**
     * Logarithmic algorithm to find the position of a specific data w.r.t a given string ID.
     * @param id
     * @return
     */
    int pos(const char * id) {
        std::string s = std::string(id);
        auto h = std::hash<std::string>{}(s);
        auto pos_ = bin_search<size_t>(ptr, current_pos, h, comp_t); // O(log(current_pos))
        if (pos_ < 0 || pos_ >= current_pos) return -1;
        if (ptr[pos_] != h) return -1;
        return pos_;
    }

    unsigned char * get(const char * id) {
        auto pos_ = pos(id);
        if (pos_ < 0 || pos_ > current_pos) {
            return nullptr;
        }
        return data + pos_ * data_size;
    }

protected:
    size_t * ptr = nullptr;
    unsigned char * data = nullptr;
    size_t current_pos = 0;
    int data_size;
    size_t size;
};

#endif //MARRAY_LIBRARY_H
