#include "stdafx.h"
#include "NonogramSolver.h"
#include <stack>
#include <queue>



NonogramSolver::NonogramSolver()
{
}

NonogramSolver::NonogramSolver(Nonogram nonogram) {
	this->nonogram = nonogram;
}


NonogramSolver::~NonogramSolver()
{
}

void NonogramSolver::setNonogram(Nonogram nonogram) {
	this->nonogram = nonogram;
}


Nonogram NonogramSolver::getSolution(bool debug) {
	queue<LineDescriptor> searchQueue = queue<LineDescriptor>();

	//add all rows and columns to the queue
	for (unsigned int i = 0; i < nonogram.getHeight(); i++) {
		searchQueue.push({ i, true });
	}
	for (unsigned int i = 0; i < nonogram.getWidth(); i++) {
		searchQueue.push({ i, false });
	}

	//while there are still rows and columns to constrain 
	while (!searchQueue.empty()) {
		LineDescriptor element = searchQueue.front();
		searchQueue.pop();

		Line line = Line(nonogram.getLine(element.index, element.row), element.index, element.row);
		//find the revisions possible
		vector<LineDescriptor> revisions = consolidate(line, debug);
		if(debug) nonogram.printWithHints();
		if(revisions.size() > 0) {
			//incorporate the revised line
			nonogram.setLine(line.getVector(), line.getIndex(), line.getRowWise());
			//add the columns and rows that now have new information
			for (unsigned int i = 0; i < revisions.size(); i++) {
				searchQueue.push(revisions[i]);
			}
		}
	}

	//if solved, return.
	if (nonogram.isSolved()) {
		return nonogram;
	}
	else {
		//Backtrack search
		return nonogram;
	}

}



vector<LineDescriptor> NonogramSolver::consolidate(Line &line, bool debug) {

	//if a line is fully solved, don't bother with permutations.  
	bool lineAlreadySolved = true;
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == UNKNOWN) {
			lineAlreadySolved = false;
			break;
		}
	}
	if (lineAlreadySolved) return vector<LineDescriptor>();


	vector<unsigned int> hints = nonogram.getHints(line.getIndex(), line.getRowWise());

	//keep track of the common ground between each permutation
	vector<CellInfo> commonGround = vector<CellInfo>(line.size());

	stack<PartialPermutation> permutationStack = stack<PartialPermutation>();

	int numValidPermutations = 0;

	permutationStack.push({ Line(line), 0, 0 });

	while (!permutationStack.empty()) {
		PartialPermutation permutation = permutationStack.top();
		permutationStack.pop();
		//If permutation on stack is valid and complete, add to common ground
		if (permutation.hintIndex == hints.size() && permutation.lineIndex >= permutation.line.size()) {
			commonGround = Line::addConstraints(commonGround, permutation.line);
			numValidPermutations++;
			continue;
		} 
		int start = permutation.lineIndex-1;
		newPerm:
		start++;
		//For each possible start position of the hint
		for (; hints.size() != 0 && start <= permutation.maxIndex(hints); start++) {
			unsigned int newIndex = 0, hintIndex = permutation.hintIndex;
			Line newLine = Line(permutation.line);
			//start at the beginning of the permutation
			//fill everything beforehand as EMPTY
			//fill in hints[hintIndex] many FILL squares, then a trailing EMPTY cell
			for (int cursor = (int)permutation.lineIndex - start; cursor < (int)hints[hintIndex]; cursor++) {
				//if we aren't at the permutation yet, fill EMPTY
				if (cursor < 0) {
					//If trying to fill empty, but are already FILL, permutation is impossible
					//so we stop trying, and go to the next permutation
					if (newLine[start + cursor] == FILL) goto newPerm;
					newLine[start + cursor] = EMPTY;
				} 
				//if we want to fill it but it's empty
				else if (newLine[start + cursor] == EMPTY) {
					goto newPerm;
				}
				else {
					if (newLine[start + cursor] == EMPTY) goto newPerm;
					newLine[start + cursor] = FILL;
				}
				newIndex = start + cursor;
			}
			//The block of FILL cells continues past the hint
			if ((int)newIndex < (int)newLine.size()-1 && newLine[newIndex + 1] == FILL) {
				goto newPerm;
			}
			//if we are at end of the last hint, fill everything with EMPTY
			else if (++hintIndex == hints.size()) {
				for (++newIndex; newIndex < newLine.size(); newIndex++) {
					if (newLine[newIndex] == FILL) {
						goto newPerm;
					}
					else {
						newLine[newIndex] = EMPTY;
					}
				}
				//Begin next hint after +2 cells.  +1 for the EMPTY cell after a block, and +1 for beginning after that
			}
			//fill the cell right after the hint with EMPTY
			else {
				newLine[newIndex + 1] = EMPTY;
			}
			//add the the stack of partial permutations
			permutationStack.push({ newLine, newIndex + 2, hintIndex });
		}


	}

	//if the numValidPermutations is 0, then we might have a contradiction.  
	//  we could backtrack here if we implement backtracking search.

	return line.constrain(commonGround, numValidPermutations);
}






