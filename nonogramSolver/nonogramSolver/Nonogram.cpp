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
	this->width = width;
	this->height = height;
	board.resize(width);
	for (unsigned int i = 0; i < board.size(); i++) {
		board[i].resize(height, UNKNOWN);
	}


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
	//column constraints 
	hints.push_back(generateHints(false));
	//row constraints
	hints.push_back(generateHints(true));

}

//make nonogram based on config file
Nonogram::Nonogram(string filename) {
	ifstream fin(filename);
	string line;

	fin >> width >> height;
	getline(fin, line);
	board.resize(width);
	for (unsigned int i = 0; i < board.size(); i++) {
		board[i].resize(height, UNKNOWN);
	}
	hints.resize(2);
	unsigned int index = 0;
	//read each new line as a set of constraints, put them into hint list
	while (getline(fin, line))
	{
		hints[index / width].push_back(vector<unsigned int>());
		std::istringstream iss(line);
		int val;
		while (iss >> val) {
			//if we have more than width constriants, put it into the row constraint list (index/width = 1)
			if(val > 0) hints[index / width][index%width].push_back(val);
		}
		index++;
	}
	if ((index-1) > (width + height)) {
		throw std::length_error("Too many constraints given the width/height");
	}
}

TileType Nonogram::get(unsigned int x, unsigned int y) {
	if (x >= width || y >= height || x < 0 || y < 0) {
		throw std::out_of_range("Getting board element outside of range.");
	}
	return board[x][y];
}
void Nonogram::set(unsigned int x, unsigned int y, TileType newVal) {
	if (x >= width || y >= height || x < 0 || y < 0) {
		throw std::out_of_range("Setting board element outside of range.");
	}
	board[x][y] = newVal;
}

//output board config to file
void Nonogram::saveBoardConfig(string filename) {
	ofstream fout(filename);
	fout << width << " " << height << endl;

	//for each set of hints, output a line with each set of constraints
	//there should be width + height constraints at the end 
	for (unsigned int dimension = 0; dimension <= 1; dimension++) {
		for (unsigned int chunk = 0; chunk < hints[dimension].size(); chunk++) {
			for (unsigned int hintIndex = 0; hintIndex < hints[dimension][chunk].size(); hintIndex++) {
				fout << hints[dimension][chunk][hintIndex] << " ";
			}
			fout << endl;
		}
	}

	fout.close();
}

//TODO: Test
//determine if a board is valid based on its hints 
bool Nonogram::isSolved() {
	return meetsConstraints(false) && meetsConstraints(true);
}

bool Nonogram::meetsConstraints(bool rowWise) {
	if (board.size() == 0 || board[0].size() == 0) {
		throw std::out_of_range("Cannot have a 1D or null board.");
	}
	
	vector<vector<TileType>> testBoard;
	vector<vector<unsigned int>> testHints = vector<vector<unsigned int>>(rowWise ? hints[1] : hints[0]);

	int hintIndex = 0;
	if (rowWise) {
		testBoard = this->getTranspose();
	}
	else {
		testBoard = vector<vector<TileType>>(board);
	}
	for (unsigned int x = 0; x < testBoard.size(); x++) {
		TileType prev = UNKNOWN;
		for (unsigned int y = 0; y < testBoard[0].size(); y++) {
			TileType tile = testBoard[x][y];
			//if any cell is undefined, 
			//	 return false
			if (tile == UNKNOWN) {
				return false;
			}
			//if have a filled cell
			//  if we have run out of rules
			//  or if we are in a block that is too long for the constraint
			//		return false
			else if (tile == FILL) {
				if (testHints[x].size() < (hintIndex+1) || testHints[x][hintIndex] <= 0) {
					return false;
				}
				testHints[x][hintIndex]--;
			}
			else if (tile == EMPTY) {
				//if there are no rules, don't return false
				// if there are rules, 
				//		if we have run out of rules 
				//			or if we have not completed a constraint
				//			return false
				if (testHints[x].size() != 0 && prev == FILL && (testHints[x].size() < (hintIndex + 1) || testHints[x][hintIndex] > 0)) {
					return false;
				}
				else if (prev == FILL) {
					hintIndex++;
				}
			}
			prev = tile;
		}
		if (testHints[x].size() != 0 && testHints[x].size() > (hintIndex + 1) && testHints[x][hintIndex] != 0) return false;
		hintIndex = 0;
	}
	return true;
}

vector<vector<unsigned int>> Nonogram::generateHints(bool rowWise) {
	vector<vector<TileType>> testBoard;
	vector<vector<unsigned int>> hints = vector<vector<unsigned int>>();
	if (rowWise) {
		testBoard = this->getTranspose();
	}
	else {
		testBoard = vector<vector<TileType>>(board);
	}
	TileType prev = UNKNOWN;
	for (unsigned int x = 0; x < testBoard.size(); x++) {
		vector<unsigned int> lineHints = vector<unsigned int>();
		unsigned int hint = 0;
		for (unsigned int y = 0; y < testBoard[0].size(); y++) {
			TileType tile = testBoard[x][y];
			if (tile == UNKNOWN) {
				throw std::domain_error("Cannot generate constraints of partially unknown puzzle solution.");
			}
			else if (tile == FILL) {
				hint++;
			}
			else if (tile == EMPTY) {
				if (hint > 0) {
					lineHints.push_back(hint);
					hint = 0;
				}
			}
		}
		if (hint > 0) lineHints.push_back(hint);
		hints.push_back(lineHints);
	}
	return hints;
}

vector<vector<TileType>> Nonogram::getTranspose() {
	vector<vector<TileType>> transpose = vector<vector<TileType>>(height);
	for (int i = 0; i < height; i++) {
		transpose[i].resize(width);
	}
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			transpose[y][x] = board[x][y];
		}
	}
	return transpose;
}

vector<TileType> Nonogram::getLine(unsigned int index, bool rowWise) {
	if (rowWise) {
		if (board.size() <= 0 || index > board[0].size()) {
			throw std::out_of_range("Getting board element outside of range.");
		}
		vector<TileType> result;
		for (unsigned int i = 0; i < board.size(); i++) {
			result.push_back(board[i][index]);
		}
		return result;
	}
	else {
		if (index > board.size()) {
			throw std::out_of_range("Getting board element outside of range.");
		}
		return board[index];
	}
}

vector<unsigned int> Nonogram::getHints(unsigned int index, bool rowWise) {
	return (rowWise ? hints[1][index] : hints[0][index]);
}
 
void Nonogram::setLine(vector<TileType> newLine, unsigned int index,  bool rowWise) {
	if (rowWise) {
		if (newLine.size() != board.size()) {
			throw std::out_of_range("Getting board element outside of range.");
		}
		for (unsigned int i = 0; i < board.size(); i++) {
			board[i][index] = newLine[i];
		}
	}
	else {
		if (board.size() <= 0 || index >= board.size() || newLine.size() != board[0].size()) {
			throw std::out_of_range("Getting board element outside of range.");
		}
		for (unsigned int i = 0; i < board[0].size(); i++) {
			board[index][i] = newLine[i];
		}
	}
}
