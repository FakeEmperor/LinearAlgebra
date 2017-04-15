#pragma once

#ifndef SRC_UTILS_UTILS_H
#define SRC_UTILS_UTILS_H
#include <cstdlib>
#include <math.h>



namespace algebra::utils {
    size_t pow(size_t a, size_t n);
    constexpr size_t cpow(size_t a, size_t n, size_t res = 1);
}
#endif
