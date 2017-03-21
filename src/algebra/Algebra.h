#pragma once
#include <array>
#include <vector>
#include <functional>

namespace Algebra
{
	size_t explicit mod(int a, size_t Zp);
	size_t mod(size_t a, size_t Zp);

	std::vector<size_t> normalize(const std::vector<int>& vec, size_t Zp);

	size_t randmod(std::function<int()> generator, size_t Zp);

	size_t powmod(int a, size_t pow, size_t Zp);


	// GCD(a, b) = GCD(b, b % a)
	size_t gcd(size_t a, size_t b);

	/**
	Solves equation: a*x = b mod Zp
	@return -1 if cannot solve
	*/
	size_t expanded_gcd(size_t a, size_t b, size_t Zp);

	size_t inverse_mod(size_t a, size_t Zp);
}
