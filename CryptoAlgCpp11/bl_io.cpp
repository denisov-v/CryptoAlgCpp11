#include <iomanip>
#include "biglong.h"

namespace blong
{
	std::istream& operator>>(std::istream &stream, biglong &n)
	{
		std::string str;
		stream >> str;
		n.from_string(str);
		return stream;
	}

	std::ostream& operator<<(std::ostream &stream, const biglong &n)
	{
		stream << std::hex;
		for (auto i = n.value.crbegin(); i != n.value.crend(); ++i)
			stream << std::setfill('0') << std::setw(HEX_DIGITS) << *i;

		return stream;
	}
}