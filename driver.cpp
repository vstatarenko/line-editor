#include "Led.h"
using namespace std;

int main(int argc, char *argv[])
{
	string filename;			//an empty filename
	switch (argc)
	{
	case 1:						//no filename
		break;
	case 2:
		filename = argv[1];		//initialize filename from argument
		break;
	default: cout << ("too many arguments - all discarded") << endl;
		break;
	}
	Led led(filename);			// create an editor named led
	led.run(); 
	return 0;
}

