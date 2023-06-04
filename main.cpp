#include "Kernel.hpp"

int	main(int argc, char **argv)
{
	string	str = argv[1];
	Kernel	kernel(str);
	ofstream	ofile;
	ofile.open("result", std::ios::out);
	ofile << "[cycle #0]" << endl;
	kernel.printState(ofile);

	int	cycle = 1;
	while (!kernel.getallExit())
	{		
		if (kernel.getSysflag())
			kernel.checkSyscall();
		kernel.updateSleep();
		kernel.updateState();
		kernel.updateRq();
		kernel.excute();
		ofile << "[cycle #" << cycle << "]" << endl;
		kernel.printState(ofile);
		++cycle;
	}
	return (0);
}
