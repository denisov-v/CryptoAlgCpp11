#include "baseconvert.h"

namespace baseconvert
{
	const char* BaseConverter::binarySet_ = "01";
	const char* BaseConverter::decimalSet_ = "0123456789";
	const char* BaseConverter::hexSet_ = "0123456789abcdef";

	BaseConverter::BaseConverter(const std::string& sourceBaseSet, const std::string& targetBaseSet)
		: sourceBaseSet_(sourceBaseSet)
		, targetBaseSet_(targetBaseSet)
	{
		if (sourceBaseSet.empty() || targetBaseSet.empty())
			throw std::invalid_argument("Invalid base character set");
	}

	const BaseConverter& BaseConverter::DecimalToBinaryConverter()
	{
		static const BaseConverter dec2bin(decimalSet_, binarySet_);
		return dec2bin;
	}

	const BaseConverter& BaseConverter::BinaryToDecimalConverter()
	{
		static const BaseConverter bin2dec(binarySet_, decimalSet_);
		return bin2dec;
	}

	const BaseConverter& BaseConverter::DecimalToHexConverter()
	{
		static const BaseConverter dec2hex(decimalSet_, hexSet_);
		return dec2hex;
	}

	const BaseConverter& BaseConverter::HexToDecimalConverter()
	{
		static const BaseConverter hex2dec(hexSet_, decimalSet_);
		return hex2dec;
	}

	std::string BaseConverter::Convert(std::string value) const
	{
		unsigned int numberBase = GetTargetBase();
		std::string result;

		do
		{
			unsigned int remainder = divide(sourceBaseSet_, value, numberBase);
			result.push_back(targetBaseSet_[remainder]);
		} while (!value.empty() && !(value.length() == 1 && value[0] == sourceBaseSet_[0]));

		reverse(result.begin(), result.end());
		return result;
	}

	std::string BaseConverter::Convert(const std::string& value, size_t minDigits) const
	{
		std::string result = Convert(value);
		if (result.length() < minDigits)
			return std::string(minDigits - result.length(), targetBaseSet_[0]) + result;
		return result;
	}

	std::string BaseConverter::FromDecimal(unsigned int value) const
	{
		return dec2base(targetBaseSet_, value);
	}

	std::string BaseConverter::FromDecimal(unsigned int value, size_t minDigits) const
	{
		std::string result = FromDecimal(value);
		if (result.length() < minDigits)
			return std::string(minDigits - result.length(), targetBaseSet_[0]) + result;
		return result;
	}

	unsigned int BaseConverter::ToDecimal(std::string value) const
	{
		return base2dec(sourceBaseSet_, value);
	}

	unsigned int BaseConverter::divide(const std::string& baseDigits, std::string& x, unsigned int y)
	{
		std::string quotient;

		size_t lenght = x.length();
		for (size_t i = 0; i < lenght; ++i)
		{
			size_t j = i + 1 + x.length() - lenght;
			if (x.length() < j)
				break;

			unsigned int value = base2dec(baseDigits, x.substr(0, j));

			quotient.push_back(baseDigits[value / y]);
			x = dec2base(baseDigits, value % y) + x.substr(j);
		}

		// calculate remainder
		unsigned int remainder = base2dec(baseDigits, x);

		// remove leading "zeros" from quotient and store in 'x'
		size_t n = quotient.find_first_not_of(baseDigits[0]);
		if (n != std::string::npos)
		{
			x = quotient.substr(n);
		}
		else
		{
			x.clear();
		}

		return remainder;
	}

	std::string BaseConverter::dec2base(const std::string& baseDigits, unsigned int value)
	{
		unsigned int numberBase = static_cast<unsigned int>(baseDigits.length());
		std::string result;
		do
		{
			result.push_back(baseDigits[value % numberBase]);
			value /= numberBase;
		} while (value > 0);

		reverse(result.begin(), result.end());
		return result;
	}

	unsigned int BaseConverter::base2dec(const std::string& baseDigits, const std::string& value)
	{
		unsigned int numberBase = static_cast<unsigned int>(baseDigits.length());
		unsigned int result = 0;
		for (size_t i = 0; i < value.length(); ++i)
		{
			result *= numberBase;
			size_t c = baseDigits.find(value[i]);
			if (c == std::string::npos)
				throw std::runtime_error("Invalid character");

			result += static_cast<unsigned int>(c);
		}

		return result;
	}

//************************************ Helper Functions ************************************************

	std::string hex_char_to_bin_quad(char c)
	{
		switch (tolower(c))
		{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'a': return "1010";
		case 'b': return "1011";
		case 'c': return "1100";
		case 'd': return "1101";
		case 'e': return "1110";
		case 'f': return "1111";
		default: return "ERRO";
		}
	}	

	char bin_quad_to_hex_char(std::string s)
	{
		if (s == "0000" || s == "000" || s == "00" || s == "0") return '0';
		if (s == "0001" || s == "001" || s == "01" || s == "1") return '1';
		if (s == "0010" || s == "010" || s == "10") return '2';
		if (s == "0011" || s == "11" || s == "11") return '3';
		if (s == "0100" || s == "100") return '4';
		if (s == "0101" || s == "101") return '5';
		if (s == "0110" || s == "110") return '6';
		if (s == "0111" || s == "111") return '7';
		if (s == "1000") return '8';
		if (s == "1001") return '9';
		if (s == "1010") return 'a';
		if (s == "1011") return 'b';
		if (s == "1100") return 'c';
		if (s == "1101") return 'd';
		if (s == "1110") return 'e';
		if (s == "1111") return 'f';
		return 'X';
	}

	std::string hex_to_bin_str(const std::string& hex)
	{
		std::string bin;
		for (size_t i = 0; i < hex.length(); ++i)
			bin += hex_char_to_bin_quad(hex[i]);
		return bin;
	}

	std::string bin_to_hex_str(const std::string& bin)
	{
		std::string hex;
		int i;
		for (i = static_cast<int>(bin.length()) - 4; i >= 0; i -= 4)
			hex = bin_quad_to_hex_char(bin.substr(i, 4)) + hex;
		if (i > -4)
			hex = bin_quad_to_hex_char(bin.substr(0, -i + 4)) + hex;
		std::string::size_type pos = hex.find_first_not_of('0', 0);
		if (pos > 0)
			hex.erase(0, pos);
		return hex;
	}

	std::string hex_to_dec_str(const std::string& hex)
	{
		std::string temp = hex;
		transform(temp.begin(), temp.end(), temp.begin(), tolower);
		const BaseConverter& hex2dec = BaseConverter::HexToDecimalConverter();
		return hex2dec.Convert(hex);
	}

	std::string dec_to_hex_str(const std::string& dec)
	{
		const BaseConverter& dec2hex = BaseConverter::DecimalToHexConverter();
		return dec2hex.Convert(dec);
	}
}