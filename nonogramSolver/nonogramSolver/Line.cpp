#include "stdafx.h"
#include "NonogramSolver.h"



Line::Line(vector<TileType> lineVector, unsigned int index, bool rowWise) {
	data = lineVector;
	commonGround = vector<LineInfo>(lineVector.size());
	this->index = index;
	this->rowWise = rowWise;
}
Line::Line(const Line &rhs) {
	data = rhs.data;
	commonGround = vector<LineInfo>(rhs.commonGround);
}
unsigned int Line::size() {
	return data.size();
}
TileType& Line::operator[] (int x) {
	return data[x];
}
bool Line::getRowWise() {
	return rowWise;
}
unsigned int Line::getIndex() {
	return index;
}


void Line::addConstraints() {
	for (unsigned int i = 0; i < data.size(); i++) {
		if (data[i] == FILL) {
			commonGround[i].timesFilled++;
		}
		else if (data[i] == EMPTY) {
			commonGround[i].timesEmpty++;
		}
	}
}

bool Line::constrain(unsigned int threshold) {
	if (commonGround.size() != data.size()) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	//TODO: Make this show contradictions!  Can't set as FILL if already set as EMPTY
	bool revised = false;
	for (unsigned int i = 0; i < data.size(); i++) {
		if (data[i] == UNKNOWN) {
			if (commonGround[i].timesEmpty == threshold) {
				data[i] = EMPTY;
				revised = true;
			}
			else if (commonGround[i].timesFilled == threshold) {
				data[i] = FILL;
				revised = true;
			}
		}
	}
	return revised;
}


