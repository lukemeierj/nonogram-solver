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

	non.setLine(secondRow, 1, true);
	non.setLine(secondColumn, 1, false);


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

	Nonogram little("6x1.txt");
	NonogramSolver solveLittle(little);
	assert(solveLittle.getSolution().isSolved());

	NonogramSolver solveHarder(non);
	assert(solveHarder.getSolution().isSolved());

	Nonogram det5x5("5x5-det.txt");
	NonogramSolver det5x5Solver(det5x5);
	assert(det5x5Solver.getSolution().isSolved());

	Nonogram det5x5_2("5x5-det2.txt");
	NonogramSolver det5x5_2Solver(det5x5_2);
	assert(det5x5_2Solver.getSolution().isSolved());


	Nonogram d10x10("10x10_1.txt");
	NonogramSolver d10x10Solver(d10x10);
	assert(d10x10Solver.getSolution().isSolved());

	int trials = 5;

	timer watch_pure, watch;
	watch.start();
	watch_pure.start();
	double total = 0;
	for (int i = 0; i < trials; i++) {
		Nonogram random(5, 5);
		random.saveBoardConfig("lastRandom.txt");

		NonogramSolver randomSolver(random);
		Nonogram solution = randomSolver.getSolution();
		total += watch.get();
		if (solution != random) {
			cout << "Solve failed or multiple solutions!" << endl;
			random.saveBoardConfig(to_string(i)+".txt");
		}
		watch.start();
	}
	cout << watch_pure.get()  << endl;
	cout << total << endl;
	cout << total / (double) trials;

	system("pause");

    return 0;
}

