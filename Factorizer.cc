#include "Factorizer.h"

const std::vector<size_t> Algebra::Factorizer::CorePrimes = { 2 };

bool Algebra::Factorizer::PrimeCheck_(size_t candidate)
{
	size_t candidate_root = std::sqrtl(candidate) + 1;
	size_t prime = 0;
	for (size_t i = 0, sz = Primes.size(); i < sz && (prime = Primes[i]) <= candidate_root; ++i)
	{
		if (candidate % prime == 0)
			return false;
	}
	return true;
}

std::vector<std::pair<size_t, size_t>> Algebra::Factorizer::operator()(size_t number)
{
	if (number == 0)
		return {{0, 0}};
	if (number == 1)
		return {{1,0}};

	std::vector<std::pair<size_t, size_t>> factorized;
	if (!Primes.size())
		FlushPrimes();
	if (Primes[Primes.size() - 1] < number)
		CalculatePrimesTo(number);

	auto numbc = number;
	for (auto it = Primes.begin(), ed = Primes.end(); it != ed && numbc != 1; ++it)
	{
		size_t degree = 0, prime = *it;
		while (prime <= numbc && numbc % prime == 0)
			++degree , numbc /= prime;
		if (degree)
			factorized.push_back(std::make_pair(prime, degree));
	}
	return factorized;
}

void Algebra::Factorizer::CalculatePrimesTo(size_t max_possible_prime)
{
	for (size_t sz = Primes.size(), curnumber = Primes[sz - 1] + 1; curnumber <= max_possible_prime; ++curnumber)
	{
		if (PrimeCheck_(curnumber))
			Primes.push_back(curnumber);
	}
}

void Algebra::Factorizer::FlushPrimes()
{
	Primes = CorePrimes;
}
