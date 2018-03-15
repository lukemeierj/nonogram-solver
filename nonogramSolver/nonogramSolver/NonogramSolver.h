#include "NOnogram.h"

#pragma once
class NonogramSolver
{
public:
	NonogramSolver();
	NonogramSolver(Nonogram nonogram);
	~NonogramSolver();
	Nonogram getSolution();
	void setNonogram(Nonogram nonogram);
private:
	Nonogram nonogram;
	vector<TileType> consolidate(int index, bool rowWise);

	
};

