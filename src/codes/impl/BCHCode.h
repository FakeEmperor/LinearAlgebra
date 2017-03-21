//
// Created by lol on 3/15/2017.
//
#pragma once

#include <codes/core/ICyclicCode.h>
#include <algebra/utils/utils.h>
#include <algebra/PolynomialGenerator.h>
#include <algebra/RandomPolynomialGenerator.h>
#include <algebra/GaloisFieldExtension.h>

namespace algebra::codes::impl
{
    template <size_t Q = 2, size_t QPower = 2, size_t MinErrors>
    class BCHCode: public ICyclicCode<Q, QPower> {
        static_assert(MinErrors < (utils::pow(Q, QPower) - 1) / 2,
                      "'MinErrors' cannot be more than a half of block size (Q^QPower - 1)");
    private:
        std::unique_ptr<Algebra::Polynomial<Q>> g_, h_;
        bool got_h_ = { false };

    public:
        // default constructor
        // All necessary information (g(x), primitive element for field, modulus)
        // will be generated randomly using default
        // generation algorithms
        BCHCode() : g_(std::make_unique(BCHCode::MakeGenerator())) {

        }



        Algebra::Polynomial<Q> *getGenerator() const override {
            return g_.get();
        }

        Algebra::Polynomial<Q> *getChecking() const override {
            if (got_h_)
                return h_.get();
            else
                throw std::runtime_error("Not implemented");
        }

        Algebra::Polynomial<Q> MakeGenerator() const {
            // make field
            auto gen = Algebra::RandomPolynomialGenerator<Q, QPower>();
            auto field = Algebra::GaloisFieldExtension<Q, QPower>::Build(gen);
            return field.FindMinimalPolynomial(0);
        }

        size_t distance() const override {
            return MinErrors*2;
        }

    };



}