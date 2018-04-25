#ifndef SUDOKU_H
#define SUDOKU_H

#define SUDOKU_SUCCESS			0
#define SUDOKU_NO_FILE_ERROR	1
#define SUDOKU_UNKNOWN_ERROR	2
#define SUDOKU_CONTRADICTION	3
#define SUDOKU_ILLEGAL			4
#define SUDOKU_NO_HINT			5
#define SUDOKU_BAD_FORMAT		6
#define SUDOKU_SOLVED			7
#define SUDOKU_ILLEGAL_INDEX	8
#define SUDOKU_NO_SOLUTION		9
#define SUDOKU_ILLEGAL_VALUE	10
#define SUDOKU_ERROR			99
#define SUDOKU_MAX_ERROR		100

/**
 *	Initializes the necessary components for a sudoku program. Should be called
 *	before any other components of a program
 *	Returns:
 *		SUDOKU_SUCCESS: Successfully initialized board
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_initialize (void);

/**
 *	Loads a sudoku board from a sudoku file with numbers representing the known
 *	items and '-'s representing unknowns
 *	Returns:
 *		SUDOKU_SUCCESS: Successfully loaded board
 *		SUDOKU_NO_FILE_ERROR: Unknown file or unable to load file
 *		SUDOKU_BAD_FORMAT: Error in file validation
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_loadfile (const char *filename);


/**
 *	Saves a sudoku board state to file given a filename
 *	Args:
 *		filename (char*): A string representing the filename
 *	Returns:
 *		SUDOKU_SUCCESS: File successfully to disk
 *		SUDOKU_NO_FILE_ERROR: Was unable to successfully to create the file
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_savefile (const char *filename);

/**
 *	Set the value of a square given the specific position with a letter from 'A' to
 *	'I' representing the row and a number from 1 to 9 representing the column with
 *	the value being a character from '1' to '9' or '-'
 *	Args:
 *		row (char): A character representing the row ('A' to 'I')
 *		col (int): An integer representing the col (1 to 9)
 *		digit (char): A character representing the col ('1' to '9' or '-')
 *	Returns:
 *		SUDOKU_SUCCESS: Successfully updated board
 *		SUDOKU_CONTRADICTION: The update would contradict the current board state
 *							  violating the rules of Sudoku
 *		SUDOKU_ILLEGAL_VALUE: The input value doesn't match the range of viable
 *							  input values
 *		SUDOKU_ILLEGAL_INDEX: The input indicies do not match the range of viable
 *							  range of indicies
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_set_square (char row, int col, char digit);

/**
 *	Gets the value of square from the given indicies and stores it to a provided
 *	pointer representing a character
 *	Args:
 *		row (char): A character representing the row ('A' to 'I')
 *		col (int): An integer representing the col (1 to 9)
 *		digit (char*): A character pointer representing the value at the index
 *	Returns:
 *		SUDOKU_SUCCESS: Value was successfully retrieved
 *		SUDOKU_ILLEGAL_INDEX: The input indicies do not match the range of viable
 *							  range of indicies
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_get_square (char row, int col, char *value);

/**
 *	Returns a hint by iterating through the board determining possible values in 
 *  each of the squares. If a square only has a single possible value, it is
 *  returned as a hint.
 *	Args:
 *		row (char*): A character pointer representing the row of hint
 *		col (int*): An integer pointer representing the column of hint
 *		digit (char*): A character pointer representing the value of the hint
 *	Returns:
 *		SUDOKU_SUCCESS: Hint was found
 *		SUDOKU_NO_HINT: Was unable to find a hint to provide
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_give_hint (char *row, int *col, char *value);

/**
 *	Pretty prints the board to the console.
 */
void Sudoku_print (void);

#endif /* SUDOKU_H */
