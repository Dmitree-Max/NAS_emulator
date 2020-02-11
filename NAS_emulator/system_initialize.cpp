

#include "system_initialize.h"


int initialize_system(const char* filename)
{
	  std::streampos size;
	  char * memblock;

	  std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
	  if (file.is_open())
	  {
	    size = file.tellg();
	    memblock = new char [size];
	    file.seekg (0, std::ios::beg);
	    file.read (memblock, size);
	    file.close();
	  }
	  else
	  {
		  return -1;
	  }
	  printf(memblock);
	  return 0;
}

