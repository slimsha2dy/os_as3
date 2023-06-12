#include "Process.hpp"

Process::Process()
{
}

Process::Process(string pname)
{
	this->pname = "init";
	this->pid = 1;
	this->ppid = 0;
	this->pstate = "new";
	this->next = 0;
	this->pc = 0;
	this->runtime = 0;
	this->sleeptime = 0;

	// memory
	this->last_pageid = -1;
	this->last_allocid = -1;
	// memory

	ifstream	file(pname);
	int	length = 0;				// number of lines in the file(pname)
	string	line;
	
	while (getline(file, line))	// count length
		length++;
	file.clear();
	file.seekg(0, ios::beg);	// reset file stream

	this->code = new string[length];
	for (int i = 0; i < length; i++) {
		getline(file, line);
		this->code[i] = line;
	}
}

Process::~Process()
{
	delete[] this->code;
}

Process::Process(string input, string pname, int pid, int ppid, Vmemory& vmemory, int lastPageid, int lastAllocid)
{
	this->pname = pname;
	this->pid = pid;
	this->ppid = ppid;
	this->pstate = "new";
	this->next = 0;
	this->pc = 0;
	this->runtime = 0;
	this->sleeptime = 0;

	// memory
	this->last_pageid = lastPageid;
	this->last_allocid = lastAllocid;
    this->vmemory = vmemory;
	// memory

	ifstream	file(input + pname);
	int	length = 0;				// number of lines in the file(pname)
	string	line;
	
	while (getline(file, line))	// count length
		length++;
	file.clear();
	file.seekg(0, ios::beg);	// reset file stream

	this->code = new string[length];
	for (int i = 0; i < length; i++) {
		getline(file, line);
		this->code[i] = line;
	}
}

string	Process::readCommand(void)
{
	stringstream	ss(this->code[this->pc]);	// temporary command

	ss >> this->tmpCode[0];
	ss >> this->tmpCode[1];

	if (this->tmpCode[0] == "run")
	{
		this->run(this->tmpCode[1]);
	}
	else if (this->tmpCode[0] == "exit")
	{
		this->exit();
	}
	else if (this->tmpCode[0] == "sleep")
	{
		this->sleep(this->tmpCode[1]);
	}
	else if (this->tmpCode[0] == "fork_and_exec" || this->tmpCode[0] == "wait" \
			|| this->tmpCode[0] == "memory_allocate" || this->tmpCode[0] == "memory_release")
	{
		this->pc++;
	}
    else if (this->tmpCode[0] == "memory_read")
    {
        if (this->memory_read(stoi(this->tmpCode[1])))
            return ("page_fault");
    }
    else if (this->tmpCode[0] == "memory_write")
    {
        int flag = this->memory_write(stoi(this->tmpCode[1]));
        if (flag == 0)
            return ("page_fault");
        else if (flag == -1)
            return ("protection_fault");
    }
	return (this->tmpCode[0]);
}

void	Process::run(string arg)
{
	this->pstate = "running";
	if (this->runtime == 0)
		this->runtime = stoi(arg);
	this->runtime--;
	if (this->runtime == 0)
		this->pc++;
}

void	Process::exit(void)
{
	this->pstate = "terminate";
}


void	Process::sleep(string arg)
{
	this->sleeptime = stoi(arg);
	this->pc++;
}

void	Process::allocVmem(void)
{
	(this->vmemory).allocVmem(stoi(this->tmpCode[1]), this->last_allocid, this->last_pageid);
}

Vmemory	&Process::getVmemory(void)
{
	return (this->vmemory);
}

int Process::getAllocid(void) const
{
	return (this->last_allocid);
}

void    Process::memoryRelease(int allocid, Pmemory &pmemory)
{
    vector<int> paddress = (this->vmemory).memoryRelease(allocid, this->pid);
    for (auto& i : paddress) {
        pmemory.invalid(i);
    }
}

int Process::memory_read(int arg)
{
    int idx = (this->vmemory).findPageId(arg);
    pc++;
    if ((this->vmemory).getValid(idx)) {
        return (0);
    }
    return (1);
}

int Process::memory_write(int arg)
{
    int idx = (this->vmemory).findPageId(arg);
    pc++;
    if (!(this->vmemory).getPermission(idx)) {
        return (-1);    // Protection fault
    }
    else if (!(this->vmemory).getValid(idx)) {
        return (0);     // page fault
    }
    return (1);
}