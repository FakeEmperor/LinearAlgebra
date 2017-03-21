#include "Algebra.h"

explicit size_t Algebra::mod(int a, size_t Zp)
{
	if (a < 0)
		return Zp - (-a) % Zp;
	else
		return a % Zp;
}

size_t Algebra::mod(size_t a, size_t Zp) {
    return a % Zp;
}


std::vector<size_t> Algebra::normalize(const std::vector<int>& vec, size_t Zp)
{
	std::vector<size_t> res(vec.size());
	for (size_t i = 0, s = vec.size(); i<s; ++i)
		res[i] = mod(vec[i], Zp);
	return res;
}

size_t Algebra::randmod(std::function<int()> generator, size_t Zp)
{
	return mod(generator(), Zp);
}

size_t Algebra::powmod(int a, size_t pow, size_t Zp)
{
	size_t res = 1;
	while (pow--)
	{
		res = mod((int) (res * a), Zp);
	}

	return res;
}

size_t Algebra::gcd(size_t a, size_t b)
{
	if (a < b)
		std::swap(a, b);
	if (b == 0)
		return a;
	if (b == 1)
		return 1;
	if (a == b)
		return a;

	while (b != 0)
	{
		auto r = a % b;
		a = b;
		b = r;
	}

	return a;
}

size_t Algebra::expanded_gcd(size_t a, size_t b, size_t Zp)
{
	a = mod(a, Zp);
	b = mod(b, Zp);
	if (a == 1)
		return b;
	auto g = gcd(a, Zp);
	if (g != 1)
	{
		if (b % g)
			return (size_t) -1;
		if (g != 1) // BUG: Fix this condition (always true)
		{
			a /= g;
			b /= g;
			Zp /= g;
		}
	}
	std::array<std::array<int, 4>, 2> matrix = {{{0, Zp, 1, 0},{0, a, 0, 1}}};
	while (matrix[1][1] != 1)
	{
		int q_i = matrix[0][1] / matrix[1][1]; // q_i = r_i-2 / r_i-1
		std::array<int, 4> temp = {
			q_i,
			matrix[0][1] % matrix[1][1], // r_i = r_i-2 mod r_i-1
			matrix[0][2] - q_i * matrix[1][2], // x_i = x_i-2 - q_i-1*x_i-1
			matrix[0][3] - q_i * matrix[1][3] // y_i = y_i-2 - q_i-1*y_i-1
		};

		matrix[0] = matrix[1]; // shift up
		matrix[1] = temp;
	}
	return mod(matrix[1][3] * b, Zp);
}

size_t Algebra::inverse_mod(size_t a, size_t Zp)
{
	return expanded_gcd(a, 1, Zp);
}
