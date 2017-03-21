//
// Created by lol on 3/15/2017.
//
#include "utils.h"

namespace algebra::utils {
    size_t pow(size_t a, size_t n) { return static_cast<size_t>(powl(a, n)); }
}