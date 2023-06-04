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

Process::Process(string input, string pname, int pid, int ppid)
{
	this->pname = pname;
	this->pid = pid;
	this->ppid = ppid;
	this->pstate = "new";
	this->next = 0;
	this->pc = 0;
	this->runtime = 0;
	this->sleeptime = 0;

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
	if (this->tmpCode[0] == "exit")
	{
		this->exit();
	}
	if (this->tmpCode[0] == "sleep")
	{
		this->sleep(this->tmpCode[1]);
	}
	if (this->tmpCode[0] == "fork_and_exec" || this->tmpCode[0] == "wait")
	{
		this->pc++;
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
