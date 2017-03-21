#pragma once
#include "IBlockCode.h"


namespace algebra
{
	namespace codes
	{
		class ILinearCode : public IBlockCode
		{
		public:
			virtual ~ILinearCode() = default;
			virtual size_t distance() const = 0;
		};
	}

}
