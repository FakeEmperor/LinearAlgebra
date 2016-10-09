#pragma once
#include <vector>
#include <iomanip>
#include <cmath>

#include "PolynomialGenerator.h"
#include "Polynomial.h"
#include "Factorizer.h"


namespace Algebra
{


	template<size_t Zp, size_t Deg>
	// GF(Zp^Deg)
	class GaloisFieldExtension
	{

		Polynomial<Zp> factor_;
		Polynomial<Zp> generator_;
		std::vector<Polynomial<Zp>> elements_ = { Polynomial<Zp>::One };

		static_assert(Deg > 0, "Degree should be greater than zero to build non-trivial field");
	protected:



		GaloisFieldExtension() = delete;

		GaloisFieldExtension(const Polynomial<Zp>& factor, const Polynomial<Zp>& generator);

		static GaloisFieldExtension BuildFactorGroup(const Polynomial<Zp>& factor);
	public:
		static Polynomial<Zp> DefaultGenerator;

		// Most stupid test ever
		static bool RabinTest(const Polynomial<Zp>& poly);

		// Field's order
		size_t order() const;

		// Order of multiplicative gorup of the field
		size_t m_order() const;

		// get element by its order
		Polynomial<Zp> operator[](size_t order) const;

		// find log_generator(poly mod factor)
		size_t log_alpha(const Polynomial<Zp>& poly) const;

		std::vector<size_t> GetAdjointElements(size_t elem_index) const;

		Polynomial<Zp> FindMinimalPolynomial(size_t elem_index) const;

		std::vector<size_t> FindAllPrimitiveElements(size_t order = powl(Zp, Deg)) const;

		// Tests primitivity over finite field in subfield or field itself
		static bool TestPrimitivity(size_t elem_order, size_t order = powl(Zp, Deg));

		static bool TestIrreducibility(const Polynomial<Zp>& poly);

		static Polynomial<Zp> FindIrreducible(PolynomialGenerator<Zp, Deg> &generator)
		{
			Polynomial<Zp> poly = Polynomial<Zp>::Zero;
			std::vector<int> vec(Deg + 1);
			vec[Deg] = 1;
			// every 1-degree Polynomial is irreducible
			if (Deg == 1)
			{
				std::random_device rd;
				std::mt19937 mt(rd());

				vec[0] = randmod([&mt]() { return mt(); }, Zp);
				return Polynomial<Zp>(vec);
			}

			auto end_deterministic = generator.end_supported();
			while (!generator.end() || !end_deterministic)
			{
				Polynomial<Zp> candidate = generator();
				if (TestIrreducibility(candidate))
					return candidate;
			}

			throw std::runtime_error("Could not find an irreducible polynomial. Something wrong with Zp. Zp should be a prime number.");
		}

		// factorizes in field Zp^Deg
		// first - irreducible polynomial, second - its arity
		// Berlekamp's algorithm
		std::vector<std::pair<Polynomial<Zp>, size_t>> FactorizeByFieldElements(const Polynomial<Zp>& poly) const;


		static std::vector<Polynomial<Zp>> FindAllIrreducibles(PolynomialGenerator<Zp, Deg> &generator) noexcept(false)
		{
			std::vector<Polynomial<Zp>> res;
			auto end_deterministic = generator.end_supported();
			if (!end_deterministic)
				throw std::runtime_error("Could not generate all irreducible polynomials with non-deterministic generator.");
			while (!generator.end())
			{
				Polynomial<Zp> candidate = generator();
				if (TestIrreducibility(candidate))
					res.push_back(candidate);
			}

			return res;
		}

		static GaloisFieldExtension Build(PolynomialGenerator<Zp, Deg> &generator)
		{
			return BuildFactorGroup(FindIrreducible(generator));
		}

		static GaloisFieldExtension Build(const Polynomial<Zp>& factor, bool test_irreducibilty = true);

		void PrintPretty(std::ostream& s, bool print_elements = true) const;
	};

	template<size_t Zp, size_t Deg>
	Polynomial<Zp> GaloisFieldExtension<Zp, Deg>::DefaultGenerator = Polynomial<Zp>::X;



	// IMPLEMENTATION

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg>::GaloisFieldExtension(const Polynomial<Zp>& factor, const Polynomial<Zp>& generator): factor_(factor), generator_(generator)
	{
	}

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::BuildFactorGroup(const Polynomial<Zp>& factor)
	{
		auto generator = GaloisFieldExtension::DefaultGenerator;
		GaloisFieldExtension f(factor, generator);
		size_t i = 0;
		auto poly = Polynomial<Zp>::One;
		do
		{
			poly = (generator * f.elements_[i++]) % f.factor_;
			if (poly.deg() != 0)
				f.elements_.push_back(poly);
		}
		while (poly.deg() != 0);
		return f;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::RabinTest(const Polynomial<Zp>& poly)
	{
		auto deg = poly.deg();
		Factorizer factorize;
		auto divisors = factorize(deg);
		for (const auto& divisor : divisors)
		{
			auto nj = deg / divisor.first;
			if (Polynomial<Zp>::Gcd(poly, Polynomial<Zp>::SpecialPolyMod(nj, poly)) != Polynomial<Zp>::One)
				return false;
		}
		return Polynomial<Zp>::SpecialPolyMod(deg, poly) == Polynomial<Zp>::Zero;
	}

	template <size_t Zp, size_t Deg>
	size_t GaloisFieldExtension<Zp, Deg>::order() const
	{
		return elements_.size() + 1;
	}

	template <size_t Zp, size_t Deg>
	size_t GaloisFieldExtension<Zp, Deg>::m_order() const
	{
		return elements_.size();
	}

	template <size_t Zp, size_t Deg>
	Polynomial<Zp> GaloisFieldExtension<Zp, Deg>::operator[](size_t order) const
	{
		return elements_[order];
	}

	template <size_t Zp, size_t Deg>
	size_t GaloisFieldExtension<Zp, Deg>::log_alpha(const Polynomial<Zp>& poly) const
	{
		auto remainder = poly % factor_;
		for (size_t i = 0, sz = elements_.size(); i < sz; ++i)
			if (remainder == elements_[i])
				return i;
		throw std::runtime_error("Could not find logarithm of alpha. This is strange.");
	}

	template <size_t Zp, size_t Deg>
	std::vector<size_t> GaloisFieldExtension<Zp, Deg>::GetAdjointElements(size_t elem_index) const
	{
		std::vector<size_t> indexes;
		size_t last_adjoint = elem_index;
		size_t power = 1, mult_order = m_order();
		do
		{
			power = power * Zp;
			last_adjoint = (elem_index * power) % mult_order;
			if (last_adjoint != elem_index)
				indexes.push_back(last_adjoint);
		}
		while (last_adjoint != elem_index);
		return indexes;
	}

	template <size_t Zp, size_t Deg>
	Polynomial<Zp> GaloisFieldExtension<Zp, Deg>::FindMinimalPolynomial(size_t elem_index) const
	{
		auto adjoints = GetAdjointElements(elem_index);
		auto poly = Polynomial<Zp>::X - elements_[elem_index];
		for (auto adjoint : adjoints)
		{
			poly *= Polynomial<Zp>::X - elements_[adjoint];
		}
		return poly;
	}

	template <size_t Zp, size_t Deg>
	std::vector<size_t> GaloisFieldExtension<Zp, Deg>::FindAllPrimitiveElements(size_t order) const
	{
		std::vector<size_t> primitives;
		// find first primitive with given field order
		size_t primitive = 0;
		for (size_t i = 1, mord = m_order(); i < mord; ++i)
		{
			if (TestPrimitivity(i, order))
			{
				primitive = i;
				break;
			}
		}
		primitives.push_back(primitive);
		// generate remaining - Find all co-primes, lesser than order-1
		for (size_t coprime_candidate = 1; coprime_candidate < order - 1; ++coprime_candidate)
		{
			if (gcd(coprime_candidate, order - 1) == 1)
				primitives.push_back(powmod(primitive, coprime_candidate, order));
		}
		return primitives;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::TestPrimitivity(size_t elem_order, size_t order)
	{
		Factorizer frizer;
		auto mult_order = order - 1;
		auto factors = frizer(mult_order);
		for (auto factor : factors)
		{
			size_t p_i = mult_order / factor.first;
			if (powmod(elem_order, p_i, order) != 1)
				return false;
		}
		return true;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::TestIrreducibility(const Polynomial<Zp>& poly)
	{
		auto deg = poly.deg();
		if (deg <= 1)
			return true;
		for (size_t x = 0; x < Zp; ++x)
			if (!poly.eval(x))
				return false;
		if (deg <= 3)
			return true;
		else
		{
			return RabinTest(poly);
		}

		return true;
	}

	template <size_t Zp, size_t Deg>
	std::vector<std::pair<Polynomial<Zp>, size_t>> GaloisFieldExtension<Zp, Deg>::FactorizeByFieldElements(const Polynomial<Zp>& poly) const
	{
		std::vector<std::pair<Polynomial<Zp>, size_t>> factors;
		// check simple roots
		// TODO: Make it real Berlekamp's algorithm instead of this fucking dumb cycle
		auto cur_poly = poly;
		size_t cdeg = cur_poly.deg();
		for (size_t i = 0, sz = order(); i < sz; ++i)
		{
			auto elem = i ? elements_[i - 1] : Polynomial<Zp>::Zero;
			if (cdeg >= elem.deg() && elem != Polynomial<Zp>::X)
			{
				auto root = Polynomial<Zp>::X - elem;
				if (root != Polynomial<Zp>::One)
				{
					auto div_res = cur_poly.divide(root);
					size_t arity = 0;
					while (div_res.second == Polynomial<Zp>::Zero)
					{
						++arity;
						cur_poly = div_res.first;
						cdeg = cur_poly.deg();
						div_res = cur_poly.divide(root);
					}
					if (arity != 0)
						factors.push_back(std::make_pair(root, arity));
				}
			}
			// factorized successfully
			if (cdeg == 0)
				break;
		}
		if (cdeg != 0)
			factors.push_back(std::make_pair(cur_poly, 1));
		return factors;
	}

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::Build(const Polynomial<Zp>& factor, bool test_irreducibilty)
	{
		if (test_irreducibilty && TestIrreducibility(factor))
			throw std::runtime_error("Given polynomial is reducible");
		return BuildFactorGroup(factor);
	}

	template <size_t Zp, size_t Deg>
	void GaloisFieldExtension<Zp, Deg>::PrintPretty(std::ostream& s, bool print_elements) const
	{
		s << "Field GF(" << Zp << "^" << Deg << ")" << " [ " << this->order() << " elements ]" << std::endl;
		s << "Generator: " << generator_ << std::endl;
		s << "Irreducible polynomial as factor: " << factor_ << std::endl;
		if (print_elements)
		{
			size_t sz = m_order();
			s << "Elements of multiplicative group (" << sz << " elements ): " << std::endl;
			auto space = log10(sz) + 1;
			for (size_t i = 0; i < sz; ++i)
			{
				s << "a^" << std::setw(space) << i << " : " << elements_[i] << std::endl;
			}
		}
	}
}
