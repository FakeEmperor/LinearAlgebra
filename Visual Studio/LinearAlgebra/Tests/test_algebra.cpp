#include "catch.hpp"
#include <Algebra.h>
#include <Polynomial.h>
#include <GaloisFieldExtension.h>
#include "ConsecutivePolynomialGenerator.h"

TEST_CASE("Algebra polynomial division is working", "[algebra][polynomial]")
{
	std::vector<int>
		m1_x = { 1, 0, 0, 0, 1 },
		m2_x = { 1, 1 },
		division_result = { 1, 1, 1, 1 };
	Algebra::Polynomial<2> p1(m1_x), p2(m2_x), pres(division_result);
	
	REQUIRE(p1 / p2 == pres);
	

}

TEST_CASE("Algebra Field building is working", "[algebra][field]")
{
	constexpr size_t degree = 6;
	Algebra::ConsecutivePolynomialGenerator<2, degree> gen;
	auto field = Algebra::GaloisFieldExtension<2, degree>::Build(gen);
	std::set<Algebra::Polynomial<2>> sets;
	for (int i = 0, s = field.order(); i<s; ++i)
	{
		sets.emplace(field[i]);
	}
	REQUIRE(sets.size() == (size_t)pow(2, degree));


}