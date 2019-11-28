

#ifndef BOX_H_
#define BOX_H_
#include "Disk.h"


class Box {
private:
	int number;
	Disk* disks;
public:
	Box();
	virtual ~Box();
};

#endif /* BOX_H_ */
