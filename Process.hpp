#ifndef	PROCESS_HPP
#define	PROCESS_HPP

#include "Memory.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class	Process
{
	private:
		string	pname;
		int		pid;
		int		ppid;
		string	pstate;	// state of process (new, ready, running, waiting, terminate)
		string	*code;
		Process	*next;
		string	tmpCode[2];	// [0]: command, [1]: argument
		int		pc;
		int		runtime;	// remaining time to run
		int		sleeptime;	// remaining time to sleep

		// Memory Part
		Vmemory vmemory;	// virtual memory of process
		int	last_pageid;	// last page id of process
		int last_allocid;	// last allocation id of process
		//

	public:
		Process();
		~Process();
		Process(string pname);
		Process(string input, string pname, int pid, int ppid);

		// Process.cpp
		string	readCommand(void);
		void	run(string arg);
		void	exit(void);
		void	sleep(string arg);
		void	allocVmem(void);
		
		// Process_util.cpp
		int		getPid(void) const;
		int		getPpid(void) const;
		void	changeState(const string state);
		void	addNext(Process *next);
		Process	*getNext(void) const;	// get next process on queue
		void	printInfo(ofstream &ofile) const;	// print "pid(pname, ppid)"
		void	printWait(ofstream &ofile) const;	// print "pid(S/W)"
		string	getPstate(void) const;
		string	getCommand(void) const;
		string	getArgument(void) const;
		void	subSleep(void);	
		int		getSleep(void);
		void	changeSleep(int time);
		// Memory
		Vmemory	&getVmemory(void);
		int		getAllocid(void) const;
		void	printMemory(ofstream &ofile) const;
};

#endif
