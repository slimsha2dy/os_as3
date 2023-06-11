#include "Process.hpp"
#include <fstream>

int	Process::getPid(void) const
{
	return (this->pid);
}

int	Process::getPpid(void) const
{
	return (this->ppid);
}

void	Process::changeState(const string state)
{
	this->pstate = state;
}

void	Process::addNext(Process *next)
{
	this->next = next;
}

Process	*Process::getNext(void) const
{
	return (this->next);
}

void	Process::printInfo(ofstream &ofile) const
{
	ofile << this->pid << "(" << this->pname << ", " << this->ppid \
		<< ")" << endl;
}

void	Process::printWait(ofstream &ofile) const
{
	ofile << " " << this->pid << "(";
	if (this->tmpCode[0] == "wait")
		ofile << "W";
	else
		ofile << "S";
	ofile << ")";
}

string	Process::getPstate(void) const
{
	return (this->pstate);
}

string	Process::getCommand(void) const
{
	return (this->tmpCode[0]);
}

string	Process::getArgument(void) const
{
	return (this->tmpCode[1]);
}

void	Process::subSleep(void)
{
	this->sleeptime--;
}

int	Process::getSleep(void)
{
	return (this->sleeptime);
}

void	Process::changeSleep(int time)
{
	this->sleeptime = time;
}

void	Process::printMemory(ofstream &ofile) const
{
	// 9. virtual memory
	ofile << "9. virtual memory:" << endl;
	int i = 0;
	for (int j = 0; j < 8; ++j) {
		ofile << "|";
		for (int k = 0; k < 4; ++k) {
			if (!(this->vmemory).getIsAlloc(i))
				ofile << "-";
			else
				ofile << (this->vmemory).getPageId(i);
			if (k != 3)
				ofile << " ";
			i++;
		}
		ofile << "|";
	}
	ofile << endl;

	// 10. page table
	ofile << "10. page table:" << endl;
	i = 0;
	for (int j = 0; j < 8; ++j) {
		ofile << "|";
		for (int k = 0; k < 4; ++k) {
			if (!(this->vmemory).getValid(i))
				ofile << "-";
			else
				ofile << (this->vmemory).getAddress(i);
			if (k != 3)
				ofile << " ";
			i++;
		}
		ofile << "|";
	}
	ofile << endl;

	i = 0;
	for (int j = 0; j < 8; ++j) {
		ofile << "|";
		for (int k = 0; k < 4; ++k) {
			if (!(this->vmemory).getIsAlloc(i))
				ofile << "-";
			else {
				if ((this->vmemory).getPermission(i))
					ofile << "W";
				else
					ofile << "R";
			}
			if (k != 3)
				ofile << " ";
			i++;
		}
		ofile << "|";
	}
	ofile << endl;
}
