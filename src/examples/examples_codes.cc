#include <codes/impl/ReedMullerCode.h>
#include <algebra/Algebra.h>
#include <algebra/Polynomial.h>
#include <algebra/GaloisFieldExtension.h>
#include <algebra/RandomPolynomialGenerator.h>
#include <iostream>
#include <codes/impl/BCHCode.h>


int main() {
    algebra::RandomPolynomialGenerator<2, 4> gen;
    auto field = algebra::GaloisFieldExtension<2, 4>::Build({1, 1, 0, 0, 1}, {0, 1});
    for (size_t index = 0; index < field.m_order(); ++index)
        std::cout << field.FindMinimalPolynomial(index) << std::endl;
    algebra::codes::impl::BCHCode<2, 4, 2> bchCode;
    bchCode.distance();

	return 0;
}