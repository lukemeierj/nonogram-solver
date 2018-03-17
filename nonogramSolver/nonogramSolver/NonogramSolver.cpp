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
	queue<LineDescriptor> searchQueue = queue<LineDescriptor>();

	for (unsigned int i = 0; i < nonogram.getHeight(); i++) {
		searchQueue.push({ i, true });
	}
	for (unsigned int i = 0; i < nonogram.getWidth(); i++) {
		searchQueue.push({ i, false });
	}

	while (!searchQueue.empty()) {
		LineDescriptor element = searchQueue.front();
		searchQueue.pop();
		Line line = Line(nonogram.getLine(element.index, element.row), element.index, element.row);
		vector<LineDescriptor> revisions = consolidate(line);
		if(revisions.size() > 0) {
			nonogram.setLine(line.getVector(), line.getIndex(), line.getRowWise());
			for (unsigned int i = 0; i < revisions.size(); i++) {
				searchQueue.push(revisions[i]);
			}
		}
	}

	if (nonogram.isSolved()) {
		return nonogram;
	}
	else {
		//Backtrack search
		return Nonogram();
	}

}



vector<LineDescriptor> NonogramSolver::consolidate(Line &line) {
	vector<unsigned int> hints = nonogram.getHints(line.getIndex(), line.getRowWise());

	vector<LineInfo> commonGround = vector<LineInfo>(line.size());

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
			for (int cursor = (int)permutation.lineIndex - start; cursor < (int)hints[hintIndex]; cursor++) {
				if (cursor < 0) {
					if (newLine[start + cursor] == FILL) goto newPerm;
					newLine[start + cursor] = EMPTY;
				} 
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

	return line.constrain(commonGround, numValidPermutations);
}






