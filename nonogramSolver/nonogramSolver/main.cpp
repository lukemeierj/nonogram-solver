// nonogramSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Nonogram.h"

int main()
{
	Nonogram non = Nonogram(5, 2);
	non.saveBoardConfig("2x2.txt");
	Nonogram nonConfig("2x2.txt");
	nonConfig.saveBoardConfig("2x2-test.txt");
	non.isSolved();
    return 0;
}

