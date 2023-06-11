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

class	Pmemory	// physical memory class
{
	private:
		bool valid[16];
		int pid[16];
		int pageid[16];
		int allocTime[16];	// allocation time of frame for fifo
		int accessTime[16];	// access time of frame for lru
		int	usedCount[16];	// counting of used	for lfu, mfu

	public:
		Pmemory();
		~Pmemory();

		void allocPmem(int num, Vmemory &vmemory, int allocid, int pid, string policy, int cycle);
		void findVictim(int num, string policy, Vmemory &vmemory);	// find victim and return index
		int fifo(void);
		int lru(void);
		int lmfu(string policy);

		int	getFramePid(int i) const;
		int getFramePageid(int i) const;
		bool getValid(int i) const;
		void plusUsedCount(int i);
};

#endif
