#include "Class.h"


int main(int argc, char* argv[])
{

	string in_file, out_file;
	for (int i = 0; i < argc; i++)
	{
		if (string(argv[i]) == "-i")
		{
			in_file = string(argv[i + 1]);
		}
		else if (string(argv[i]) == "-o")
		{
			out_file = string(argv[i + 1]);
		}
	}


	BruteForce bf;
	bf.Solve(in_file, out_file);
	// cout << bf.getIntersection("../../pat/combined.txt");
	
	return 0;
}