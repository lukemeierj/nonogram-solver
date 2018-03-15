// nonogramSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Nonogram.h"
#include<assert.h>

int main()
{
	Nonogram non("5x2.txt");
	non.saveBoardConfig("5x2-test.txt");
	non.set(0, 0, FILL);
	non.set(1, 0, EMPTY);
	non.set(2, 0, FILL);
	non.set(3, 0, EMPTY);
	non.set(4, 0, FILL);
	non.set(0, 1, FILL);
	non.set(1, 1, FILL);
	non.set(2, 1, FILL);
	non.set(3, 1, EMPTY);
	non.set(4, 1, EMPTY);
	assert(non.isSolved());

	vector<TileType> secondRow = non.getLine(1, true);
	vector<TileType> secondColumn = non.getLine(1, false);
	assert(secondRow.size() == 5);
	assert(secondColumn.size() == 2);

	non.set(0, 0, FILL);
	non.set(1, 0, EMPTY);
	non.set(2, 0, FILL);
	non.set(3, 0, EMPTY);
	non.set(4, 0, FILL);
	non.set(0, 1, FILL);
	non.set(1, 1, FILL);
	non.set(2, 1, FILL);
	non.set(3, 1, EMPTY);
	non.set(4, 1, FILL);
	assert(!non.isSolved());
	non.set(0, 0, FILL);
	non.set(1, 0, UNKNOWN);
	non.set(2, 0, FILL);
	non.set(3, 0, EMPTY);
	non.set(4, 0, FILL);
	non.set(0, 1, FILL);
	non.set(1, 1, FILL);
	non.set(2, 1, FILL);
	non.set(3, 1, EMPTY);
	non.set(4, 1, EMPTY);
	assert(!non.isSolved());




    return 0;
}

