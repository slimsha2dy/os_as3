#include "Kernel.hpp"

Kernel::Kernel(string pname, string policy)
{
	this->headRq = 0;
	this->tailRq = 0;
	this->headWq = 0;
	this->tailWq = 0;
	this->tmp = 0;
	this->input = pname + "/";
	this->newProcess = new Process(this->input + "init");
	this->terProcess = 0;

	this->mode = "kernel";
	this->syscallFlag = 0;
	this->kstate = "boot";
	this->last_pid = 1;
	this->exitCount = 0;
	this->cycle = 1;

	this->policy = policy;
} 

Kernel::~Kernel(void)
{
	if (this->terProcess)
		delete this->terProcess;
}

void	Kernel::checkSyscall(void)
{
	this->mode = "kernel";
	this->kstate = "system call";
	if (this->syscallCommand == "sleep")
	{
		(this->tmp)->changeState("waiting");
		this->pushWq(this->tmp);
		this->tmp = 0;
	}
}

void	Kernel::updateSleep(void)
{
	Process	*tmp = this->headWq;
	while (tmp)
	{
		if (tmp->getSleep() > 0)	// if process sleeping
			tmp->subSleep();
		tmp = tmp->getNext();
	}
}

void	Kernel::updateState(void)
{
	// change state of process on readyqueue
	Process	*tmp = this->headWq;
	while (tmp)
	{
		if (tmp->getSleep() == 0)
			tmp->changeState("ready");
		tmp = tmp->getNext();
	}

	// change state of new process
	if (this->newProcess)
		(this->newProcess)->changeState("ready");

	// delete terminated process
	if (this->terProcess)
	{
		delete this->terProcess;
		this->terProcess = 0;
	}
}

void	Kernel::updateRq(void)
{
	// waiting queue to readyqueue
	Process	*tmp = this->headWq;
	while (tmp)
	{
		if (tmp->getSleep() == 0)
		{
			Process	*buf = tmp;
			tmp = tmp->getNext();
			this->pushRq(this->popWq(buf));
		}
		else
			tmp = tmp->getNext();
	}
	// new process to readyqueue
	if (this->newProcess)
	{
		this->pushRq(this->newProcess);
		this->newProcess = 0;
	}
}

void	Kernel::excute(void)
{
	if (this->syscallFlag)	// system call command
	{
		this->mode = "kernel";
		syscall();
		return ;
	}

	else if (!this->tmp)	// schedule or idle
	{
		this->mode = "kernel";
		this->scheduleIdle();
		return ;
	}

	// running process command
	this->mode = "user";
	string command = (this->tmp)->readCommand();
    if (this->tmp->getCommand() == "memory_read" || this->tmp->getCommand() == "memory_write")
    {
        // update access time and count
        int idx = (this->tmp->getVmemory()).findPageId(stoi(this->tmp->getArgument()));
        int add = (this->tmp->getVmemory()).findAddress(idx);
        (this->pmemory).plusUsedCount(add);
        (this->pmemory).updateTime(add, this->cycle);
    }
	if (command != "run" && command != "memory_read" && command != "memory_write")
	{
		this->syscallFlag = 1;
		this->syscallCommand = command;
	}
}

void	Kernel::scheduleIdle(void)
{
	if (!this->headRq)	// empty readyqueue: idle
	{
		this->changeKstate("idle");
		return ;
	}

	// scheduler action
	this->changeKstate("schedule");
	this->tmp = this->popRq();
	(this->tmp)->changeState("running");
}

void	Kernel::syscall(void)
{
	// exit
	if (this->syscallCommand == "exit")
	{
		Process *tmp = this->headWq;
		while (tmp)	// if there is waiting parent, take off the waiting
		{
			if ((this->tmp)->getPpid() == tmp->getPid() && tmp->getSleep() == -1)
			{
				tmp->changeSleep(0);
				this->pushRq(this->popWq(tmp));
				break;
			}
			tmp = tmp->getNext();
		}
        memory_release_all();
		this->terProcess = this->tmp;
		this->tmp = 0;
		this->exitCount++;
	}

	// fork_and_exec
	else if (this->syscallCommand == "fork_and_exec")
	{
		this->newProcess = new Process(this->input, (this->tmp)->getArgument(), ++this->last_pid, (this->tmp)->getPid(), (this->tmp)->getVmemory(), (this->tmp)->getLastPage(), (this->tmp)->getLastAlloc());
        (this->tmp->getVmemory()).permToread();
        (this->tmp)->changeState("ready");
		this->pushRq(this->tmp);
		this->tmp = 0;
	}

	// wait
	else if (this->syscallCommand == "wait")
		this->wait();

	// memory allocate
	else if (this->syscallCommand == "memory_allocate")
	{
		this->memory_allocate();
		(this->tmp)->changeState("ready");
		this->pushRq(this->tmp);
		this->tmp = 0;
	}

    // memory release
    else if (this->syscallCommand == "memory_release")
    {
        this->memory_release();
        (this->tmp)->changeState("ready");
        this->pushRq(this->tmp);
        this->tmp = 0;
    }

    // page fault
    else if (this->syscallCommand == "page_fault")
    {
        this->page_fault();
        (this->tmp)->changeState("ready");
        this->pushRq(this->tmp);
        this->tmp = 0;
    }

    // protection fault
    else if (this->syscallCommand == "protection_fault")
    {
        this->protection_fault();
        (this->tmp)->changeState("ready");
        this->pushRq(this->tmp);
        this->tmp = 0;
    }

	this->syscallFlag = 0;
}

void	Kernel::wait(void)
{
	bool	is_child = 0;

	Process	*tmp = this->headRq;
	while (tmp)
	{
		if (tmp->getPpid() == (this->tmp)->getPid())
		{
			is_child = 1;
			break;
		}
		tmp = tmp->getNext();
	}
	tmp = this->headWq;
	while (tmp)
	{
		if (tmp->getPpid() == (this->tmp)->getPid())
		{
			is_child = 1;
			break;
		}
		tmp = tmp->getNext();
	}
	if (is_child)	// if there is child process, push to waiting queue
	{
		(this->tmp)->changeSleep(-1);
		(this->tmp)->changeState("waiting");
		this->pushWq(this->tmp);
		this->tmp = 0;
	}
	else			// if there is no child, push to ready queue
	{
		(this->tmp)->changeState("ready");
		this->pushRq(this->tmp);
		this->tmp = 0;
	}
}

void	Kernel::memory_allocate(void)
{
    // allocate to virtual memory
	this->tmp->allocVmem();
    // allocate to physical memory
	(this->pmemory).allocPmem(stoi(this->tmp->getArgument()), this->tmp, this->headRq, this->headWq, this->tmp->getAllocid(), this->tmp->getPid(), this->policy, this->cycle);
}

void    Kernel::memory_release(void)
{
    Process *buf = this->headRq;
    int id = stoi(this->tmp->getArgument());
    // if permission of allocation id is read, all shared allocation id change to write
    if (!(this->tmp->getVmemory()).getPermofAlloc(id)) {
        // R -> W
        while (buf) {
            (buf->getVmemory()).permTowrite(id);
            buf = buf->getNext();
        }
        buf = this->headWq;
        while (buf) {
            (buf->getVmemory()).permTowrite(id);
            buf = buf->getNext();
        }
    }
    // memory release
    this->tmp->memoryRelease(id, this->pmemory);
}

void    Kernel::memory_release_all(void)
{
    for (int id = 0; id <= this->tmp->getLastAlloc(); ++id) {
        if (!(this->tmp->getVmemory()).getPermofAlloc(id)) {
            // R -> W
            Process *buf = this->headRq;
            while (buf) {
                (buf->getVmemory()).permTowrite(id);
                buf = buf->getNext();
            }
            buf = this->headWq;
            while (buf) {
                (buf->getVmemory()).permTowrite(id);
                buf = buf->getNext();
            }
        }

        // memory release
        this->tmp->memoryRelease(id, this->pmemory);
    }
}

void Kernel::page_fault(void)
{
    // allocate to physical memory and write to page table
    (this->pmemory).faultHandle(this->tmp, this->headRq, this->headWq, this->tmp->getPid(), this->policy, this->cycle, stoi(this->tmp->getArgument()));
}

void Kernel::protection_fault(void)
{
    // all shared page's permission to write
    int pageid = stoi(this->tmp->getArgument());
    Process *buf = this->headRq;
    while (buf) {
        (buf->getVmemory()).permTowritePage(pageid);
        buf = buf->getNext();
    }
    buf = this->headWq;
    while (buf) {
        (buf->getVmemory()).permTowritePage(pageid);
        buf = buf->getNext();
    }
    // if parent, change permission
    if (this->tmp->getPid() == 1)
    {
        (this->tmp->getVmemory()).permTowritePage(pageid);
        int idx = (this->tmp->getVmemory()).findPageId(pageid);
        if (!(this->tmp->getVmemory()).getValid(idx)) {
            (this->pmemory).faultHandle(this->tmp, this->headRq, this->headWq, this->tmp->getPid(), this->policy, this->cycle, pageid);
        }
        // update count and access time
        int add = (this->tmp->getVmemory()).getAddress(idx);
        (this->pmemory).plusUsedCount(add);
        (this->pmemory).updateTime(add, this->cycle);
        // invalid shared page of child
        buf = this->headRq;
        while (buf) {
            idx = (buf->getVmemory()).findAddress(add);
            if (idx != -1 && (buf->getVmemory()).getCopied(idx)) {
                (buf->getVmemory()).unvalid(idx);
                (buf->getVmemory()).setCopied(idx, 0);  // copied bit to false
            }
            buf = buf->getNext();
        }
        buf = this->headWq;
        while (buf) {
            idx = (buf->getVmemory()).findAddress(add);
            if (idx != -1 && (buf->getVmemory()).getCopied(idx)) {
                (buf->getVmemory()).unvalid(idx);
                (buf->getVmemory()).setCopied(idx, 0);  // copied bit to false
            }
            buf = buf->getNext();
        }
    }
    // if not, allocate new frame
    else
    {
        // copied bit to false
        int idx = (this->tmp->getVmemory()).findPageId(pageid);
        int add = (this->tmp->getVmemory()).getAddress(idx);
        buf = this->headRq;
        while (buf) {
            idx = (buf->getVmemory()).findAddress(add);
            if (idx != -1 && (buf->getVmemory()).getCopied(idx)) {
                (buf->getVmemory()).setCopied(idx, 0);  // copied bit to false
            }
            buf = buf->getNext();
        }
        buf = this->headWq;
        while (buf) {
            idx = (buf->getVmemory()).findAddress(add);
            if (idx != -1 && (buf->getVmemory()).getCopied(idx)) {
                (buf->getVmemory()).setCopied(idx, 0);  // copied bit to false
            }
            buf = buf->getNext();
        }
        // allocate page
        (this->tmp->getVmemory()).permTowritePage(pageid);
        (this->pmemory).faultHandle(this->tmp, this->headRq, this->headWq, this->tmp->getPid(), this->policy, this->cycle, pageid);
    }
}
