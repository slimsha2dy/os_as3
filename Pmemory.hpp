#ifndef PMEMORY_HPP
#define PMEMORY_HPP

#include "Memory.hpp"

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
