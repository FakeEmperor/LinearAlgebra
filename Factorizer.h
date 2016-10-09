#pragma once
#include <vector>
#include <ostream>


namespace Algebra
{
	class Factorizer
	{
	protected:
		std::vector<size_t> Primes;

		bool PrimeCheck_(size_t candidate);

	public:
		static const std::vector<size_t> CorePrimes;

		std::vector<std::pair<size_t, size_t>> operator()(size_t number);

		void CalculatePrimesTo(size_t max_possible_prime);

		void FlushPrimes();;


	};

	inline std::ostream& operator << (std::ostream& s, const std::vector<std::pair<size_t, size_t>> &factorized) {
		for (size_t i = 0, sz = factorized.size(); i < sz; ++i)
		{
			if (i != 0)
				s << "*";
			const auto &factor = factorized[i];
			s << factor.first;
			if (factor.second != 1)
				s << "^" << factor.second;
		}
		return s;
	}
}
