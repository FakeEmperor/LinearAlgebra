#pragma once
#include <Algebra.h>
#include <Polynomial.h>
#include <vector>
#include <memory>

namespace algebra
{
	namespace codes
	{
		typedef unsigned char byte;
		typedef std::vector<byte> data;


		struct ICodeData
		{
		public:
			virtual std::vector<byte> * getData() const = 0;
			virtual const std::string&  getType() const = 0;
			virtual void set(const std::string &data) const = 0;
			virtual void set(const data &data) const = 0;
		};

		class ICode
		{
		public:
			virtual ~ICode() = default;

			virtual std::unique_ptr<ICodeData> encode(const data &in) const noexcept(false) = 0;
			virtual std::unique_ptr<ICodeData> decode(const ICodeData *in) const noexcept(false) = 0;
			virtual std::unique_ptr<ICodeData> errors(const ICodeData *in) const noexcept(false) = 0;
		};

		class IBlockCode : ICode
		{
		public:
			virtual ~IBlockCode() = default;
			virtual size_t blockSize() const = 0;
			virtual size_t infoSize() const = 0;
			
			
		};

		class ILinearCode : public IBlockCode
		{
		public:
			virtual ~ILinearCode() = default;
			virtual size_t distance() const = 0;
		};

		class ICyclicCode : public ILinearCode {
		public:
			virtual ~ICyclicCode() = default;

			virtual Algebra::Polynomial<2>* getGenerator() const = 0;
			virtual Algebra::Polynomial<2>* getChecking() const = 0;

		};
	}

}

