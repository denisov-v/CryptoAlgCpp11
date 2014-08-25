#include "biglong.h"

namespace blong
{
	bool biglong::operator==(const biglong& rhs) const
	{
		if (value.size() != rhs.value.size())
			return false;
		for (SIGINT i = value.size() - 1; i >= 0; --i)
			if (value[i] != rhs.value[i])
				return false;    
		return true;
	}

	bool biglong::operator!=(const biglong& rhs) const
	{
		if (value.size() != rhs.value.size())
			return true;
		for (SIGINT i = value.size() - 1; i >= 0; --i)
			if (value[i] != rhs.value[i])
				return true; 
		return false;
	}

	bool biglong::operator<(const biglong& rhs) const
	{
		if (value.size() > rhs.value.size())
			return false;
		if (value.size() < rhs.value.size())
			return true;

		for (SIGINT i = value.size() - 1; i >= 0; --i)
		{
			if (value[i] < rhs.value[i])
				return true;
			if (value[i] > rhs.value[i])
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

		for (SIGINT i = value.size() - 1; i >= 0; --i)
		{
			if (value[i] < rhs.value[i])
				return true;
			if (value[i] > rhs.value[i])
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

		for (SIGINT i = value.size() - 1; i >= 0; --i)
		{
			if (value[i] < rhs.value[i])
				return false;
			if (value[i] > rhs.value[i])
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

		for (SIGINT i = value.size() - 1; i >= 0; --i)
		{
			if (value[i] < rhs.value[i])
				return false;
			if (value[i] > rhs.value[i])
				return true;
		}
		return true;
	}
}