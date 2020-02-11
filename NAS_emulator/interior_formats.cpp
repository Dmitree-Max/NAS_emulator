

#include "interior_formats.h"


// device header   flags    start    cnt      cmd  fmt   mhop
// 0001   30000002 00000000 00000000 00000000 0003 0000  00000001
std::ostream& operator << (std::ostream& stm, const struct Request& req)
{
    stm <<  "device: "  << std::hex <<  req.device << "\nheader: " << std::hex << req.header <<
    	    "\nflags: " << std::hex <<  req.flags  <<
			"\nstart: " << std::hex <<  req.start  << "\ncnt: "    << std::hex << req.cnt    <<
			"\ncmd: "   << std::hex <<  req.cmd    << "\nfmt: "    << std::hex << req.fmt    <<
			"\nmhop: "  << std::hex <<  req.mhop   << std::endl;
    return stm;
}


// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
std::ostream& operator << (std::ostream& stm, const struct Answer& req)
{
    stm <<  "device: "  << std::hex <<  req.device << "\nheader: " << std::hex << req.header <<
    	    "\nflags: " << std::hex <<  req.flags  <<
			"\nstart: " << std::hex <<  req.start  << "\ncnt: "    << std::hex << req.cnt    <<
			"\ncmd: "   << std::hex <<  req.cmd    << "\nfmt: "    << std::hex << req.fmt    <<  std::endl;
    return stm;
}



std::string expand_to_byte(int number)
{
	std::string result = std::to_string(number);
	if (result.length() > 8)
	{
		throw("Number more then byte");
	}
	result = "00000000" + result;
	result = result.substr(result.length() - 8);
	return result;
}


std::string expand_to_half_byte(int number)
{
	std::string result = std::to_string(number);
	if (result.length() > 4)
	{
		throw("Number more then byte");
	}
	result = "0000" + result;
	result = result.substr(result.length() - 4);
	return result;
}


std::string* answer_to_string(Answer* answer)
{
	std::string* result = new std::string;
	*result = expand_to_byte(answer->header) + expand_to_byte(answer->flags) + expand_to_byte(answer->start) +\
			expand_to_byte(answer->cnt) + expand_to_half_byte(answer->cmd) + expand_to_half_byte(answer->fmt);
	return result;
}


