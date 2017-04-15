//
// Created by lol on 3/15/2017.
//
#include "utils.h"

size_t algebra::utils::pow(size_t a, size_t n) { return static_cast<size_t>(powl(a, n)); }
constexpr size_t algebra::utils::cpow(size_t a, size_t n, size_t res = 1) {
    return n < 1 ? res : ( res*=a, n/=2, cpow(a, n, res) );
}

