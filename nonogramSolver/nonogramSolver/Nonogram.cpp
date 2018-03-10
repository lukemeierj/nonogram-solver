#include "stdafx.h"
#include "Nonogram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>    
#include <time.h>  


Nonogram::Nonogram()
{
	width = 0;
	height = 0;

}


Nonogram::~Nonogram()
{
}

//randomly generate a width x height nonogram
Nonogram::Nonogram(unsigned int width, unsigned int height) {
	srand(time(NULL));
	for (unsigned int col = 0; col < width; col++) {
		for (unsigned int row = 0; row < height; row++) {
			if (rand() % 100 > 33) {
				set(col, row, FILL);
			}
			else {
				set(col, row, EMPTY);
			}
		}
	}
	//Run through the board again, give empty cells a chance to be filled if their neighbors are filled.
	for (unsigned int col = 0; col < width; col++) {
		for (unsigned int row = 0; row < height; row++) {
			unsigned int neighbors = 0;
			if (get(col, row) == EMPTY) {
				set(col, row, FILL);
				for (unsigned int x = col - 1; x < width && x >= 0 && x <= col + 1; x++) {
					for (unsigned int y = row - 1; y < height && y >= 0 && y <= row + 1; y++) {
						if (get(x, y) == FILL) {
							neighbors++;
						}
					}
				}
				//Higher chance of being filled with more neighbors being filled 
				if (rand() % 100 < neighbors * 10) {
					set(col, row, FILL);
				}
			}
		}
	}

}

//make nonogram based on config file
Nonogram::Nonogram(string filename) {
	ifstream fin(filename);
	fin >> width >> height;
	string line;
	unsigned int index = 0;
	//read each new line as a set of constraints, put them into hint list
	while (getline(fin, line))
	{
		std::istringstream iss(line);
		int val;
		while (iss >> val) {
			//if we have more than width constriants, put it into the row constraint list (index/width = 1)
			hints[index / width][index%width].push_back(val);
		}
		index++;
	}
	if (index > (width + height)) {
		throw std::length_error("Too many constraints given the width/height");
	}
}

TileType Nonogram::get(unsigned int x, unsigned int y) {
	if (x >= width || y >= height || x == 0 || y == 0) {
		throw std::out_of_range("Getting board element outside of range.");
	}
	return board[x][y];
}
void Nonogram::set(unsigned int x, unsigned int y, TileType newVal) {
	if (x >= width || y >= height || x == 0 || y == 0) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	board[x][y] = newVal;
}

//output board config to file
void Nonogram::saveBoardConfig(string filename) {
	ofstream fout(filename);
	cout << width << " " << height << endl;

	//for each set of hints, output a line with each set of constraints
	//there should be width + height constraints at the end 
	for (unsigned int dimension = 0; dimension <= 1; dimension++) {
		for (unsigned int chunk = 0; chunk < hints[dimension].size(); chunk++) {
			for (unsigned int hintIndex = 0; hintIndex < hints[dimension][chunk].size(); hintIndex++) {
				cout << hints[dimension][chunk][hintIndex] << " ";
			}
			cout << endl;
		}
	}

	fout.close();
}

//TODO: Test
//determine if a board is valid based on its hints 
bool Nonogram::isSolved() {

	vector<vector<unsigned int>> testRowHints = vector<vector<unsigned int>>(hints[1]);
	int rowHintIndex = 0;
	TileType prev = UNKNOWN;
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
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

	vector<vector<unsigned int>> testColumnHints = vector<vector<unsigned int>>(hints[0]);
	int columnHintIndex = 0;
	prev = UNKNOWN;
	for (unsigned int col = 0; col < width; col++) {
		for (unsigned int row = 0; row < height; row++) {
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