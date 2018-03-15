#include "Nonogram.h"

#pragma once

struct LineInfo {
	unsigned int timesEmpty = 0;
	unsigned int timesFilled = 0;
};

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
	vector<LineInfo> addToCommonGround(vector<TileType> line, vector<LineInfo> &commonGround);
	vector<TileType> incorporateCommonGround(vector<TileType> line, vector<LineInfo> &commonGround, unsigned int threshold);

};

