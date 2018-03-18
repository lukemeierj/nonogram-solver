#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>    
#include <time.h>  
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

enum TileType {
	UNKNOWN,
	FILL,
	EMPTY,
};

class Nonogram
{
public:
	//make empty, 0x0 nonogram
	Nonogram();
	//randomly generate a width x height nonogram
	Nonogram(unsigned int width, unsigned int height, bool bunch = true);

	//make nonogram based on config file
	Nonogram(string filename);
	~Nonogram();

	//output board config to file
	void saveBoardConfig(string filename);

	//determine if a board is valid based on its hints 
	bool isSolved();

	unsigned int getWidth() { return width; }
	unsigned int getHeight() { return height; }

	//get a position x,y on the board
	TileType get(unsigned int x, unsigned int y);

	//set a position x,y on the board
	void set(unsigned int x, unsigned int y, TileType newVal);

	//returns a row (rowWise = true) or a column (rowWise = false)
	vector<TileType> getLine(unsigned int index, bool rowWise);

	//returns the hints at a given row (rowWise == true) or column (rowWise == false)
	vector<unsigned int> getHints(unsigned int index, bool rowWise);

	//sets a line in the board at index "index" as newLine, either a row (rowWise == true) or col (rowWise == false)
	void setLine(vector<TileType> newLine, unsigned int index, bool rowWise);
	//checks if two nonograms have equal boards 
	bool operator==(Nonogram& rhs)const;
	bool operator!=(Nonogram& rhs)const;
	//return a nonogram with a board that is all UNKNOWN
	Nonogram emptied();
	//print off a board's contents
	void print();
	//print off a board's contents with it's hints 
	void printWithHints();

private:
	//get the max number of hints in the nonogram based on a vector of all the hints rowWise or column-wise
	int maxHintLength(vector<vector<unsigned int>>);
	//checks that all the constraints are met either  row by row (rowWise == true) or column by column
	bool meetsConstraints(bool rowWise = true);
	//this returns all the hints based on a given board for rows (rowWise == true) or columns
	vector<vector<unsigned int>> generateHints(bool rowWise = true);
	//return the transpose of the board as a copy
	vector<vector<TileType>> getTranspose();
	unsigned int width, height;
	//board ordered like this
	/*
		(0,0) (1,0) (2,0)
		(0,1) (1,1) (2,1)
		(0,2) (1,2) (2,2)
		(0,3) (1,3) (2,3)
		where each pair is (x,y)
	*/
	vector<vector<TileType>> board;
	//index = 0 -> the column hints, index = 1 -> the row hints 
	vector<vector<vector<unsigned int>>> hints;

};

