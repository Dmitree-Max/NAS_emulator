

#include "Disk.h"

Disk::Disk(int arg_number) {
	number = arg_number;
}

Disk::~Disk() {
}

int Disk::get_number() {
	return this->number;
}

