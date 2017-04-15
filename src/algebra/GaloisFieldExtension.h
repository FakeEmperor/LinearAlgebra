#pragma once
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>

#include "Algebra.h"
#include "PolynomialGenerator.h"
#include "Polynomial.h"
#include "Factorizer.h"

#include <utils/utils.h>

namespace algebra
{


	template<size_t Zp, size_t Deg>
	// GF(Zp^Deg)
	class GaloisFieldExtension
	{
	public:
		typedef Polynomial<Zp> Poly;
	private:
		Poly factor_;
		Poly generator_;
		std::vector<Poly> elements_ = { Poly::One };

		static_assert(Deg > 0, "Degree should be greater than zero to build non-trivial field");
	protected:



		GaloisFieldExtension() = delete;

		GaloisFieldExtension(const Poly& factor, const Poly& primitive);

		static GaloisFieldExtension BuildMultGroup(const Poly& factor, const Poly &primitive);
	public:
		static Poly DefaultPrimitive;
		

		// Most stupid test ever
		static bool RabinTest(const Poly& poly);

		// Field's order
		size_t order() const;

		// Order of multiplicative gorup of the field
		size_t m_order() const;

		// get element by its order
		Poly operator[](size_t m_order) const;

		// find log_generator(poly mod factor)
		size_t log_alpha(const Poly& poly) const;

		std::vector<size_t> GetAdjointElements(size_t elem_index) const;

		Poly FindMinimalPolynomial(size_t elem_index) const;

		std::vector<size_t> FindAllPrimitiveElements(size_t order = powl(Zp, Deg)) const;

		// Tests primitivity over finite field in subfield or field itself
		static bool TestPrimitivity(size_t elem_order, size_t order = powl(Zp, Deg));

		static bool TestPrimitivity(const Poly &poly, const Poly &modulus);

		static bool TestIrreducibility(const Poly& poly);

		static Poly FindPrimitive(PolynomialGenerator<Zp, Deg> &generator, Poly factor)
		{
			auto primitive = GaloisFieldExtension::DefaultPrimitive;
			if (!TestPrimitivity(primitive, factor))
			{
				generator.reset();
				do
				{
					primitive = generator();
					primitive.set(Deg, 0);
				} while (!TestPrimitivity(primitive, factor));
				
			}
			return primitive;
		}

		static Poly FindIrreducible(PolynomialGenerator<Zp, Deg> &generator)
		{
			Poly poly = Poly::Zero;
			typename Poly::vec vec(Deg + 1);
			vec[Deg] = 1;
			// every 1-degree Polynomial is irreducible
			if (Deg == 1) // select x or x+rand(Zp)
			{
                std::random_device rd;
				std::mt19937 mt(rd());

				vec[0] = randmod([&mt]() { return mt(); }, Zp);
				return Poly(vec);
			}

			auto end_deterministic = generator.end_supported();
			while (!generator.end() || !end_deterministic)
			{
				Poly candidate = generator();
				if (TestIrreducibility(candidate))
					return candidate;
			}

			throw std::runtime_error("Could not find an irreducible polynomial. Something wrong with Zp. Zp should be a prime number.");
		}

		// factorizes in field Zp^Deg
		// first - irreducible polynomial, second - its arity
		// Berlekamp's algorithm
		std::vector<std::pair<Poly, size_t>> FactorizeByFieldElements(const Poly& poly) const;


		static std::vector<Poly> FindAllIrreducibles(PolynomialGenerator<Zp, Deg>& generator) noexcept(false)
		{
			std::vector<Poly> res;
			auto end_deterministic = generator.end_supported();
			if (!end_deterministic)
				throw std::runtime_error("Could not generate all irreducible polynomials with non-deterministic generator.");
			while (!generator.end())
			{
				Poly candidate = generator();
				if (TestIrreducibility(candidate))
					res.push_back(candidate);
			}

			return res;
		}

		static GaloisFieldExtension Build(PolynomialGenerator<Zp, Deg> &generator)
		{
			auto gen = FindIrreducible(generator);
			return BuildMultGroup(gen, FindPrimitive(generator, gen ));
		}

		static GaloisFieldExtension Build(const Poly& factor,
                                          const Poly& primitive=GaloisFieldExtension::DefaultPrimitive,
                                          bool test_irreducibilty = true, bool test_primitivity = true);
        static GaloisFieldExtension BuildByPrimitive(const Poly& primitive, PolynomialGenerator<Zp, Deg>& generator,
                                                     bool test_primitivity = true);

        static GaloisFieldExtension BuildByPrimitive(const Poly& factor, const Poly& primtivie,
                                                     bool test_irreducibility = true, bool test_primitivity = true);

		void PrintPretty(std::ostream& s, bool print_elements = true) const;
	};

	template<size_t Zp, size_t Deg>
	Polynomial<Zp> GaloisFieldExtension<Zp, Deg>::DefaultPrimitive = { 0, 1 };



	// IMPLEMENTATION

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg>::GaloisFieldExtension(const Poly& factor, const Poly& generator): factor_(factor), generator_(generator)
	{
	}

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::BuildMultGroup(const Poly& factor, const Poly &primitive)
	{
		GaloisFieldExtension f(factor, primitive);		
		size_t i = 0;
		auto poly = Poly::One;
		do
		{
			poly = (primitive * f.elements_[i++]) % f.factor_;
			if (poly != Poly::One)
				f.elements_.push_back(poly);
		}
		while (poly != Poly::One);
		return f;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::RabinTest(const Poly& poly)
	{
		auto deg = poly.deg();
		Factorizer factorize;
		auto divisors = factorize(deg);
		for (const auto& divisor : divisors)
		{
			auto nj = deg / divisor.first;
			if (Poly::Gcd(poly, Poly::SpecialPolyMod(nj, poly)) != Poly::One)
				return false;
		}
		return Poly::SpecialPolyMod(deg, poly) == Poly::Zero;
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
	Polynomial<Zp> GaloisFieldExtension<Zp, Deg>::operator[](size_t m_order) const
	{
		return elements_[m_order];
	}

	template <size_t Zp, size_t Deg>
	size_t GaloisFieldExtension<Zp, Deg>::log_alpha(const Poly& poly) const
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
		auto poly = Poly::X - elements_[elem_index];
		for (auto adjoint : adjoints)
		{
			poly *= Poly::X - elements_[adjoint];
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
			if (powmod(elem_order, p_i, order) == 1)
				return false;
		}
		return true;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::TestPrimitivity(const Poly &poly, const Poly &modulus)
	{
		if (poly == 0)
			return false;
		if (poly.deg() >= modulus.deg())
			throw std::runtime_error("Generator cannot have degree more than factor's");
		Factorizer frizer;
		size_t mult_order = powl(Zp, Deg) - 1;
		auto factors = frizer(mult_order);
		for(auto factor : factors)
		{	
			auto p_i = mult_order / factor.first;
			if (poly.pow(p_i) % modulus == Poly::One)
				return false;
		}
		return true;
	}

	template <size_t Zp, size_t Deg>
	bool GaloisFieldExtension<Zp, Deg>::TestIrreducibility(const Poly& poly)
	{
		auto deg = poly.deg();
		if (deg == 1)
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
	}

	template <size_t Zp, size_t Deg>
	std::vector<std::pair<algebra::Polynomial<Zp>, size_t>> GaloisFieldExtension<Zp, Deg>::FactorizeByFieldElements(const Poly& poly) const
	{
		std::vector<std::pair<Poly, size_t>> factors;
		// check simple roots
		// TODO: Make it real Berlekamp's algorithm instead of this fucking dumb cycle
		auto cur_poly = poly;
		size_t cdeg = cur_poly.deg();
		// cdeg is 0 (constant) => fully factorized
		for (size_t i = 0, sz = order(); i < sz && cdeg != 0; ++i)
		{
			auto elem = i ? elements_[i - 1] : Poly::Zero;
			if (cdeg >= elem.deg() && elem != Poly::X)
			{
				auto root = Poly::X - elem;
				if (root != Poly::One)
				{
					auto div_res = cur_poly.divide(root);
					size_t arity = 0;
					while (div_res.second == Poly::Zero)
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
		}
		if (cdeg != 0)
			factors.push_back(std::make_pair(cur_poly, 1));
		return factors;
	}

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::Build(const Poly& factor, const Poly& generator,
                                                                       bool test_irreducibilty, bool test_primitivity)
	{
		if (factor.deg() < Deg)
			throw std::runtime_error("Given polynomial has lesser degree than degree of the field");
		if (test_irreducibilty && !TestIrreducibility(factor))
			throw std::runtime_error("Given polynomial is reducible");
		if (!TestPrimitivity(generator, factor))
			throw std::runtime_error("Give generator is non-primitive");
		return BuildMultGroup(factor, generator);
	}

	template <size_t Zp, size_t Deg>
	GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::BuildByPrimitive(const Poly& factor, const Poly& primitive,
                                                                                  bool test_irreducibility, bool test_primitivity)
	{
		if (factor.deg() < Deg)
			throw std::runtime_error("Given polynomial has lesser degree than degree of the field");
		if (test_irreducibility && !TestIrreducibility(factor))
			throw std::runtime_error("Given polynomial is reducible");
        //make modulus
        if (test_primitivity && !TestPrimitivity(primitive, factor))
			throw std::runtime_error("Given generator is non-primitive");
		return BuildMultGroup(factor, primitive);
	}
    template <size_t Zp, size_t Deg>
    GaloisFieldExtension<Zp, Deg> GaloisFieldExtension<Zp, Deg>::BuildByPrimitive(const Poly& primitive,
                                                                                  PolynomialGenerator<Zp, Deg>& gen,
                                                                                  bool test_primitivity)
    {
        return BuildByPrimitive(GaloisFieldExtension::FindIrreducible(gen), primitive, false, test_primitivity);
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
			int space = (int) (log10(sz) + 1);
			for (size_t i = 0; i < sz; ++i)
			{
				s << "a^" << std::setw(space) << i << " : " << elements_[i] << std::endl;
			}
		}
	}
}
