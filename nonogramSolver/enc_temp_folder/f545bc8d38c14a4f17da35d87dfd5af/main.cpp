// nonogramSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NonogramSolver.h"
#include "Nonogram.h"
#include <assert.h>
#include <cstdio>
#include <ctime>
#include <iostream>

class timer {
public:
	clock_t init;
	double duration;
	void start() {
		init = clock();
	}
	double get() {
		return (clock() - init) / (double)CLOCKS_PER_SEC;
	}
};

void test(int trials, int width, int height) {
	timer watch;
	double total = 0;
	int failed = 0;
	Nonogram empty = Nonogram();
	Nonogram prev = empty;
	for (int i = 0; i < trials; i++) {
		Nonogram random(width, height, false);
		Nonogram emptyRandom = random.emptied();
		NonogramSolver randomSolver(emptyRandom);
		watch.start();
		Nonogram solution = randomSolver.nearestSolution();
		total += watch.get();

		if (solution.isSolved() && solution != random) {
			failed++;
		}
		prev = random;

	}
	cout << "Test " << trials << " " << width << "x" << height << " nonograms." << endl;
	cout << "Total time solving " << (trials - failed) << "/" << trials << " : " << total << endl;
	cout << "Per solution: " << total / (double)trials << endl << endl;
}

int main()
{
	//test that reading from a file works
	//test that isSolved works
	//test that saving a config works 
	Nonogram non("5x2.txt");
	non.saveBoardConfig("5x2-out.txt");
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
	

	Nonogram non2("5x2-out.txt");
	non2.set(0, 0, FILL);
	non2.set(1, 0, EMPTY);
	non2.set(2, 0, FILL);
	non2.set(3, 0, EMPTY);
	non2.set(4, 0, FILL);
	non2.set(0, 1, FILL);
	non2.set(1, 1, FILL);
	non2.set(2, 1, FILL);
	non2.set(3, 1, EMPTY);
	non2.set(4, 1, EMPTY);

	assert(non2.isSolved());

	//test that when a nonogram is not solved it doesn't give false positives

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


	//test that getLine gets the right thing
	vector<TileType> secondRow = non.getLine(1, true);
	vector<TileType> secondColumn = non.getLine(1, false);
	assert(secondRow.size() == 5);
	assert(secondColumn.size() == 2);

	non.setLine(secondRow, 1, true);
	non.setLine(secondColumn, 1, false);


	Nonogram little("6x1.txt");
	NonogramSolver solveLittle(little);
	assert(solveLittle.nearestSolution().isSolved());

	NonogramSolver solveHarder(non);
	assert(solveHarder.nearestSolution().isSolved());

	Nonogram det5x5("5x5-det.txt");
	NonogramSolver det5x5Solver(det5x5);
	assert(det5x5Solver.nearestSolution().isSolved());

	Nonogram det5x5_2("5x5-det2.txt");
	NonogramSolver det5x5_2Solver(det5x5_2);
	assert(det5x5_2Solver.nearestSolution(true).isSolved());

	//test(1000, 20, 25);


	//for (int i = 5; i < 50; i++)
	//{
	//	test(100, i, i);
	//}

	Nonogram bad("tooManyConstraints.txt");
	NonogramSolver bestOfLuck(bad);
	bestOfLuck.nearestSolution().printWithHints();



	system("pause");

	return 0;
}

