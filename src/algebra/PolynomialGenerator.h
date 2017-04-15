#pragma once
#include "Polynomial.h"

namespace algebra
{
	template <size_t Zp, size_t PolynomialDegree>
	class PolynomialGenerator
	{
	public:
		virtual Polynomial<Zp> operator() () = 0;
		virtual bool end() const = 0;
		virtual bool end_supported() const = 0;
		virtual void reset() = 0;
		virtual ~PolynomialGenerator() = default;
	};
}

