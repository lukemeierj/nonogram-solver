#include "stdafx.h"
#include "NonogramSolver.h"



Line::Line(vector<TileType> lineVector, unsigned int index, bool rowWise) {
	data = lineVector;
	this->index = index;
	this->rowWise = rowWise;
}
Line::Line(const Line &rhs) {
	data = rhs.data;
	this->index = rhs.index;
	this->rowWise = rhs.rowWise;
}
const unsigned int Line::size() {
	return data.size();
}
TileType& Line::operator[] (int x) {
	return data[x];
}
const bool Line::getRowWise() {
	return rowWise;
}
const unsigned int Line::getIndex() {
	return index;
}

vector<LineInfo> Line::addConstraints(vector<LineInfo> commonGround, Line line) {
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

bool Line::constrain(vector<LineInfo> commonGround, unsigned int threshold) {
	if (commonGround.size() != data.size()) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	else if (threshold <= 0) {
		return false;
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


const vector<TileType> Line::getVector() {
	return data;
}