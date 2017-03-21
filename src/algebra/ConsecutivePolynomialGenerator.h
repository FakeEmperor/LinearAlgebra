#pragma once
#include "PolynomialGenerator.h"


namespace Algebra
{
	// Generates canonical polynomials of degree PolynomialDegree consecutively (if degree is 2, and Zp=2: x^2, x^2+1, x^2+x, x^2+x+1). 
	// Will cycle after generating all possible Polynomials, setting end_reached = true, once reached
	template <size_t Zp, size_t PolynomialDegree>
	class ConsecutivePolynomialGenerator : public PolynomialGenerator<Zp, PolynomialDegree>
	{
		Polynomial<Zp> last_polynomial_ = {};
		bool end_reached_ = { false };
	public:
		virtual Polynomial<Zp> operator()() override;

		virtual bool end() const override;
		virtual bool end_supported() const override;

		virtual void reset() override;

		virtual ~ConsecutivePolynomialGenerator() = default;
	};


	// IMPLEMENTATION


	template <size_t Zp, size_t PolynomialDegree>
	Polynomial<Zp> ConsecutivePolynomialGenerator<Zp, PolynomialDegree>::operator()()
	{
		end_reached_ = false;
		if (last_polynomial_.size() == 0)
		{
			reset();
			return last_polynomial_;
		}
		bool move = false;
		size_t cindex = 0;
		do
		{
			move = false;
			if (cindex == PolynomialDegree)
				end_reached_ = true;
			else
			{
				last_polynomial_.set(cindex, last_polynomial_[cindex] + 1);
				if (last_polynomial_[cindex] == 0)
				{
					move = true;
					++cindex;
				}
			}
		}
		while (move);

		return last_polynomial_;
	}

	template <size_t Zp, size_t PolynomialDegree>
	bool ConsecutivePolynomialGenerator<Zp, PolynomialDegree>::end() const
	{
		return end_reached_;
	}

	template <size_t Zp, size_t PolynomialDegree>
	bool ConsecutivePolynomialGenerator<Zp, PolynomialDegree>::end_supported() const
	{
		return true;
	}

	template <size_t Zp, size_t PolynomialDegree>
	void ConsecutivePolynomialGenerator<Zp, PolynomialDegree>::reset()
	{
		end_reached_ = false;
		last_polynomial_ = std::vector<int>(PolynomialDegree + 1);
		last_polynomial_.set(PolynomialDegree, 1);
	}
}
