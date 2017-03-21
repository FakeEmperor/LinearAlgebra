#pragma once
#include <memory>
#include "ICodeData.h"

namespace algebra
{
	namespace codes
	{
		class ICode
		{
		public:
			virtual ~ICode() = default;

			virtual std::unique_ptr<ICodeData> encode(const data &in) const noexcept(false) = 0;
			virtual std::unique_ptr<ICodeData> decode(const ICodeData *in) const noexcept(false) = 0;
			virtual std::unique_ptr<ICodeData> errors(const ICodeData *in) const noexcept(false) = 0;
		};

	}
}
