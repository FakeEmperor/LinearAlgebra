//
// Created by lol on 5/4/2017.
//

#ifndef SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H
#define SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H

#include <memory>
#include "../Group.h"

namespace algebra::finite::impl {

    // Forward declaration works only that way
    // - you need to declare all the template variables AND their default values
    template <typename IntegerType>
    class IntAdditiveGroup;


    template <typename IntegerType, typename = std::enable_if<std::is_integral<IntegerType>::value>::type>
    class IntAdditiveFGroupElement: public FGroupElement<IntegerType, IntegerType(0), IntAdditiveFGroupElement<IntegerType>> {
    public:
        typedef FGroupElement<IntegerType, IntegerType(0), IntAdditiveFGroupElement<IntegerType>> Base;
        // Yes, typedefs are not "inherited" - you need to full name for them
        // cause this: https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
        // So this is to "simulate" desired behaviour
        typedef typename Base::Ptr Ptr;
        typedef typename Base::GroupPtr GroupPtr;
        typedef typename Base::Group Group;

        IntAdditiveFGroupElement(const IntAdditiveFGroupElement &) = default;
        IntAdditiveFGroupElement(const IntegerType &value, const Group *bound_group) : value_(value), bound_group_(bound_group) {};

        ~IntAdditiveFGroupElement() override final = default;

        operator IntegerType() const override {
            return value_;
        }

        Ptr operator+(const IntAdditiveFGroupElement &a) const override {
            return bound_group_->operateInside(this->value_ + a.value_);
        }

        Ptr inv() const override {
            return bound_group_->inverse(*this);
        }

        bool operator<(const IntAdditiveFGroupElement &a) const override {
            return value_ < a;
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

        IntegerType operator*() const override {
            return value_;
        }

        std::ofstream &print(std::ofstream &ofstream, const Group &group) const override {
            return ofstream << value_;
        }

        operator IntegralType() const override {
            return nullptr;
        }

    protected:
        IntegerType value_;
        GroupPtr bound_group_; // non-owning reference to the group. Be careful with it


    };


    template <typename IntegerType>
    class IntAdditiveGroup : public Group<IntAdditiveFGroupElement<IntegerType>> {
    public:
        typedef Group<IntAdditiveFGroupElement<IntegerType>> Base;
        typedef typename Base::ElemType ElemType;
        typedef typename Base::IntegralType IntegralType;


        IntAdditiveGroup(const IntAdditiveGroup& ) = default;
        IntAdditiveGroup(const IntAdditiveGroup&&) = default;
        IntAdditiveGroup(const IntegerType &modulus) : modulus_(modulus) {}

        ~IntAdditiveGroup() override = default;

        typename ElemType::Ptr operateInside(const IntegralType &a) const override {
            return ElemType::make(a % modulus_);
        }

        typename ElemType::Ptr inverse(const ElemType &type) const override {
            return ElemType::make(modulus_ - *type );
        }

        typename ElemType::Ptr operator[](size_t index) const override {
            // this is kind of a hack - we know, that index of the element
            // is the element itself
            return operateInside(IntegralType(index));
        }

        size_t order() const override {
            return size_t(modulus_);
        }

        bool check() const override {
            throw ::utils::NotImplementedError();
        }

        std::ofstream& print(std::ofstream &ofstream, const Base &group) const override {
            throw ::utils::NotImplementedError();
        }

        FFGroupProperties properties() const override {
            return FFGroupProperties::FGP_INVALID;
        }

    protected:
        const IntegerType& modulus_;

    };


}





#endif //SRC_ALGEBRA_FINITE_IMPL_INTGROUP_H
