#ifndef __MS_STRUTIL_H__
#define __MS_STRUTIL_H__
#include <string>
#include <map>
#include <stdlib.h>


namespace matchvs 
{
	void strTrim(std::string &str);
	std::string makeSignCommon(std::map<std::string, std::string> &params, std::string &separator, std::string &headKey, std::string &tailKey);
};



#endif