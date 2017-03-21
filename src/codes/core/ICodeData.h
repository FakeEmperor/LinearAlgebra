#pragma once
#include <vector>

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
	}
}
