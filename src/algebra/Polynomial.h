#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <array>
#include <ostream>
#include <cmath>

#include "utils/utils.h"
#include "Algebra.h"


namespace Algebra
{
	
	enum class OUTPUT_MODE
	{
		CANONICAL,
		MINIMAL
	};

	enum class DIVISION_CORRUPTION_POLICY
	{
		THROW,
		CLOSEST_REMAINDER
	};

	template <size_t Zp>
	// FROM LEFT TO RIGHT: 0 0 1 - x^2
	class Polynomial {
    public:
		typedef std::vector<size_t> vec;
    private:
        static_assert(Zp > 1, "Zp cannot be less than 2");


		vec powers_;

		size_t deg_cache_;
	protected:
		// used inside pow() - just dumb * cycle
		Polynomial		rpow			(size_t num)				const;
		// use this to construct without normalization (modulus check)
        Polynomial(const std::vector<size_t>& powers, bool normalization);
		size_t			compute_deg		()							const;

	public:
		static OUTPUT_MODE							OutputMode;
		static DIVISION_CORRUPTION_POLICY			DivisionPolicy;
		static const Polynomial						One;
		static const Polynomial						X;
		static const Polynomial						Zero;


		Polynomial						(const std::initializer_list<int>& l);
		Polynomial						(const vec& powers);
		Polynomial						(const int constant);

		//// CONTROL FUNCTIONS AND OPERATORS

		size_t			size()										const;

		int				operator[]		(size_t idx)				const;
		

		void			assign			(const std::vector<int>& p);
		void			set				(size_t idx, int value);

		size_t			eval			(int x_value)				const;

		size_t			deg				()							const;

		//// ALGEBRAIC FUNCTIONS AND OPERATORS

		Polynomial		derivative		()							const;
		Polynomial		shift_back		(size_t shift)				const;

		// O(n)
		Polynomial		shift(size_t shift)				const;
		/////// ARITHMETIC

		//
		Polynomial&		operator +=		(const Polynomial& p);
		// O(n)
		Polynomial		operator +		(const Polynomial& p)		const;

		Polynomial&		operator -=		(const Polynomial& p);
		Polynomial		operator -		(const Polynomial& p)		const;

		// O(n^2)
		Polynomial&		operator *=		(const Polynomial& p);
		// O(n^2)
		Polynomial		operator *		(const Polynomial& p)		const;
		// Just constant multiplication: O(n)
		Polynomial		operator *		(int number)				const;
		// *-1
		Polynomial		negate()									const;


		std::pair<Polynomial, Polynomial> divide(const Polynomial& p)const;
		Polynomial		operator %		(const Polynomial& p);
		Polynomial&		operator %=		(const Polynomial& p);
		Polynomial		operator /		(const Polynomial& p);
		Polynomial&		operator /=		(const Polynomial& p);

		// O(d*n^2)
		Polynomial		pow				(size_t power)				const;
		

		//////// LOGICAL

		bool			operator ==		(const Polynomial& p)		const;
		bool			operator !=		(const Polynomial& p)		const;

		bool			operator <		(const Polynomial& p)		const;
		bool			operator >		(const Polynomial& p)		const;



		//// OUTPUT

		friend std::ostream& operator << (std::ostream &s, const Polynomial &p) {
			const size_t last_index = p.size() - 1;

			bool printed = false;
			for (size_t i = last_index; i != -1; --i) {
				if (p[i] != 0 || OutputMode == OUTPUT_MODE::CANONICAL)
				{
					// insert operator only AFTER the first (thus, we are sure, that this operator is needed)
					if (printed)
					{
						s << " + ";
					}
					if (i == 0 || p[i] != 1)
						s << p[i];

					if (i == 1) {
						s << "x";
					}
					else if (i > 1) { // i > 1
						s << "x^" << i;
					}
					printed = true;
				}
			}
			if (!printed)
				s << "0";
			return s;
		}

		static Polynomial	Gcd			(const Polynomial& p1, const Polynomial& p2);
		static Polynomial	ExpandedGcd	(Polynomial a, Polynomial b, Polynomial mod);

		// Poly : x^(q^n) - x mod f(x)
		// TODO: Make more sophisticated implmenetation
		static Polynomial SpecialPolyMod(size_t n, const Polynomial& modpoly);
	};

	/////////////// CONSTANT DEFINTION 

	template <size_t Zp>
	OUTPUT_MODE Polynomial<Zp>::OutputMode = OUTPUT_MODE::MINIMAL;

	template <size_t Zp>
	DIVISION_CORRUPTION_POLICY Polynomial<Zp>::DivisionPolicy = DIVISION_CORRUPTION_POLICY::THROW;

	
	template <size_t Zp>
	const Polynomial<Zp> Polynomial<Zp>::One	=	1;
	template <size_t Zp>
	const Polynomial<Zp> Polynomial<Zp>::Zero	=	0;
	template <size_t Zp>
	const Polynomial<Zp> Polynomial<Zp>::X		=	{ 0, 1 };
	

	/////////////// IMPLEMENTATION



	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::rpow(size_t num) const
	{
		if (num == 0)
			return One;
		if (num == 1)
			return *this;

		Polynomial res = One;
		while (num--)
		{
			res *= *this;
		}
		return res;
	}

	template <size_t Zp>
	Polynomial<Zp>::Polynomial(const vec& powers, bool normalization) :
		powers_(normalization ? normalize(powers, Zp) : powers), deg_cache_(compute_deg())
	{
	}

	template <size_t Zp>
	size_t Polynomial<Zp>::compute_deg() const
	{
		for (size_t i = powers_.size() - 1; i != -1; --i)
		{
			if (powers_[i])
				return i;
		}
		return 0;
	}


	template <size_t Zp>
	Polynomial<Zp>::Polynomial(const std::initializer_list<int>& l) : powers_(normalize(l, Zp)), deg_cache_(compute_deg())
	{
	}

	template <size_t Zp>
	Polynomial<Zp>::Polynomial(const vec& powers) :
		Polynomial(powers, true)
	{
	}

	template <size_t Zp>
	Polynomial<Zp>::Polynomial(const int constant):
		powers_({mod(constant, Zp)}), deg_cache_(compute_deg())
	{

	}


	template <size_t Zp>
	int Polynomial<Zp>::operator[](size_t idx) const
	{
		// TODO: allocate memory gracefully
		return powers_[idx];
	}


	template <size_t Zp>
	size_t Polynomial<Zp>::size() const
	{
		return powers_.size();
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::negate() const
	{
		return *this * -1;
	}

	template <size_t Zp>
	Polynomial<Zp>& Polynomial<Zp>::operator-=(const Polynomial& p)
	{
		return *this += p.negate();
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::operator-(const Polynomial& p) const
	{
		Polynomial res = *this;
		return res -= p;
	}

	template <size_t Zp>
	Polynomial<Zp>& Polynomial<Zp>::operator+=(const Polynomial& p)
	{
		size_t maxs = std::max(p.size(), this->size());
		const vec &minv = maxs == p.size() ? powers_ : p.powers_, &maxv = maxs == p.size() ? p.powers_ : powers_;
		size_t mins = minv.size();
		vec resv(maxs);
		for (size_t i = 0; i < maxs; ++i)
		{
			if (i < mins)
				resv[i] = mod((minv[i] + maxv[i]), Zp);
			else
				resv[i] = maxv[i];
		}
		this->powers_ = resv;
		return *this;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::operator+(const Polynomial& p) const
	{
		auto res = *this;
		return res += p;
	}

    template <size_t Zp>
    Polynomial<Zp>& Polynomial<Zp>::operator*=(const Polynomial& p)
    {
        if (*this == Polynomial::One)
            return *this = p;
        if (p == Polynomial::One)
            return *this;

        Polynomial res = { 0 };
        for (size_t idx = 0, s = p.size(); idx < s; ++idx)
        {
            if (p[idx])
            {
                res += (*this * p[idx]).shift(idx);
            }
        }
        *this = res;
        return *this;
    }

    template <size_t Zp>
    Polynomial<Zp> Polynomial<Zp>::operator*(int number) const
    {
        number = mod(number, Zp);
        if (number == 0)
            return{ 0 };
        Polynomial res = *this;
        if (number == 1)
            return res;
        for (auto& p : res.powers_)
        {
            p = mod((p * number), Zp);
        }
        return res;
    }

    template <size_t Zp>
    Polynomial<Zp> Polynomial<Zp>::operator*(const Polynomial& p) const
    {
        Polynomial res = *this;
        return res *= p;
    }

    template <size_t Zp>
    Polynomial<Zp> Polynomial<Zp>::pow(size_t power) const
    {
        if (!power)
            return{ 1 };
        if (power == 1)
            return *this;
        Polynomial res = One, prev = *this;
        size_t prev_power = 1;
        size_t rpower = 1;
        while (rpower != 0)
        {
            if (power & rpower)
            {
                // calculate how many pows to prev
                prev = prev.rpow(rpower / prev_power);
                res *= prev;
                prev_power = rpower;
            }
            rpower <<= 1;
        }
        return res;
    }

    template <size_t Zp>
    std::pair<Polynomial<Zp>, Polynomial<Zp>> Polynomial<Zp>::divide(const Polynomial& p) const
    {
        if (p == Polynomial::Zero)
            throw std::runtime_error("Cannot divide by zero");

        size_t dt = this->deg(), dp = p.deg();
        if (dp > dt)
            return std::make_pair(Polynomial::Zero, *this);
        Polynomial quotient = vec(dt - dp + 1);
        Polynomial remainder = *this;

        auto leading_coefficient = p[dp];
        bool end = false;

        while (dt >= dp && remainder != Polynomial::Zero && !end)
        {
            size_t shift_value = dt - dp; // x^3 / x - shift value will be 2
            size_t coefficient = expanded_gcd(leading_coefficient, remainder[dt], Zp);
            if (coefficient == -1)
            {
                if (DivisionPolicy == DIVISION_CORRUPTION_POLICY::THROW)
                    throw std::runtime_error("Cannot find the coefficients wich will suffice the equation");
                else
                {
                    // try to find a solution
                    coefficient = remainder[dt] / coefficient;
                    if (!coefficient)
                        coefficient = 1;
                    end = true;
                }
            }
            quotient.set(shift_value, coefficient);
            remainder -= (p * coefficient).shift(shift_value);
            dt = remainder.deg();
        }
        return std::make_pair(quotient, remainder);
    }

    template <size_t Zp>
    Polynomial<Zp> Polynomial<Zp>::operator%(const Polynomial& p)
    {
        return this->divide(p).second;
    }

    template <size_t Zp>
    Polynomial<Zp>& Polynomial<Zp>::operator%=(const Polynomial& p)
    {
        *this = *this % p;
        return *this;
    }

    template <size_t Zp>
    Polynomial<Zp> Polynomial<Zp>::operator/(const Polynomial& p)
    {
        return this->divide(p).first;
    }

    template <size_t Zp>
    Polynomial<Zp>& Polynomial<Zp>::operator/=(const Polynomial& p)
    {
        *this = *this / p;
        return *this;
    }
	template <size_t Zp>
	bool Polynomial<Zp>::operator==(const Polynomial& p) const
	{
		size_t dt = this->deg(), dp = p.deg();
		if (dt != dp)
			return false;
		for (size_t i = dt; i != -1; --i)
		{
			if ((*this)[i] != p[i])
				return false;
		}
		return true;
	}

	template <size_t Zp>
	bool Polynomial<Zp>::operator!=(const Polynomial& p) const
	{
		return !((*this) == p);
	}

	template <size_t Zp>
	Polynomial<Zp>& Polynomial<Zp>::operator*=(const Polynomial& p)
	{
		if (*this == Polynomial::One)
			return *this = p;
		if (p == Polynomial::One)
			return *this;

		Polynomial res = { 0 };
		for (size_t idx = 0, s = p.size(); idx < s; ++idx)
		{
			if (p[idx])
			{
				res += (*this * p[idx]).shift(idx);
			}
		}
		*this = res;
		return *this;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::operator*(int number) const
	{
		number = mod(number, Zp);
		if (number == 0)
			return{ 0 };
		Polynomial res = *this;
		if (number == 1)
			return res;
		for (auto& p : res.powers_)
		{
			p = mod((p * number), Zp);
		}
		return res;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::operator*(const Polynomial& p) const
	{
		Polynomial res = *this;
		return res *= p;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::pow(size_t power) const
	{
		if (!power)
			return{ 1 };
		if (power == 1)
			return *this;
		Polynomial res = One, prev = *this;
		size_t prev_power = 1;
		size_t rpower = 1;
		while (rpower != 0)
		{
			if (power & rpower)
			{
				// calculate how many pows to prev
				prev = prev.rpow(rpower / prev_power);
				res *= prev;
				prev_power = rpower;
			}
			rpower <<= 1;
		}
		return res;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::shift(size_t shift) const
	{
		if (shift == 0)
			return *this;
		vec resv(this->size() + shift);
		for (size_t i = 0, s = this->size(); i < s; ++i)
		{
			resv[shift + i] = (*this)[i];
		}
		return Polynomial(resv, false);
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::shift_back(size_t shift) const
	{
		if (shift == 0)
			return *this;
		auto dg = this->deg();
		if (shift > dg)
			return Zero;
		vec resv(dg + 1 - shift);
		for (size_t i = 0, sz = dg - shift; i<=sz; ++i)
		{
			resv[i] = (*this)[i + shift];
		}
		return Polynomial(resv, false);
	}


	template <size_t Zp>
	void Polynomial<Zp>::assign(const std::vector<int>& p)
	{
		powers_ = normalize(p, Zp);
	}

	template <size_t Zp>
	void Polynomial<Zp>::set(size_t idx, int value)
	{
		powers_[idx] = mod(value, Zp);
	}

	template <size_t Zp>
	size_t Polynomial<Zp>::eval(int x_value) const
	{
		x_value = mod(x_value, Zp);
		size_t res = 0;
		for (size_t i = 0, sz = powers_.size(); i < sz; ++i)
		{
			auto p = powers_[i];
			if (p)
				res = mod(res + powmod(x_value, i, Zp) * p, Zp);
		}
		return res;
	}

	template <size_t Zp>
	size_t Polynomial<Zp>::deg() const
	{
		return compute_deg();
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::derivative() const
	{
		Polynomial<Zp> res = this->shift_back(1);
		for (size_t i = res.deg(); i != -1; --i)
		{
			res.set(i, res[i] * (i + 1));
		}
		return res;
	}



	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::Gcd(const Polynomial& p1, const Polynomial& p2)
	{
		const Polynomial *minp = &p1, *maxp = &p2;
		if (p1 > p2)
			std::swap(minp, maxp);
		if (*minp == Polynomial::Zero)
			return p2;
		if (*minp == Polynomial::One)
			return Polynomial::One;
		if (*minp == *maxp)
			return *minp;
		// Oh, well, let's copy
		Polynomial a = *maxp, b = *minp;
		while (b != Polynomial::Zero)
		{
			auto r = a.divide(b).second;
			a = b;
			b = r;
		}

		return a;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::ExpandedGcd(Polynomial a, Polynomial b, Polynomial mod)
	{
		a %= mod;
		b %= mod;
		if (a == Polynomial::One)
			return b;
		auto g = Polynomial::Gcd(a, mod);
		if (g != Polynomial::One)
		{
			if (b % g != Polynomial::Zero)
				throw std::runtime_error("Cannot find a solution");
			if (g != One)
			{
				a /= g;
				b /= g;
				mod /= g;
			}
		}
		std::array<std::array<Polynomial, 4>, 2> matrix = { { { Zero, mod, One, Zero },{ Zero, a, Zero, One } } };
		while (matrix[1][1] != One)
		{
			Polynomial q_i = matrix[0][1] / matrix[1][1]; // q_i = r_i-2 / r_i-1
			std::array<Polynomial, 4> temp = {
				q_i,
				matrix[0][1] % matrix[1][1], // r_i = r_i-2 mod r_i-1
				matrix[0][2] - q_i * matrix[1][2], // x_i = x_i-2 - q_i-1*x_i-1
				matrix[0][3] - q_i * matrix[1][3] // y_i = y_i-2 - q_i-1*y_i-1
			};

			matrix[0] = matrix[1]; // shift up
			matrix[1] = temp;
		}
		return matrix[1][3] * b % mod;
	}

	template <size_t Zp>
	Polynomial<Zp> Polynomial<Zp>::SpecialPolyMod(size_t n, const Polynomial& modpoly)
	{
		size_t deg = algebra::utils::pow(Zp, n);
		std::vector<int> polynomial(deg + 1);
		polynomial[deg] = 1;
		polynomial[1] = -1;
		return Polynomial(polynomial) % modpoly;
	}
}

