#pragma once
#include "PolynomialGenerator.h"
#include <random>
#include <algorithm>

namespace algebra
{
	
	template <size_t Zp, size_t PolynomialDegree>
	class 	RandomPolynomialGenerator : public PolynomialGenerator<Zp, PolynomialDegree>
	{
		std::random_device device_;
		std::mt19937 engine_ = { std::mt19937(device_()) };
		std::uniform_int_distribution<int> dist = { std::uniform_int_distribution<int>(0, Zp-1) };
		std::vector<int> ivec = { std::vector<int>(PolynomialDegree + 1) };
	public:
		RandomPolynomialGenerator( const PolynomialGenerator<Zp, PolynomialDegree>& copy) = delete;
		RandomPolynomialGenerator()
		{
			ivec[PolynomialDegree] = 1;
		}

		Polynomial<Zp> operator() () override {
			std::generate(ivec.begin(), ivec.begin() + PolynomialDegree, [this]() { return dist(engine_); });
			return ivec;
		};

		bool end() const override {
			return false;
		};
		bool end_supported() const override {
			return false;
		};

		void reset() override
		{
			engine_.seed(engine_.default_seed);
		};

		virtual ~RandomPolynomialGenerator() = default;
	};
}