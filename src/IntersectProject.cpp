#include "Class.h"


int main(int argc, char* argv[])
{
	string in_file, out_file;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i] == "-i")
		{
			in_file = argv[i + 1];
		}
		else if (argv[i] == "-o")
		{
			out_file = argv[i + 1];
		}
	}

	BurteForce bf;
	bf.Solve(in_file, out_file);
	return 0;
}