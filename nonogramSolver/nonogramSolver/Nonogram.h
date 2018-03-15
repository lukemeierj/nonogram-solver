#pragma once

#include <vector>
#include <string>

using namespace std;

//UNKNOWN = 0, FILL = 1, EMPTY = 2
enum TileType {
	UNKNOWN,
	FILL,
	EMPTY,
};

//class NonogramChunk {
//public:
//	TileType get(int i);
//
//private:
//	vector<TileType> data;
//	const vector<int> hints;
//};

class Nonogram
{
public:
	Nonogram();
	//randomly generate a width x height nonogram
	Nonogram(unsigned int width, unsigned int height);

	//make nonogram based on config file
	Nonogram(string filename);
	~Nonogram();

	//output board config to file
	void saveBoardConfig(string filename);

	//determine if a board is valid based on its hints 
	bool isSolved();

	TileType get(unsigned int x, unsigned int y);
	void set(unsigned int x, unsigned int y, TileType newVal);
	//returns a row (rowWise = true) or a column (rowWise = false)
	vector<TileType> getLine(unsigned int index, bool rowWise);

private:
	bool meetsConstraints(bool rowWise = true);
	vector<vector<unsigned int>> generateHints(bool rowWise = true);
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
	//col = 0, row = 1
	vector<vector<vector<unsigned int>>> hints;

};

