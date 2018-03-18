#include "Nonogram.h"

#pragma once


//Information on
struct CellInfo {
	unsigned int timesEmpty = 0;
	unsigned int timesFilled = 0;
};

struct LineDescriptor {
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
	static vector<CellInfo> addConstraints(vector<CellInfo> commonGround, Line line);
	vector<LineDescriptor> constrain(vector<CellInfo> commonGround, unsigned int threshold);
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
	Nonogram getSolution(bool debug = false);
	void setNonogram(Nonogram nonogram);
private:
	Nonogram nonogram;
	vector<LineDescriptor> consolidate(Line &line, bool debug = false);
	
};

