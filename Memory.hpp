#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <iostream>

using namespace std;

class	Vmemory	// virtual memory and page table class
{
	private:
		bool is_alloc[32];	// if the page is allocated
		int	pageid[32];
		int allocid[32]; // allocation id(page id is index)
		bool valid[32];	// valid bit: if the page is on physical memory
		bool permission[32];	// read(0) or write(1)
		int address[32];	// physical address

	public:
		Vmemory();
		~Vmemory();

		void allocVmem(int num, int &allocid, int &pageid);	// allocation
		int tableWrite(int allocid, int address);	// write to table page and return page id
		bool getIsAlloc(int i) const;
		int getPageId(int i) const;
		bool getValid(int i) const;
		bool getPermission(int i) const;
		int getAddress(int i) const;
		int findAddress(int add) const;
		void unvalid(int i);
};

#endif
