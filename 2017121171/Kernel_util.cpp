#include "Kernel.hpp"

void    Kernel::changeKstate(const string kstate)
{
    this->kstate = kstate;
}

bool	Kernel::getSysflag(void) const
{
	return (this->syscallFlag);
}

bool	Kernel::getallExit(void) const
{
	return (this->exitCount == this->last_pid);
}

void	Kernel::printState(ofstream &ofile) const
{
	// 1. mode
	ofile << "1. mode: " << this->mode << endl;

	// 2. command
	ofile << "2. command: " ;
	if (this->mode == "kernel") {
        if (this->kstate == "system call" && (this->syscallCommand == "page_fault" || this->syscallCommand == "protection_fault"))
            ofile << "fault" << endl;
        else
            ofile << this->kstate << endl;
    }
	else
	{
		ofile << (this->tmp)->getCommand();
		if ((this->tmp)->getCommand() != "exit" && (this->tmp)->getCommand() != "wait")
			ofile << " " << (this->tmp)->getArgument();
		ofile << endl;
	}

	// 3. running
	ofile << "3. running: ";
	if (this->tmp)
		(this->tmp)->printInfo(ofile);
	else
		ofile << "none" << endl;

	// 4. physical memory
	ofile << "4. physical memory:" << endl;
	int i = 0;
	for (int j = 0; j < 4; ++j) {
		ofile << "|";
		for (int k = 0; k < 4; ++k) {
			if (!(this->pmemory).getValid(i))
				ofile << "-";
			else
				ofile << (this->pmemory).getFramePid(i) << "(" << (this->pmemory).getFramePageid(i) << ")";
			if (k != 3)
				ofile << " ";
			i++;
		}
		ofile << "|";
	}
	ofile << endl;
	// 5. virtual memory
	// 6. page table
	if (!this->tmp) {
		ofile << endl;
		return ;
	}
	this->tmp->printMemory(ofile);

	ofile << endl;
}

void	Kernel::pushRq(Process *p)
{
	if (!this->headRq)
	{
		this->headRq = p;
		this->tailRq = p;
	}
	else
	{
		(this->tailRq)->addNext(p);
		this->tailRq = p;
	}
}

Process	*Kernel::popRq(void)
{
	if (this->headRq == 0)
	{
		return (0);
	}
	Process	*tmp = this->headRq;
	if (this->headRq == this->tailRq)
		this->tailRq = 0;
	this->headRq = (this->headRq)->getNext();
	tmp->addNext(0);
	return (tmp);
}

void	Kernel::pushWq(Process *p)
{
	if (!this->headWq)
	{
		this->headWq = p;
		this->tailWq = p;
	}
	else
	{
		(this->tailWq)->addNext(p);
		this->tailWq = p;
	}
}

Process	*Kernel::popWq(void)
{
	if (this->headWq == 0)
	{
		return (0);
	}
	Process	*tmp = this->headWq;
	if (this->headWq == this->tailWq)
		this->tailWq = 0;
	this->headWq = (this->headWq)->getNext();
	tmp->addNext(0);
	return (tmp);
}

Process	*Kernel::popWq(Process *p)
{
	Process	*tmp = this->headWq;
	Process	*ret;
	if (tmp == p)
		return (this->popWq());
	while (tmp->getNext() != p)		// tmp is before p
		tmp = tmp->getNext();
	ret = tmp->getNext();			// ret = p
	tmp->addNext(ret->getNext());	// a-(p)-b -> a-b
	ret->addNext(0);
	if (tmp->getNext() == 0)
		this->tailWq = tmp;
	return (ret);
}

int Kernel::getCycle(void) const
{
	return (this->cycle);
}

void	Kernel::plusCycle(void)
{
	this->cycle++;
}
