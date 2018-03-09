#include "stdafx.h"
#include "Nonogram.h"


Nonogram::Nonogram()
{
}


Nonogram::~Nonogram()
{
}

//randomly generate a width x height nonogram
Nonogram::Nonogram(int width, int height) {

}

//make nonogram based on config file
Nonogram::Nonogram(string filename) {

}

TileType Nonogram::get(int x, int y) {
	if (x >= width || y >= height || x == 0 || y == 0) {
		throw std::out_of_range("Getting board element outside of range.");
	}
	return board[x][y];
}
void Nonogram::set(int x, int y, TileType newVal) {
	if (x >= width || y >= height || x == 0 || y == 0) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	board[x][y] = newVal;
}

//output board config to file
void Nonogram::saveBoardConfig(string filename) {

}

//TODO: Test
//determine if a board is valid based on its hints 
bool Nonogram::isSolved() {

	vector<vector<int>> testRowHints = vector<vector<int>>(rowHints);
	int rowHintIndex = 0;
	TileType prev = UNKNOWN;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			TileType tile = get(col, row);
			if (tile == UNKNOWN) {
				return false;
			}
			else if (tile == FILL) {
				testRowHints[row][rowHintIndex]--;
				if (testRowHints[row][rowHintIndex] < 0) {
					return false;
				}
			}
			else if (tile == EMPTY) {
				if (testRowHints[row][rowHintIndex] > 0) {
					return false;
				}
				else if (prev == FILL) {
					rowHintIndex++;
				}
			}
			prev = tile;
		}
		rowHintIndex = 0;
	}

	vector<vector<int>> testColumnHints = vector<vector<int>>(columnHints);
	int columnHintIndex = 0;
	TileType prev = UNKNOWN;
	for (int col = 0; col < width; col++) {
		for (int row = 0; row < height; row++) {
			TileType tile = get(col, row);
			if (tile == UNKNOWN) {
				return false;
			}
			else if (tile == FILL) {
				testColumnHints[col][columnHintIndex]--;
				if (testColumnHints[col][columnHintIndex] < 0) {
					return false;
				}
			}
			else if (tile == EMPTY) {
				if (testColumnHints[col][columnHintIndex] > 0) {
					return false;
				}
				else if (prev == FILL) {
					columnHintIndex++;
				}
			}
			prev = tile;
		}
		columnHintIndex = 0;
	}
}