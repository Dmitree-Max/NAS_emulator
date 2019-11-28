

#ifndef SNAPSHOT_H_
#define SNAPSHOT_H_

class Snapshot {
private:
	int box_from;
	int disk_from;
	int time_of_making;
public:
	Snapshot();
	virtual ~Snapshot();
};

#endif
