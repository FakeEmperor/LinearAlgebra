//
// Created by lol on 5/4/2017.
//

#ifndef SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H
#define SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H

#include <memory>
#include <ostream>
#include <set>
#include <utils/utils.h>
#include "../Group.h"

namespace algebra::finite::impl {

    // Forward declaration works only that way
    // - you need to declare all the template variables AND their default values
    template <typename IntegerType>
    class IntAdditiveGroup;


    template <typename IntegralType>
    class IntAdditiveFGroupElement: public FGroupElement<IntegralType, IntegralType(0), IntAdditiveFGroupElement> {
    public:
        static_assert(std::is_integral<IntegralType>::value,
                      "IntAdditiveFGroupElement can be defined for only integral types");

		#ifndef _MSC_VER
        typedef FGroupElement<IntegralType, IntegralType(0), IntAdditiveFGroupElement> Base;
		#else 
		typedef FGroupElement Base;
    	#endif
        // Yes, typedefs are not "inherited" - you need to full name for them
        // cause this: https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
        // So this is to "simulate" desired behaviour
        typedef typename Base::Ptr Ptr;
        typedef typename Base::GroupPtr GroupPtr;
        typedef typename Base::Group Group;

        IntAdditiveFGroupElement(const IntAdditiveFGroupElement &) = default;
        IntAdditiveFGroupElement(const IntegralType &value, const Group *bound_group) : value_(value), bound_group_(bound_group) {};

        ~IntAdditiveFGroupElement() override final = default;

//        explicit operator IntegralType() const override {
//            return value_;
//        }
        Ptr operator+(const IntAdditiveFGroupElement<IntegralType> &a) const override {
            return bound_group_->operateInside(this->value_ + a.value_);
        }

        Ptr inv() const override {
            return bound_group_->inverse(*this);
        }

        bool operator<(const IntAdditiveFGroupElement<IntegralType> &a) const override {
            return value_ < a.value_;
        }

        std::unique_ptr<std::set<Ptr>> stabilizator() const override {
            return nullptr;
        }

        GroupPtr group() override {
            return bound_group_;
        }

        bool isGenerator() const override {
            throw ::utils::NotImplementedError();
        }

        std::unique_ptr<Group> subgroup() const override {
            throw ::utils::NotImplementedError();
        }

        size_t order() const override {
            throw ::utils::NotImplementedError();
        }

        IntegralType operator*() const override {
            return value_;
        }


        std::ostream &print(std::ostream &ostream, const Group &group) const override {
            throw ::utils::NotImplementedError();
        }


    protected:
        IntegralType value_;
        GroupPtr bound_group_; // non-owning reference to the group. Be careful with it


    };


    template <typename IntegralType>
    class IntAdditiveGroup : public Group<IntegralType, IntAdditiveFGroupElement> {
    public:
        typedef Group<IntegralType, IntAdditiveFGroupElement> Base;
        typedef typename Base::ElemType ElemType;

        IntAdditiveGroup(const IntAdditiveGroup& ) = default;
        IntAdditiveGroup(IntAdditiveGroup&&) noexcept = default;
        explicit IntAdditiveGroup(const IntegralType &modulus) : modulus_(modulus) {}

        ~IntAdditiveGroup() override = default;
        typename Base::ElemType::Ptr operateInside(const IntegralType &a) const override {
            return ElemType::make(a % modulus_, this);
        }

        typename ElemType::Ptr inverse(const ElemType &e) const override {
            return ElemType::make(modulus_ - *e, this);
        }

        typename ElemType::Ptr operator[](size_t index) const override {
            // this is kind of a hack - we know that index of the element
            // is the element itself modulo group's order
            return operateInside(IntegralType(index));
        }

        size_t order() const override {
            return size_t(modulus_);
        }

        bool check() const override {
            throw ::utils::NotImplementedError();
        }

        std::ostream& print(std::ostream &ofstream, const Base &group) const override {
            throw ::utils::NotImplementedError();
        }

        void pregenerate() const {
            // do nothing - it's cheaper to generate
        }

        FFGroupProperties properties() const override {
            return FFGroupProperties::FGP_INVALID;
        }
    protected:
        const IntegralType& modulus_;
    };


}





#endif //SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H
