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
	Nonogram(int width, int height);

	//make nonogram based on config file
	Nonogram(string filename);
	~Nonogram();

	//output board config to file
	void saveBoardConfig(string filename);

	//determine if a board is valid based on its hints 
	bool isSolved();

	TileType get(int x, int y);
	void set(int x, int y, TileType newVal);

private:
	int width, height;
	vector<vector<TileType>> board;
	vector<vector<int>> columnHints;
	vector<vector<int>> rowHints;

};

