#include "stdafx.h"
#include "Nonogram.h"



Nonogram::Nonogram()
{
	width = 0;
	height = 0;

}


Nonogram::~Nonogram()
{
}

//randomly generate a width x height nonogram
Nonogram::Nonogram(unsigned int width, unsigned int height, bool bunch) {
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
	if (bunch) {
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
	int hintIndex = 0;
	while (getline(fin, line))
	{
		if (hintIndex == 0 && index == width) {
			hintIndex = 1;
			index = 0;
		}
		hints[hintIndex].push_back(vector<unsigned int>());
		std::istringstream iss(line);
		int val;
		while (iss >> val) {
			//if we have more than width constriants, put it into the row constraint list (index/width = 1)
			if(val > 0) hints[hintIndex][index].push_back(val);
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


bool Nonogram::operator==(Nonogram& rhs)const {
	if (width != rhs.getWidth() || height != rhs.getHeight()) {
		return false;
	}
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			if (board[x][y] != rhs.get(x, y)) {
				return false;
			}
		}
	}
	return true;

}

bool Nonogram::operator!=(Nonogram& rhs)const {
	return !(*this == rhs);
}

Nonogram Nonogram::emptied() {
	Nonogram newNonogram = Nonogram(*this);
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			newNonogram.set(x, y, UNKNOWN);
		}
	}
	return newNonogram;
}

void Nonogram::print() {
	for (int i = 0; i < board[0].size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			std::cout << " ";
			switch (board[j][i])
			{
			case FILL:
				std::cout << "X";
				break;
			case EMPTY:
				std::cout << "_";
				break;
			case UNKNOWN:
				std::cout << "?";
				break;
			default:
				break;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Nonogram::printWithHints() {
	if (hints.size() < 2) {
		throw std::out_of_range("Cannot print with hints if there are none.");
	}
	int maxColHints = maxHintLength(hints[0]);
	int maxRowHints = maxHintLength(hints[1]);

	// Print column hints
	for (int i = 0; i < maxColHints; i++) {
		// Prints the padding for column hints
		for (int s = 0; s < maxRowHints; s++) {
			std::cout << setw(3) << " ";
		}
		std::cout << "  ";

		// For every column
		for (int j = 0; j < hints[0].size(); j++) {
			int h = i - (maxColHints - hints[0][j].size());

			if (h >= 0) {
				cout << " " << setw(2) << hints[0][j][h];
			}
			else {
				cout << setw(3) << " ";
			}
		}

		cout << std::endl;
	}

	// Print horizontal separator
	for (int s = 0; s < maxRowHints; s++) {
		std::cout << setw(3) << " ";
	}
	std::cout << "  ";
	// For every column
	for (int j = 0; j < board[0].size(); j++) {
		cout << "--";
	}
	std::cout << std::endl;

	// Print row hints and board
	for (int i = 0; i < board[0].size(); i++) {
		// Print hints row
		for (int j = 0; j < maxRowHints; j++) {
			int h = j -(maxRowHints - hints[1][i].size());

			if (h >= 0) {
				std::cout << " " << setw(2) << hints[1][i][h];
			}
			else {
				std::cout << setw(3) << " ";
			}
		}

		// Print vertical hints separator
		std::cout << " |";

		// Print board row
		for (int j = 0; j < board.size(); j++) {
			std::cout << " ";
			switch (board[j][i])
			{
			case FILL:
				std::cout << setw(2) << " X";
				break;
			case EMPTY:
				std::cout << setw(2) << " _";
				break;
			case UNKNOWN:
				std::cout << setw(2) << " ?";
				break;
			default:
				break;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int Nonogram::maxHintLength(vector<vector<unsigned int>> hintList) {
	int max = 0;
	for (int i = 0; i < hintList.size(); i ++) {
		if (max < hintList[i].size()) {
			max = hintList[i].size();
		}
	}
	return max;
}
