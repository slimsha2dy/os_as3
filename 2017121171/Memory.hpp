#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class   Pmemory;

class	Vmemory	// virtual memory and page table class
{
	private:
		bool is_alloc[32];	// if the page is allocated
		int	pageid[32];
		int allocid[32]; // allocation id(page id is index)
		bool valid[32];	// valid bit: if the page is on physical memory
		bool permission[32];	// read(0) or write(1)
		int address[32];	// physical address
        int copied[32];

	public:
		Vmemory();
		~Vmemory();

		void allocVmem(int num, int &allocid, int &pageid);	// allocation
		int tableWrite(int allocid, int address);	// write to table page and return page id
		bool getIsAlloc(int i) const;
		int getPageId(int i) const;
		bool getValid(int i) const;
		bool getPermission(int i) const;
		int getAddress(int i) const;    // get address of index i
		int findAddress(int add) const; // get index that have a "i" physical address
		void unvalid(int i);    // invalidate i index
        Vmemory& operator=(Vmemory& other);
        void permToread(void);  // change all permission to read
        void permTowrite(int allocid); // change permission to write of the allocate id
        void permTowritePage(int pageid); // change permission to write of the page id
        vector<int> memoryRelease(int allocid, int pid);
        bool getPermofAlloc(int allocid);   // get allocate id and return permission
        int findPageId(int i); // get index that have a "i" page id
        void changeTable(int pageid, int address); // change table of the pageid
        int getCopied(int i);
        void setCopied(int i, int j);
};

#endif
