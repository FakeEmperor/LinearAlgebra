//
// Created by lol on 4/24/2017.
//

#ifndef SRC_ALGEBRA_FINITE_FINITEALGEBRAERROR_H
#define SRC_ALGEBRA_FINITE_FINITEALGEBRAERROR_H

#include <stdexcept>

namespace algebra::finite {

    class FiniteAlgebraError : public std::runtime_error {
    public:
        FiniteAlgebraError(const std::string &__arg) : runtime_error(__arg) {}

    };

}


#endif //SRC_ALGEBRA_FINITE_FINITEALGEBRAERROR_H
