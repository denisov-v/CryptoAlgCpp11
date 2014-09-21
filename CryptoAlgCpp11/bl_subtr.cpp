#include "biglong.h"

namespace blong
{
	//truncated subtraction
	biglong biglong::trunc_sub(const biglong& rhs) const
	{
		if(*this <= rhs)
			return biglong(0);

		 biglong result(value.size(), 0);
	
		SIGINT borrow = 0;
		for (size_t i = 0; i < result.value.size(); ++i)
		{
			SIGINT temp = static_cast<SIGINT>(value[i]) - (i < rhs.value.size() ?
				static_cast<SIGINT>(rhs.value[i]) : 0) + borrow;
			if(temp < 0)
			{
				temp+=BASE;
				borrow = -1;
			}
			else borrow = 0;

			result.value[i] = static_cast<UNSIGINT>(temp);
		}
		result.remove_leading_zeros();

		return result;
	}
}