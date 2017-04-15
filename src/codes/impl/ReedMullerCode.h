#pragma once
#include <codes/core/ICyclicCode.h>
#include <eigen/Eigen>
#include <eigen/Eigenvalues>
#include <utils/utils.h>

namespace algebra::codes::impl
{
    template <size_t Q = 2, size_t QPower = 8, size_t InfoSize = 11>
    //@see: Bleichut R. - Theory and practice of error correcting codes - 76-80pp.
    class ReedMullerCode : public ICyclicCode<Q, QPower>
    {
        static_assert(InfoSize < powl(Q, QPower), "InfoSize cannot be more than Q^Qpower - 1");
        // Remember, that meaning of "bit" here only valid for q = 2.
        // Sizes are calculated for given numeric system
        static constexpr size_t CODE_CHARS_PER_INFO_CHAR = QPower;
    public:
        ReedMullerCode() {

        }

        ~ReedMullerCode() override = default;

        size_t infoSize() const override {
            return CODE_CHARS_PER_INFO_CHAR * InfoSize;
        }

        std::unique_ptr<ICodeData> encode(const data &in) const override {
            return nullptr;
        }

        std::unique_ptr<ICodeData> decode(const ICodeData *in) const override {
            return nullptr;
        }

        std::unique_ptr<ICodeData> errors(const ICodeData *in) const override {
            return nullptr;
        }

        const Polynomial<Q> *getGenerator() const override {
            return nullptr;
        }

        const Polynomial<Q> *getChecking() const override {
            return nullptr;
        }

        size_t distance() const override {
            return ReedMullerCode::BLOCK_SIZE - InfoSize;
        }

        static std::unique_ptr<algebra::Polynomial<Q>> MakeBCHGenerator() {
            // algebra::Polynomial<Q>::ExpandedGcd()
            throw ::utils::NotImplementedError();
        }
    private:

    }; // ::ReedMullerCode
} //algebra::

