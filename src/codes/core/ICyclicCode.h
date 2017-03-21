#pragma once
#include <algebra/Algebra.h>
#include <algebra/Polynomial.h>
#include <vector>
#include <memory>
#include <algebra/utils/utils.h>
#include "ICode.h"
#include "ILinearCode.h"

namespace algebra
{
	namespace codes
	{

        template <size_t Q, size_t QPower>
		class ICyclicCode : public ILinearCode {
		public:
            static constexpr size_t BLOCK_SIZE = static_cast<size_t>(powl(Q, QPower)) - 1;

            virtual ~ICyclicCode() = default;

			virtual const Algebra::Polynomial<Q>* getGenerator() const = 0;
			virtual const Algebra::Polynomial<Q>* getChecking() const = 0;

            virtual size_t blockSize() const override { return BLOCK_SIZE; };

		};
	}

}

