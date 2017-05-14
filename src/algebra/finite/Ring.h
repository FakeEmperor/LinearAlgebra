//
// Created by lol on 4/24/2017.
//

#ifndef SRC_ALGEBRA_FINITE_RING_H
#define SRC_ALGEBRA_FINITE_RING_H

namespace algebra::finite {
#include "Group.h"

    template <class T, const T Zero, const T One>
    class RingElement {

    };


    template <class RingElementType>
    class Ring : public Group<RingElementType> {

    };

}

#endif //SRC_ALGEBRA_FINITE_RING_H
