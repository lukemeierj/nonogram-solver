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
	vector<TileType> line = nonogram.getLine(index, rowWise);


	vector<LineInfo> commonGround = vector<LineInfo>(line.size());

	//for each permutation
	vector<TileType> permutation = vector<TileType>(line.size());

	addToCommonGround(permutation, commonGround);

	return incorporateCommonGround(line, commonGround, 1);
}

vector<LineInfo> NonogramSolver::addToCommonGround(vector<TileType> line, vector<LineInfo> &commonGround) {
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == FILL) {
			commonGround[i].timesFilled++;
		}
		else if (line[i] == EMPTY) {
			commonGround[i].timesEmpty++;
		}
	}
	return commonGround;
}

vector<TileType> NonogramSolver::incorporateCommonGround(vector<TileType> line, vector<LineInfo> &commonGround, unsigned int threshold) {
	if (commonGround.size() != line.size()) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == UNKNOWN) {
			if (commonGround[i].timesEmpty == threshold) {
				line[i] = EMPTY;
			}
			else if (commonGround[i].timesFilled == threshold) {
				line[i] = FILL;
			}
		}
	}
}




