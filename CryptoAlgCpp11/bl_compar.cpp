#include "biglong.h"

namespace blong
{
	bool biglong::operator==(const biglong& rhs) const
	{
		if (value.size() != rhs.value.size())
			return false;
		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
			if (*i != *j)
				return false;    
		return true;
	}

	bool biglong::operator!=(const biglong& rhs) const
	{
		if (value.size() != rhs.value.size())
			return true;
		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
			if (*i != *j)
				return true; 
		return false;
	}

	bool biglong::operator<(const biglong& rhs) const
	{
		if (value.size() > rhs.value.size())
			return false;
		if (value.size() < rhs.value.size())
			return true;

		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
		{
			if (*i < *j)
				return true;
			if (*i > *j)
				return false;
		}
		return false;
	}

	bool biglong::operator<=(const biglong& rhs) const
	{
		if (value.size() > rhs.value.size())
			return false;
		if (value.size() < rhs.value.size())
			return true;

		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
		{
			if (*i < *j)
				return true;
			if (*i > *j)
				return false;
		}
		return true;
	}

	bool biglong::operator>(const biglong& rhs) const
	{
		if (value.size() > rhs.value.size())
			return true;
		if (value.size() < rhs.value.size())
			return false;

		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
		{
			if (*i < *j)
				return false;
			if (*i > *j)
				return true;
		}
		return false;
	}

	bool biglong::operator>=(const biglong& rhs) const
	{
		if (value.size() > rhs.value.size())
			return true;
		if (value.size() < rhs.value.size())
			return false;

		for (auto i = value.crbegin(), j = rhs.value.crbegin(); i != value.crend(); ++i, ++j)
		{
			if (*i < *j)
				return false;
			if (*i > *j)
				return true;
		}
		return true;
	}
}