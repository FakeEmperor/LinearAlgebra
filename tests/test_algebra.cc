#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <algebra/Algebra.h>
#include <algebra/Polynomial.h>
#include <algebra/GaloisFieldExtension.h>
#include <algebra/ConsecutivePolynomialGenerator.h>
#include <algebra/common/combinatorics.h>

TEST_CASE("Algebra polynomial division is working",
          "[algebra][polynomial]")
{
	std::vector<size_t>
		m1_x = { 1, 0, 0, 0, 1 },
		m2_x = { 1, 1 },
		division_result = { 1, 1, 1, 1 };
	algebra::Polynomial<2> p1(m1_x), p2(m2_x), pres(division_result);
	
	REQUIRE(p1 / p2 == pres);
}

TEST_CASE("Algebra Field building is working",
          "[algebra][field]")
{
	constexpr size_t degree = 6;
	algebra::ConsecutivePolynomialGenerator<2, degree> gen;
	auto field = algebra::GaloisFieldExtension<2, degree>::Build(gen);
	std::set<algebra::Polynomial<2>> sets;
	for (int i = 0, s = field.m_order(); i<s; ++i)
	{
		sets.emplace(field[i]);
	}
	REQUIRE(sets.size() == (size_t)pow(2, degree));


}

TEST_CASE("[+Test] algebra::common::combinatorics::IndexPlacementsWithoutRepetitions()",
          "[algebra][common][combinatorics]")
{
    SECTION("C(0,0) == {empty response}") {
        auto res = algebra::common::combinatorics::IndexPlacementsWithoutRepetitions(0,0);
        REQUIRE_THAT(res, Catch::Matchers::Equals(decltype(res)()));
    }
}