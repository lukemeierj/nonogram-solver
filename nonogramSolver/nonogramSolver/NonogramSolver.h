#include "Nonogram.h"

#pragma once


//Information on how often a cell is empty or filled when examining multiple permutations
struct CellInfo {
	unsigned int timesEmpty = 0;
	unsigned int timesFilled = 0;
};

//This is a lightweight way to describe a line without actually storing its contents 
struct LineDescriptor {
	unsigned int index = 0;
	bool row = true;
};

class Line {
public:
	//create a line based on a vector, its position, and whether or not it is a row
	Line(vector<TileType> lineVector, unsigned int index, bool rowWise);
	//copy constructor 
	Line(const Line &rhs);
	//return the size of the line
	const unsigned int size();
	//access the raw data
	TileType& operator[] (int x);
	//get whether or not it is a row or column (rowWise == true -> it is a row)
	const bool getRowWise();
	//get the index of the row/column
	const unsigned int getIndex();
	//add constraints to a CellInfo vector.
	//this is static because it is more a task of the CellInfo vector than the line
	static vector<CellInfo> addConstraints(vector<CellInfo> commonGround, Line line);
	//set any values of the line based on the CellInfo vector, 
	//  and return a list of LineDescriptors of where values where changed
	vector<LineDescriptor> constrain(vector<CellInfo> commonGround, unsigned int threshold);
	//get the data from the Line
	const vector<TileType> getVector();
private:
	vector<TileType> data;
	unsigned int index;
	bool rowWise;

};

//this keeps track of our progress through generating a permutation 
// of possible lines
struct PartialPermutation {
	Line line;
	//how far we are in the line thus far
	unsigned int lineIndex = 0;
	//which hint we arae at thus far
	unsigned int hintIndex = 0;
	//this returns the maximum index that we should try to put our series of black cells in
	//for example, for a board 2, 1 | ? ? ? ? ? | we would only want to try 2, 1 | x x _ ? ? | and | _ x x ? ? |
	unsigned int maxIndex(vector<unsigned int> hints) {
		if (hints.size() == 0) return 0;

		unsigned int sum = 0;
		for (unsigned int i = 0; i < hints.size(); i++) {
			if(i > hintIndex)
				sum += hints[i];
		}
		//the size of the array - the number of hints left - the sum of black boxes left - the size of the current hint + 1
		return (int)line.size() - ((hints.size() - hintIndex) + sum) - hints[hintIndex] + 1;
	}
};

class NonogramSolver
{
public:
	NonogramSolver();
	//create a solver giving it a nonogram
	NonogramSolver(Nonogram nonogram);
	~NonogramSolver();
	//get a solution for a nonogram.  if debug = true, print each step as we solve
	Nonogram nearestSolution(bool debug = false);
	//set the nonogram to something new.
private:
	Nonogram nonogram;
	//this calculates permutations of possible lines and finds any common ground between all permutations
	//if there is common ground, it incorporates it into line and returns the locations changes were made
	vector<LineDescriptor> consolidate(Line &line, bool debug = false);
	
};

