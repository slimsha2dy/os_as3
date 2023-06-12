#include "Pmemory.hpp"
#include "Process.hpp"
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

void	Pmemory::allocPmem(int num, Process *tmp, Process *headRq, Process *headWq, int allocid, int pid, string policy, int cycle)
{
	// count not allocated frame
	int count = 0;
	for (int i = 0; i < 16; ++i) {
		if (!this->valid[i])
			count++;
	}

	if (count < num) {
		this->findVictim(num - count, policy, tmp, headRq, headWq);
	}

	int i = 0;
	while(num--) {
		while (this->valid[i])
			i++;
		this->valid[i] = 1;
		this->pid[i] = pid;
		this->usedCount[i] = 1;
		this->pageid[i] = (tmp->getVmemory()).tableWrite(allocid, i);
		this->allocTime[i] = cycle;
		this->accessTime[i] = cycle;
		i++;
	}
}

void	Pmemory::faultHandle(Process *tmp, Process *headRq, Process *headWq, int pid, std::string policy, int cycle, int pageid)
{
    // count not allocated frame
    int count = 0;
    for (int i = 0; i < 16; ++i) {
        if (!this->valid[i])
            count++;
    }

    if (count == 0) {
        this->findVictim(1, policy, tmp, headRq, headWq);
    }

    int i = 0;
    while (this->valid[i])
        i++;
    this->valid[i] = 1;
    this->pid[i] = pid;
    this->usedCount[i] = 1;
    this->pageid[i] = pageid;
    this->allocTime[i] = cycle;
    this->accessTime[i] = cycle;
    (tmp->getVmemory()).changeTable(pageid, i);
    i++;

}

void	Pmemory::findVictim(int num, string policy, Process *tmp, Process *headRq, Process *headWq)
{
	int i;
	int unval = 0;
    int pid = 0;
    Process *buf = NULL;    // buffer Process
	while (num--) {
		if (policy == "fifo")
			i = fifo();
		else if (policy == "lru")
			i = lru();
		else
			i = lmfu(policy);
        pid = this->pid[i];
        unval = (tmp->getVmemory()).findAddress(i);
        if (unval != -1)
            (tmp->getVmemory()).unvalid(unval);
        buf = headRq;
        while (buf) {
            unval = (buf->getVmemory()).findAddress(i);
            if (unval != -1)
                (buf->getVmemory()).unvalid(unval);
            buf = buf->getNext();
        }
        buf = headWq;
        while (buf) {
            unval = (buf->getVmemory()).findAddress(i);
            if (unval != -1)
                (buf->getVmemory()).unvalid(unval);
            buf = buf->getNext();
        }
	}
}

int	Pmemory::fifo(void)
{
	int tmp = 0;
    while (!this->valid[tmp])
        tmp++;
	int i = tmp + 1;
	while (i < 16) {
		if (this->valid[i] && this->allocTime[tmp] > this->allocTime[i])
			tmp = i;
		++i;
	}
	this->valid[tmp] = 0;
	return (tmp);
}

int	Pmemory::lru(void)
{
    int tmp = 0;
    while (!this->valid[tmp])
        tmp++;
    int i = tmp + 1;
	while (i < 16) {
		if (this->valid[i] && this->accessTime[tmp] > this->accessTime[i])
			tmp = i;
        cout << this->accessTime[i] << " ";
		++i;
	}
    cout << endl;
	this->valid[tmp] = 0;
	return (tmp);
}

int	Pmemory::lmfu(string policy)
{
    int tmp = 0;
    while (!this->valid[tmp])
        tmp++;
    int i = tmp + 1;
	while (i < 16) {
		if (policy == "lfu") {
			if (this->valid[i] && this->usedCount[tmp] > this->usedCount[i])
				tmp = i;
		}
		else {
			if (this->valid[i] && this->usedCount[tmp] < this->usedCount[i])
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

void Pmemory::invalid(int i)
{
    this->valid[i] = 0;
}

void Pmemory::updateTime(int i, int cycle)
{
    this->accessTime[i] = cycle;
}
