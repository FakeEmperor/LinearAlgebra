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
    /**
     * @param MinErrors - constructive distance
     **/
    template <size_t Q = 2, size_t QPower = 2, size_t MinErrors = utils::cpow(Q, QPower - 1) -1>
    class BCHCode: public ICyclicCode<Q, QPower> {
        static_assert(MinErrors < (utils::cpow(Q, QPower) - 1) / 2,
                      "'MinErrors' cannot be more than a half of block size (Q^QPower - 1)");
    private:

        std::unique_ptr<algebra::Polynomial<Q>> g_, h_;
        bool got_h_ = { false };

    public:
        // default constructor
        // All necessary information (g(x), primitive element for field, modulus)
        // will be generated randomly using default
        // generation algorithms
        BCHCode() : g_(std::make_unique<algebra::Polynomial<Q>>(BCHCode::MakeGenerator())) {

        }

        virtual ~BCHCode() = default;

        const Polynomial<Q> *getGenerator() const override {
            return g_.get();
        }

        const Polynomial<Q> *getChecking() const override {
            if (got_h_)
                return h_.get();
            else
                throw std::runtime_error("Not implemented");
        }

        static Polynomial<Q> MakeGenerator(algebra::GaloisFieldExtension<Q, QPower> &field) {
            Polynomial<Q> gen_poly = 1;
            for (size_t i = 0; i < 2*MinErrors; ++i)
            {
                auto min = field.FindMinimalPolynomial(i);
                gen_poly *= min;
                gen_poly /= min.Gcd(min, gen_poly);
            }
            //return field.FindMinimalPolynomial(decltype(field)::Poly::X);
            return gen_poly;
        }

        static Polynomial<Q> MakeGenerator() {
            // make field
            algebra::RandomPolynomialGenerator<Q, QPower> gen;
            auto field = algebra::GaloisFieldExtension<Q, QPower>::Build(gen);
            return MakeGenerator(field);
        }

        size_t distance() const override {
            return MinErrors*2;
        }

        size_t infoSize() const override {
            throw ::utils::NotImplementedError("BCHCode::infoSize()");
        }

        size_t blockSize() const override {
            throw ::utils::NotImplementedError("BCHCode::blockSize()");
        }


        std::unique_ptr<ICodeData> encode(const data &in) const override {
            throw ::utils::NotImplementedError();
        }

        std::unique_ptr<ICodeData> decode(const ICodeData *in) const override {
            throw ::utils::NotImplementedError();
        }

        std::unique_ptr<ICodeData> errors(const ICodeData *in) const override {
            return nullptr;
        }


    };

    };

