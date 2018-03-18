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
		//cout << i << " ";
		Nonogram random(width, height, false);
		assert(random != prev);
		Nonogram emptyRandom = random.emptied();
		NonogramSolver randomSolver(emptyRandom);
		watch.start();
		Nonogram solution = randomSolver.getSolution();
		total += watch.get();

		if (solution == empty) {
			failed++;
			//cout << "Solve failed to solve." << endl;
			random.saveBoardConfig(to_string(width) + "x" + to_string(height) + "F" + to_string(i) + ".txt");
		}
		else if (solution != random) {
			emptyRandom.printWithHints();
			solution.printWithHints();
			failed++;
			//cout << "Incorrect solve or multiple solutions!" << endl;
			random.saveBoardConfig(to_string(width) + "x" + to_string(height) + "I" + to_string(i) + ".txt");
		}
		prev = random;

	}
	cout << "Test " << trials << " " << width << "x" << height << " nonograms." << endl;
	cout << "Total time solving " << (trials - failed) << "/" << trials << " : " << total << endl;
	cout << "Per solution: " << total / (double)trials << endl << endl;
}

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

	//Nonogram d290("290.txt");
	//NonogramSolver d290solver(d290);
	//Nonogram solution = d290solver.getSolution();

	test(50, 20, 30);
	test(50, 15, 20);
	test(50, 10, 15);



	test(50, 30, 40);
	test(50, 40, 30);


	system("pause");

    return 0;
}

