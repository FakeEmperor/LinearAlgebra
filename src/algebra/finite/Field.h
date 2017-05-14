//
// Created by lol on 4/21/2017.
//

#ifndef SRC_ALGEBRA_FINITE_GALOISFIELD_H
#define SRC_ALGEBRA_FINITE_GALOISFIELD_H

#include "Ring.h"
#include <cstdlib>
#include <vector>

namespace algebra::finite {
    template<typename T>
    class FFieldElement;

    class FFieldElementGenerator;

    class FFieldFactorGenerator;

    class FFieldError;

    template<class T, const T Zero, const T One>
    class Field : public Ring<T, Zero, One> {
    public:
        static Field Build(const T &base, const T &factor);

        static Field Build(FFieldElementGenerator *gen, const T &factor);

        static Field Build(FFieldElementGenerator *e_gen, FFieldFactorGenerator *f_gen);

        static void Test(const Field &gf);

        static void GroupTest(const Field &gf);

        static void FieldTest(const Field &gf);

        static void RingTest(const Field &gf);
        /////////////////////////////

        Field(const Field &) = default;

        virtual ~Field() = default;

        FFieldElement<T> operator[](size_t m_index) const;
        // T operator [] (size_t m_index) const;

        size_t order() const;

        size_t m_order() const;


        T Zero();

        T One();

        FFieldElement<T> Zero();

        FFieldElement<T> One();

    protected:
        std::vector<T> elements_; //including 'One'
        const T &factor_;

        Field(const std::vector<T> &elements, const T &factor)

    };

    template<class T, const T Zero, const T One>
    void Field::Test(const Field &gf) {
        GroupTest(gf);
        RingTest(gf);
        FieldTest(gf);
    }

    template<class T, const T Zero, const T One>
    void Field::GroupTest(const Field &gf) {
        // check commutability

        // check permutability

        // check associability

    }

    template<class T, const T Zero, const T One>
    T Field::Zero() {
        return Zero;
    }

    template<class T, const T Zero, const T One>
    FFieldElement<T> Field::One() {
        return One;
    }

    template<class T, const T Zero, const T One>
    size_t Field::order() const {
        return elements_.size() + 1;
    }

    template<class T, const T Zero, const T One>
    size_t Field::m_order() const {
        return elements_.size();
    }

}
#endif //SRC_ALGEBRA_FINITE_GALOISFIELD_H
