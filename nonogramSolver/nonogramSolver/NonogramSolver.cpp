#include "stdafx.h"
#include "NonogramSolver.h"


NonogramSolver::NonogramSolver()
{
}

NonogramSolver::NonogramSolver(Nonogram nonogram) {
	this->nonogram = nonogram;
}


NonogramSolver::~NonogramSolver()
{
}

void NonogramSolver::setNonogram(Nonogram nonogram) {
	this->nonogram = nonogram;
}


Nonogram NonogramSolver::getSolution() {
	//rowWise means we look at the rows, if rowWise is false we look at columns
	//we want to call consolidate on each row and column, 
	//then set the nonogram's board values to the values consolidate returns for that row/column
	//so, for example, the first column = consolidate(0, false)
	//first row = consolidate(0, false);
	return Nonogram();
}

vector<TileType> NonogramSolver::consolidate(int index, bool rowWise) {
	return vector<TileType>(5, UNKNOWN);
}


