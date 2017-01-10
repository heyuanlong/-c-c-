#include <MsStrUtil.h>
#include <md5.h>


namespace matchvs
{
	void strTrim(std::string &str)
	{
		const std::string CharsToTrim = " \t\n\r";

		size_t startIndex = str.find_first_not_of(CharsToTrim);
		if (startIndex == std::string::npos)
		{
			str.erase();
			return;
		}

		str = str.substr(startIndex, str.size() - startIndex);
		str = str.substr(0, str.find_last_not_of(CharsToTrim) + 1);
	}

	std::string makeSignCommon(std::map<std::string, std::string> &params, std::string &separator, std::string &headKey, std::string &tailKey)
	{
		std::string s(headKey);
		std::map<std::string, std::string>::iterator b = params.begin();
		std::map<std::string, std::string>::iterator e = params.end();

		s.append(separator);
		for (; b != e; ++b)
		{
			s.append(b->first).append("=").append(b->second).append(separator);
		}
		s.append(tailKey);

		MD5 md5(s);
		return md5.md5();
	}
};