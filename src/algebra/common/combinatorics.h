//
// Created by lol on 3/22/2017.
//

#ifndef ALGEBRA_COMMON_COMBINATORICS_H
#define ALGEBRA_COMMON_COMBINATORICS_H

#include <vector>
#include <cstddef>
#include <array>
#include <queue>
#include <stack>
#include <algorithm>

namespace algebra::common::combinatorics {

    /*
    template <typename T, size_t BlockSize>
    std::vector<std::array<T, BlockSize>> PlacementsWithoutRepetitions(const std::vector<T>& holder) {
        auto sz = holder.size();
        if (BlockSize > sz || BlockSize == 0 || holder.size() == 0)
            return {};
        if (BlockSize == sz)
            return {{holder.begin(), holder.end()}};
        std::vector<std::array<T, BlockSize>> res;
        std::array<std::pair<size_t,  T*>, BlockSize> temp;
        temp[0] = {{ 0, holder[0]}};
        size_t temp_size = 1;
        while (true) {
            for(;temp_size < BlockSize; temp_size++)
                temp[temp_size] = { temp[temp_size-1]+1,  holder[temp_size]};
            while(temp[BlockSize-1])

        }
    } */

    /**
     * Generates C(n,k) in vector form.
     * @param n Number of distinct values. Values are ranged from 0 to n-1
     * @param block_size Block size.
     * @return A vector of vectors of fixed size. Each represents one possible placement
     */
    std::vector<std::vector<size_t>> IndexPlacementsWithoutRepetitions(size_t n, const size_t block_size) {
        if (block_size > n || n == 0 || block_size == 0)
            return {};

        std::vector<size_t> buf(block_size);
        auto fill_inc = [&buf, block_size] (size_t start_index) {
            for (size_t i = start_index; i<block_size; ++i)
                buf[i] = buf[i-1] + 1;
        };
        buf[0] = 0; fill_inc(1);
        if (n == block_size)
            return { { buf } };
        std::vector<std::vector<size_t>> res;
        bool allow = true;
        size_t bidx = block_size; // index of the first position to increment in sequence regeneration (see below)
        while (allow)
        {
            // generate index placements and push them to results
            while (buf[block_size-1] < n) {
                res.push_back(buf);
                buf[block_size - 1]++;
            }
            /**
             * <regeneration block> - as we now have an invalid sequence:
             * a0 (0), a1 (1), a2 (2) ..., a[bidx] (some number), a[bidx]+1, ... n (<-- this last sequence element is invalid)
             * we need to regenerate the sequence.
             * we do it by incrementing a[bidx] again and generating second part of the sequence...again:
             * 0, 1, 2, ... a[bidx]+1, a[bidx]+2, ... a[bidx]+(BLOCK_SIZE-bidx)
             * BUT!! If we will increment that way we can have a situation, when
             * a[bidx+K] for some K: a[bidx+K] >= n.
             * And this is an invalid sequence again. So we check that, and if that happens
             * bidx just decreases and it will be checked again because for every bidx it is true:
             * If i < bidx <=> a[i] < a[bidx]
            **/
            while(bidx != -1 && n - buf[bidx] >= block_size - bidx)
                --bidx;
            // if we didn't find an index which satisfies the condition
            if (bidx == -1)
                allow = false;
            ++buf[bidx]; fill_inc(bidx);
            // </regeneraton block>
        }


        return res;
    };




}



#endif //ALGEBRA_COMMON_COMBINATORICS_H
