#include "Memory.hpp"
#include <iostream>
using namespace std;

Pmemory::Pmemory()
{
	for (int i = 0; i < 16; ++i) {
		this->valid[i] = 0;
		this->pid[i] = -1;
		this->pageid[i] = -1;
		this->allocTime[i] = 0;
		this->accessTime[i] = 0;
		this->usedCount[i] = 0;
	}
}

Pmemory::~Pmemory()
{
}

void	Pmemory::allocPmem(int num, Vmemory &vmemory, int allocid, int pid, string policy, int cycle)
{
	// count not allocated frame
	int count = 0;
	for (int i = 0; i < 16; ++i) {
		if (this->valid[i])
			count++;
	}

	if (count < num) {
		this->findVictim(num - count, policy, vmemory);
	}

	int i = 0;
	while(num--) {
		while (this->valid[i])
			i++;
		this->valid[i] = 1;
		this->pid[i] = pid;
		this->usedCount[i] = 1;
		this->pageid[i] = vmemory.tableWrite(allocid, i);
		this->allocTime[i] = cycle;
		this->accessTime[i] = cycle;
		i++;
	}
}

void	Pmemory::findVictim(int num, string policy, Vmemory &vmemory)
{
	int i;
	int unval = 0;
	while (num--) {
		if (policy == "fifo")
			i = fifo();
		else if (policy == "lfu")
			i = lru();
		else
			i = lmfu(policy);
		unval = vmemory.findAddress(i);
		vmemory.unvalid(unval);
	}
}

int	Pmemory::fifo(void)
{
	int tmp = 0;
	int i = 1;
	while (i < 16) {
		if (this->allocTime[tmp] > this->allocTime[i])
			tmp = i;
		++i;
	}
	this->valid[tmp] = 0;
	return (tmp);
}

int	Pmemory::lru(void)
{
	int tmp = 0;
	int i = 1;
	while (i < 16) {
		if (this->accessTime[tmp] > this->accessTime[i])
			tmp = i;
		++i;
	}
	this->valid[tmp] = 0;
	return (tmp);
}

int	Pmemory::lmfu(string policy)
{
	int tmp = 0;
	int i = 1;
	while (i < 16) {
		if (policy == "lfu") {
			if (this->usedCount[tmp] > this->usedCount[i])
				tmp = i;
		}
		else {
			if (this->usedCount[tmp] < this->usedCount[i])
				tmp = i;
		}
		++i;
	}
	this->valid[tmp] = 0;
	return (tmp);
}

int	Pmemory::getFramePid(int i) const
{
	return (this->pid[i]);
}

int Pmemory::getFramePageid(int i) const
{
	return (this->pageid[i]);
}

bool Pmemory::getValid(int i) const
{
	return (this->valid[i]);
}

void Pmemory::plusUsedCount(int i)
{
	this->usedCount[i]++;
}
