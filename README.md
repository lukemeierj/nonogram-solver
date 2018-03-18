# Nonogram Solver
#### By Luke Meier and Drew Hayward

## Summary
This program includes the interface and design of a Nonogram class and a NonogramSolver class that has the capability of solving Nonograms that have only one solution.  The solver has been tested up to 50x50 boards.


## Nonogram Game Interface

* Create a random nonogram with `Nonogram(width, height)` or `Nonogram(width, height, false)` if you do not want the random generation to favor clustering.
* Save a nonogram configuration to a file with `myNonogram.saveBoardConfig("filename.txt")`
* Get and set values of the nonogram board with `myNonogram.set(x,y)` and `myNonogram.get(x,y)`
* Print a nonogram to the console with either `myNon.print()` or `myNon.printWithHints()`
* Check if two boards have equal contents with `==`, or not equal with `!=`
* Get a row with `myNon.getLine(index, true)` or a column with `myNon.getLine(index, false)`
* Set a row with `myNon.setLine(index, true)` or a column with `myNon.setLine(index, false)`
* Check if your nonogram is solved with `myNon.isSolved()`
* Generate an copy of a nonogram with all the board unsolved with `myNon.emptied()`
* Create a nonogram from a file config with `Nonogram("filename.txt")`
	* To create a config file, follow this format:

	```
	[width] [height]
	[1st column constraint #1] [1st column, constraint #2]
	[2nd column constraint #1] [2nd column, constraint #2] [2nd column, constraint #3]
	.
	.
	.
	[width column constraint #1]
	[1st row constraint #1] [1st row, constraint #2]
	[2nd row constraint #1]
	.
	.
	.
	[height column constraint #1] [height column constraint #2]
	```
	* For example
	```
	ex.

	5 2
	2
	1
	1
	2
	2
	1 2
	5
	```

## Nonogram Solver

* Create a new solver agent with `NonogramSolver(myNon)`
* Attempt to solve the nonogram with `myNon.nearestSolution()`
	* This does not necessarily return a solved board in the event it cannot be solved through just constraint satisfaction.
	* To make sure a board is actually solved, `myNon.nearestSolution().isSolved()`
* To print out iterations as the solver works use `myNon.nearestSolution(true)` 
