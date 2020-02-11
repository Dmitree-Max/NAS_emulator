

#ifndef INTERIOR_FORMATS_H_
#define INTERIOR_FORMATS_H_

#include <iostream>


// device header   flags    start    cnt      cmd  fmt   mhop
// 0001   30000002 00000000 00000000 00000000 0003 0000  00000001
struct Request {
	int device;
	int header;
	int flags;
	int start;
	int cnt;
	int cmd;
	int fmt;
	int mhop;
};


// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
struct Answer {
	int device;
	int header;
	int flags;
	int start;
	int cnt;
	int cmd;
	int fmt;
};

std::ostream& operator << (std::ostream& stm, const struct Request& req);

std::ostream& operator << (std::ostream& stm, const struct Answer& req);

std::string expand_to_byte(int number);

std::string* answer_to_string(Answer* answer);

#endif /* INTERIOR_FORMATS_H_ */
