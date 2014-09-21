#include "biglong.h"
#include <utility>

namespace blong
{
	const biglong biglong::zero = biglong(0);
	const biglong biglong::one = biglong(1);
	const biglong biglong::two = biglong(2);

	biglong::biglong()
	{
		value.push_back(0);
	}

	biglong::biglong(const std::string& hex_string)
	{
		from_string(hex_string);
	}

	biglong::biglong(UNSIGINT num)
	{
		do
		{
			value.push_back(num & REM_MASK);
			num >>= BASE_POWER;
		} while (num > 0);
	}

	biglong::biglong(biglong&& source)
	{
		value = move(source.value);
	}

	biglong::biglong(const unsigned char* raw_bytes, const size_t length)
	{
		size_t digit_size = sizeof(SHORT_UNSIGINT);

		size_t last_position_bound = length%digit_size;
		size_t vector_len = length/digit_size + (last_position_bound ? 1 : 0);
		value.resize(vector_len);
		last_position_bound = last_position_bound ? last_position_bound : digit_size;

		for(size_t i=0; i < vector_len-1; ++i)
		{
			value[i] = 0;
			for(size_t j=0; j < digit_size; ++j)			
				value[i] += static_cast<size_t>(raw_bytes[i*digit_size+j]) << (8*j);			
		}

		value[vector_len-1] = 0;
		for(size_t j=0; j < last_position_bound; ++j)
			value[vector_len-1] += 
				static_cast<size_t>(raw_bytes[(vector_len-1)*digit_size+j]) << (8*j);	
	}

	biglong& biglong::operator=(const std::string& hex_string)
	{
		from_string(hex_string);
		return *this;
	}

	biglong& biglong::operator=(UNSIGINT num)
	{
		value.clear();
		do
		{
			value.push_back(num & REM_MASK);
			num >>= BASE_POWER;
		} while (num > 0);
		return *this;
	}

	biglong& biglong::operator=(biglong&& source)
	{
		value = move(source.value);
		return *this;
	}

	biglong::biglong(size_t reserved_size, SHORT_UNSIGINT default_value) 
		: value(reserved_size, default_value)
	{
	}
}