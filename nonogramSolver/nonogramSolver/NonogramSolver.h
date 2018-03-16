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
	Line(vector<TileType> lineVector, unsigned int index, bool rowWise) {
		data = lineVector;
		commonGround = vector<LineInfo>(lineVector.size());
		this->index = index;
		this->rowWise = rowWise;
	}
	Line(const Line &rhs) {
		data = rhs.data;
		commonGround = vector<LineInfo>(rhs.commonGround);
	}
	unsigned int size() {
		return data.size();
	}
	TileType& operator[] (int x) {
		return data[x];
	}
	bool getRowWise() {
		return rowWise;
	}
	unsigned int getIndex() {
		return index;
	}
	void addConstraints();
	bool constrain(unsigned int threshold);
private:
	vector<LineInfo> commonGround;
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

