#pragma once
#include "ICode.h"

namespace algebra
{
	namespace codes
	{
	
		class IBlockCode : ICode
		{
		public:
			virtual ~IBlockCode() = default;
			virtual size_t blockSize() const = 0;
			virtual size_t infoSize() const = 0;


		};

	}
}
