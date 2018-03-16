#include "Nonogram.h"

#pragma once

struct LineInfo {
	unsigned int timesEmpty = 0;
	unsigned int timesFilled = 0;
};

struct LineQueue {
	unsigned int index = 0;
	bool row = true;
};

class Line {
public:
	Line(vector<TileType> lineVector, unsigned int index, bool rowWise);
	Line(const Line &rhs);
	const unsigned int size();
	TileType& operator[] (int x);
	const bool getRowWise();
	const unsigned int getIndex();
	static vector<LineInfo> addConstraints(vector<LineInfo> commonGround, Line line);
	bool constrain(vector<LineInfo> commonGround, unsigned int threshold);
	const vector<TileType> getVector();
private:
	vector<TileType> data;
	unsigned int index;
	bool rowWise;

};

struct PartialPermutation {
	Line line;
	unsigned int lineIndex = 0;
	unsigned int hintIndex = 0;
	unsigned int maxIndex(vector<unsigned int> hints) {
		if (hints.size() == 0) return 0;

		unsigned int sum = 0;
		for (unsigned int i = 0; i < hints.size(); i++) {
			if(i > hintIndex)
				sum += hints[i];
		}

		return (int)line.size() - ((hints.size() - hintIndex) + sum) - hints[hintIndex] + 1;
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
	bool consolidate(Line &line);
	
};

