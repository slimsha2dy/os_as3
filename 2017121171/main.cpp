#include "Kernel.hpp"

int	main(int argc, char **argv)
{
	string	str = argv[1];
	Kernel	kernel(str, argv[2]);
	ofstream	ofile;
	ofile.open("result", std::ios::out);
	ofile << "[cycle #0]" << endl;
	kernel.printState(ofile);

	while (!kernel.getallExit())
	{		
		if (kernel.getSysflag())
			kernel.checkSyscall();
		kernel.updateSleep();
		kernel.updateState();
		kernel.updateRq();
		kernel.excute();
		ofile << "[cycle #" << kernel.getCycle() << "]" << endl;
		kernel.printState(ofile);
		kernel.plusCycle();
	}
	return (0);
}
