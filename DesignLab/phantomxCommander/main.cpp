#include "phantomxCommander.h"

int main()
{
	phantomxCommander commander;
	char start;
	std::cin >> start;
	commander.demonstration1();

	std::cout << "Enter for exit\n";
	char ch;
	std::cin >> ch;
	return 0;
}
