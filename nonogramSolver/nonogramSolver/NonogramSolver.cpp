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


Nonogram NonogramSolver::getSolution() {
	queue<LineQueue> searchQueue = queue<LineQueue>();

	for (unsigned int i = 0; i < nonogram.getHeight(); i++) {
		searchQueue.push({ i, true });
	}
	for (unsigned int i = 0; i < nonogram.getWidth(); i++) {
		searchQueue.push({ i, false });
	}

	while (!searchQueue.empty()) {
		LineQueue element = searchQueue.front();
		searchQueue.pop();
		//nonogram.setLine();
	}
	//rowWise means we look at the rows, if rowWise is false we look at columns
	//we want to call consolidate on each row and column, 
	//then set the nonogram's board values to the values consolidate returns for that row/column
	//so, for example, the first column = consolidate(0, false)
	//first row = consolidate(0, false);
	return Nonogram();
}



bool NonogramSolver::consolidate(Line &line) {
	vector<unsigned int> hints = nonogram.getHints(line.getIndex(), line.getRowWise());


	stack<PartialPermutation> permutationStack = stack<PartialPermutation>();

	int numValidPermutations = 0;

	permutationStack.push({ Line(line), 0, 0 });

	while (!permutationStack.empty()) {
		PartialPermutation permutation = permutationStack.top();
		permutationStack.pop();
		//If permutation on stack is valid and complete, add to common ground
		if (permutation.hintIndex == hints.size() && permutation.lineIndex >= permutation.line.size()) {
			line.addConstraints();
			numValidPermutations++;
			continue;
		}

		//For each possible start position of the hint
		for (int start = permutation.lineIndex; hints.size() != 0 && start <= permutation.maxIndex(hints); start++) {
			newPerm:
			unsigned int newIndex = 0, hintIndex = permutation.hintIndex;
			Line newLine = Line(permutation.line);
			for (int cursor = (int)permutation.lineIndex - start; cursor < (int)hints[hintIndex]; cursor++) {
				if (newLine[start + cursor] == EMPTY) {
					goto newPerm;
				}
				else if(cursor >= 0) {
					newLine[start + cursor] = FILL;
				}
				else {
					newLine[start + cursor] = EMPTY;
				}
				newIndex = start + cursor;
			}
			//The block of FILL cells continues past the hint
			if ((int)newIndex < (int)newLine.size()-1 && newLine[newIndex + 1] == FILL) {
				goto newPerm;
			}
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
			else {
				newLine[newIndex + 1] = EMPTY;
			}
			permutationStack.push({ newLine, newIndex + 2, hintIndex });
		}


	}

	return line.constrain(numValidPermutations);
}






