#include "Process.hpp"

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
