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

//increments the number of filled or empty times for a given CellInfo list based on a line
vector<CellInfo> Line::addConstraints(vector<CellInfo> commonGround, Line line) {
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

//this sets some cells values based on a CellInfo list of common ground
vector<LineDescriptor> Line::constrain(vector<CellInfo> commonGround, unsigned int threshold) {

	vector<LineDescriptor> revisions = vector<LineDescriptor>();
	
	if (commonGround.size() != data.size()) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	else if (threshold <= 0) {
		return revisions;
	}

	//for each cell in the line
	for (unsigned int i = 0; i < data.size(); i++) {
		bool revised = false;
		//if it is unknown, and also the timesEmpty or timeFill meets the threshold, 
		//  set it to EMPTY or FILL respectively
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
		//add the row or column that was modified
		if (revised) {
			revisions.push_back({ i, !rowWise });
		}
	}
	//if there were modifications, add this line to the set of revised elements
	if (revisions.size() > 0) revisions.push_back({ index, rowWise });
	return revisions;
}


const vector<TileType> Line::getVector() {
	return data;
}
