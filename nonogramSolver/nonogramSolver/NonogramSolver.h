#include "Nonogram.h"

#pragma once

struct LineInfo {
	unsigned int timesEmpty = 0;
	unsigned int timesFilled = 0;
};

struct PartialPermutation {
	vector<TileType> line;
	unsigned int lineIndex = 0;
	unsigned int hintIndex = 0;
	unsigned int maxIndex(vector<unsigned int> hints) {
		unsigned int sum = 0;
		for (unsigned int i = 0; i < hints.size(); i++) {
			sum += hints[i];
		}
		return line.size() - ((hints.size() - hintIndex) + sum) - 1;
	}
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
	vector<TileType> incorporateCommonGround(vector<TileType> line, vector<LineInfo> &commonGround, unsigned int threshold);
};

