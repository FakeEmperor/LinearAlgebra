//
// Created by lol on 4/24/2017.
//

#ifndef   SRC_ALGEBRA_FINITE_GROUP_H
#define   SRC_ALGEBRA_FINITE_GROUP_H

#include <cstdlib>
#include <vector>
#include <type_traits>
#include "FiniteAlgebraError.h"

namespace algebra::finite {

    enum FFGroupProperties {
        FGP_INVALID = 0xFF,
        FGP_UNKNOWN = 0x00,
        FGP_ASSOCIATIVE = 0x01,
        FGP_COMMUTATIVE = 0x02,
        FGP_ABELIAN = 0x03
    };


    template <typename, template <typename> class GroupElementType>
    class Group;
    class FGroupElementGenerator;
    class FGroupFactorGenerator;

    template <class T, T Neutral, template<typename> class Impl_>
    class FGroupElement {
    public:
        /*static_assert(std::is_base_of<FGroupElement, Impl<T>>::value,
                      "Template parameter Impl must implement FGroupElement interface");*/

        typedef Impl_<T> Impl;
        typedef T IntegralType;
        typedef std::unique_ptr<Impl> Ptr;
        typedef Group<T, Impl_> Group;
        typedef const Group* GroupPtr;

        static constexpr IntegralType NeutralValue = Neutral;

        virtual ~FGroupElement() = default;

        // virtual operator IntegralType_() const = 0;
        //virtual operator FGroupElement(const T&) const = 0;

        // ARITHMETIC OPERATORS

        // primary operator, can be mapped into *
        virtual Ptr operator+(const Impl &) const = 0;

        // inverse operator - mapped into inv() method
        virtual Ptr operator-() const {
            return inv();
        }

        // secondary operator, may throw NotSupportedOperation() if not commutative
        virtual Ptr operator-(const Impl &a) const {
            return *this + *a.inv();
        }

        virtual Ptr inv() const = 0;

/*
*/
        // LOGICAL OPERATORS
        virtual bool operator<(const Impl &) const = 0;

        virtual bool operator>(const Impl &a) const {
            return !(*this < a) && !(*this == a);
        }

        virtual bool operator>=(const Impl &a) const {
            return !(*this < a);
        }

        virtual bool operator<=(const Impl &a) const {
            return !(*this > a);
        }

        virtual bool operator==(const Impl &a) const {
            return !(*this < a) && !(*this > a);
        }

        virtual bool operator!=(const Impl &a) const {
            return !(*this == a);
        }

        // Group-specific operators
        virtual std::unique_ptr<std::set<Ptr>> stabilizator() const = 0;

        virtual GroupPtr group() = 0;

        virtual bool isGenerator() const = 0;

        virtual std::unique_ptr<Group> subgroup() const = 0;

        virtual size_t order() const = 0;


        // Note the missing & - it is a full copy, not a reference
        virtual T operator*() const = 0;

        // Printing
        friend std::ostream &operator<<(std::ostream &a, const Impl &b) {
            return b.print(a, b);
        }


        virtual std::ostream &print(std::ostream &, const Group &) const = 0;

        template<typename... Args>
        static Ptr make(Args&&... args) {
            return Ptr(new Impl(std::forward<Args>(args)...));
        };
    };




    // Abel's Finite Group implementation
    template <typename IntegralType_, template <typename> class GroupElementType>
    class Group {
    public:
        // typedefs for convenience
        typedef GroupElementType<IntegralType_> ElemType;
        typedef IntegralType_ IntegralType;
        // constexpr for convenience
        static constexpr IntegralType_ NeutralValue = ElemType::NeutralValue;
        //////////////////
        class ElementIterator {
        public:
            ElementIterator(typename ElemType::Ptr&& a) : element_(a) {}
            ElementIterator operator ++() { return element_+element_.group().generator(); }
            ElementIterator operator ++(int) { return element_+element_.group().generator(); }
        private:
            typename ElemType::Ptr element_;


        };
    public:
		/*static_assert (std::is_base_of<
                               FGroupElement<
                                       typename GroupElementType::IntegralType_,
                                       GroupElementType::NeutralValue,
                                       GroupElementType>,
                               GroupElementType>::value,
			"GroupElementType must have FGroupElement as a subclass with same type traits");*/




        /////////////////////////////
        virtual ~Group() = default;

        // GROUP OPERATIONS

        /*
         * (it is used to signal the group that operation
         * on a group element is taking place
         * well, inside the group.
         * It can be modular arithmetics, fast power (if group supports it),
         * EC arithmetics and etc.
        */
        virtual typename ElemType::Ptr operateInside(const IntegralType_&) const = 0;

        virtual typename ElemType::Ptr inverse(const ElemType& ) const = 0;
        // ACCESS OPERATORS
        virtual typename ElemType::Ptr operator[](size_t index) const = 0;
        // T operator [] (size_t m_index) const

        // 1ST AGGREGATON METRICS OPERATORS
        virtual size_t order() const = 0;

        // SANITY CHECKS
        virtual bool check() const noexcept(false) = 0;
        virtual FFGroupProperties properties() const = 0;

        virtual void pregenerate() const = 0;

        virtual typename ElemType::Ptr zeroElem() const = 0;
        virtual typename ElemType::Ptr maxElem() const = 0;
        virtual typename ElemType::Ptr minGenerator() const = 0;
        // PRINT OPERATORS AND METHODS


        friend std::ostream& operator << (std::ostream &a, const Group &b) {
            return b.print(a, b);
        };
        virtual std::ostream& print(std::ostream&, const Group& ) const = 0;

        virtual ElementIterator begin() const {
            return ElementIterator(zeroElem());
        }

        virtual ElementIterator end() const {
            return ElementIterator(maxElem());
        }
    };


}
#endif // SRC_ALGEBRA_FINITE_GROUP_H
