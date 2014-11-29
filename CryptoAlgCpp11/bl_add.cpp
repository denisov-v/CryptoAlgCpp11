#include "biglong.h"

namespace blong
{
	biglong& biglong::operator++()
	{
		++value[0];
		auto last_index = value.end() - value.begin() - 1;
		for(auto i = value.begin(); i != value.end() && *i >= BASE; ++i)
		{
			*i = 0;
			if(i - value.begin() < last_index)
				++(*(i+1));
			else 
			{
				value.push_back(1);
				break;
			}
		}
		return *this;
	}

	biglong biglong::operator+(const biglong& rhs) const
	{
		size_t max_size = std::max(value.size(), rhs.value.size());
		biglong result(max_size, 0);
	
		UNSIGINT carry_prev = 0;
		for (size_t i = 0; i < result.value.size(); ++i)
		{
			result.value[i] = (i < value.size() ? value[i] : 0) + 
				(i < rhs.value.size() ? rhs.value[i] : 0) + carry_prev;
			
			if (result.value[i] >= BASE)
			{
				carry_prev = result.value[i] >> BASE_POWER;
				result.value[i] &= REM_MASK;	
			}
			else
			{
				carry_prev = 0;
			}
		}
		if(carry_prev != 0)
			result.value.push_back(carry_prev);

		return result;
	}

	biglong biglong::operator+(UNSIGINT rhs) const
	{
		biglong right_long = biglong(rhs);
		return (*this)+right_long;
	}

	biglong& biglong::operator+=(const biglong& rhs)
	{
		size_t max_size = value.size() > rhs.value.size() ? value.size() : rhs.value.size();
		value.resize(max_size, 0);

		UNSIGINT carry_prev = 0;
		for (size_t i = 0; i < rhs.value.size(); ++i)
		{
			value[i] += rhs.value[i] + carry_prev;

			if (value[i] >= BASE)
			{
				carry_prev = value[i] >> BASE_POWER;
				value[i] &= REM_MASK;
			}
			else
			{
				carry_prev = 0;
			}
		}
		
		for (size_t i = rhs.value.size(); carry_prev != 0 && i < value.size(); ++i)
		{
			value[i] += carry_prev;

			if (value[i] >= BASE)
			{
				carry_prev = value[i] >> BASE_POWER;
				value[i] &= REM_MASK;
			}
			else carry_prev = 0;
		}
		if (carry_prev != 0)
			value.push_back(carry_prev);
		
		return *this;
	}
}