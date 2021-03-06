#include <iostream>
#include <vector>

#include "Factorizer.h"
#include "Polynomial.h"
#include "GaloisFieldExtension.h"
#include "ConsecutivePolynomialGenerator.h"
#include "RandomPolynomialGenerator.h"


using namespace Algebra;

int main() {
	Polynomial<5> p1 = { 2, 1, 2, -1, -1, -1, -2, -3, -4, 1, 1, 2, 3, 4, 8 };
	Polynomial<5> p2 = { 4, 3 };
	const size_t pw = 100;
	std::cout << p1 << std::endl;

	std::boolalpha(std::cout);
	std::cout << p1 * p2 << std::endl;
	
	auto pw1 = p1.pow(pw);
	std::cout << "Pow1: " << pw1 << std::endl;
	std::cout << p1 - p2 << std::endl;

	Factorizer f;
	size_t number = 1024;
	std::cout << number << " = "<< f(number) << std::endl;

	size_t a = 15, b = 6, md = 18, e = expanded_gcd(a, b, md);
	std::cout << a << "*x = " << b << " mod " << md << std::endl
				<< "x = " << e << "? " << ((a*e) % md == b) << std::endl;

	Polynomial<7> 
		pa = { 1,2,8,4,5,0,0 },
		pb = { 7,4,6,5,1 };
	
	
	auto pdiv = pa.divide(pb);
	std::cout << pa << " / " << pb << " = " << pdiv.first << " [remainder: " << pdiv.second << "]" << std::endl;
	

	auto g = Polynomial<7>::Gcd(pa, pb), exp_g = Polynomial<7>::ExpandedGcd(pa, decltype(pa)::One, pb);
	std::cout << "GCD(" << pa << " , " << pb << ") = " << g << std::endl;
	pdiv = pa.divide(g);
	std::cout << pa << " / " << g << " = " << pdiv.first << " [remainder: " << pdiv.second << "]" << std::endl;
	pdiv = pb.divide(g);
	std::cout << pb << " / " << g << " = " << pdiv.first << " [remainder: " << pdiv.second << "]" << std::endl;

	std::cout << pa << " * X = " << 1 << " mod " << pb << ". X = " << exp_g << std::endl;
	std::cout << "Really? " << pa << " * " << exp_g << " = " << (pa * exp_g) % pb << " mod " << pb << std::endl;

	constexpr const size_t Zp = 2, degree = 8;
	ConsecutivePolynomialGenerator<Zp, degree> gen;
	auto all_irrs = GaloisFieldExtension<Zp, degree>::FindAllIrreducibles(gen);
	gen.reset();
	std::cout << "Irreducible over Z" << Zp << " with degree " << degree << " : " << GaloisFieldExtension<Zp, degree>::FindIrreducible(gen) << std::endl;
	std::cout << "All irreducibles over Z" << Zp << " with degree " << degree << "(" << all_irrs.size() << ") : " << std::endl;
	for (auto ir : all_irrs)
		std::cout << ir << std::endl;
	// Build F(2^6 = 64)
	ConsecutivePolynomialGenerator<Zp, degree> generator;
	GaloisFieldExtension<Zp, degree> gf = GaloisFieldExtension<Zp, degree>::Build(generator);
	gf.PrintPretty(std::cout);

	// FactorizeByFieldElements something
	Polynomial<2> to_factor = { 0, 0, 1, 0, 1, 0, 1, 1, 1,1, 0 , 1 }; // x^4 + x^2 + x + 1
	auto factorized_poly = gf.FactorizeByFieldElements(to_factor);
	std::cout << "Factorization of " << to_factor << " over F(" << Zp << "^" << degree << ")" << " = ";
	bool printed = false;
	for (auto factor: factorized_poly)
	{
		if (printed)
			std::cout << "*";
		std::cout << "( " << factor.first << " )";
		if (factor.second != 1)
			std::cout << "^" << factor.second;
		printed = true;
	}
	std::cout << std::endl;
	RandomPolynomialGenerator<Zp, degree> rgen;
	std::cout << "RANDOM Irreducible over Z" << Zp << " with degree " << degree << " : " << GaloisFieldExtension<Zp, degree>::FindIrreducible(rgen) << std::endl;


	return 0;
};