/* sudoku --- An interface for a sudoku program
 * Usage --- Needs a supplementary program to run (terminal)
 * Adithya Balaji, 04-24-2018
 * Tested on an EOS Linux System
 */

#include "sudoku.h"
#include "stdlib.h"
#include "stdio.h"
#include "sudoku.h"
#include <assert.h>
#include <string.h>

struct sudoku {
	char board[9][9];
	char hints[9][9][9];
};

// global defenition of gameplay struct
struct sudoku game;

/**
 *	Converts a row to an interpretable index
 *	Args:
 *		row (char): A character representing the row
 *	Returns:
 *		The row index or -1 if out of bounds
 */
static int row_convert_check(char row)
{
	return (row >= 'A' && row <= 'I') ? (row-'A') : -1;
}

/**
 *	Converts a column to an interpretable index
 *	Args:
 *		col (int): A integer representing the column
 *	Returns:
 *		The column index or -1 if out of bounds
 */
static int col_convert_check(int col)
{
	return (col >= 1 && col <= 9) ? col-1 : -1;
}

/**
 *	Checks the valdity of an input value
 *	Args:
 *		val (char): A character representing the input item
 *	Returns:
 *		Passthrough the input or '\0' if invalid
 */
static char val_convert_check(char val)
{
	return ((val >= '1' && val <= '9') || (val == '-')) ? val : '\0';
}

/**
 *	Updates the possible values across the entire board given a specific row and
 *	column index and a known value at that specific index
 *	Args:
 *		row_i (int): A integer representing the row index
 *		col_i (int): A integer representing the column index
 *		digit (char): A character representing the value at the indicies
 */
static void update_hint(int row_i, int col_i, char digit)
{
	// Update hints
	int hint_i = digit-'1';
	int b;
	// update row
	for(b = 0; b < 9; b++)
	{
		if(b == col_i)
			continue;
		game.hints[row_i][b][hint_i] = '-';
	}
	// update col
	int a;
	for(a = 0; a < 9; a++)
	{
		if(a == row_i)
			continue;
		game.hints[a][col_i][hint_i] = '-';
	}
	// update box
	int row_start_hint = row_i/3*3;
	int col_start_hint = col_i/3*3;
	for(a = row_start_hint; a < row_start_hint+3; a++)
	{
		for(b = col_start_hint; b < col_start_hint+3; b++)
		{
			if(a == row_i && b == col_i)
				continue;
			game.hints[a][b][hint_i] = '-';
		}
	}
}

/**
 *	Updates the hint 3d array representing all of the possible values in each
 *	square by iterating through known values and removing possibilities through
 *	other squares
 */
static void update_hints()
{
	int i, j, k;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			for(k = 0; k < 9; k++)
			{
				char c;
				Sudoku_get_square((char)(i+'A'), j+1, &c);
				if(c == '-')
					continue;
				update_hint(i, j, c);
			}
		}
	}
}

/**
 *	Initializes the necessary components for a sudoku program. Should be called
 *	before any other components of a program
 *	Returns:
 *		SUDOKU_SUCCESS: Successfully initialized board
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_initialize(void)
{
	/* PUT YOUR IMPLMENTATION HERE */
	int i, j, k;
	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
			game.board[i][j] = '-';

	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
			for(k = 0; k < 9; k++)
				game.hints[i][j][k] = (char)(k+'1');

	return SUDOKU_SUCCESS;
}

/**
 *	Loads a sudoku board from a sudoku file with numbers representing the known
 *	items and '-'s representing unknowns
 *	Returns:
 *		SUDOKU_SUCCESS: Successfully loaded board
 *		SUDOKU_NO_FILE_ERROR: Unknown file or unable to load file
 *		SUDOKU_BAD_FORMAT: Error in file validation
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_loadfile(const char *filename)
{
	/* PUT YOUR IMPLMENTATION HERE */
	Sudoku_initialize();

	FILE *file = fopen(filename,"r");
	if (file == NULL)
		return SUDOKU_NO_FILE_ERROR;

	int row_i = 0;
	char *input = NULL; // row
	int max_chars = 20; // arbitrary num above 9
	input = malloc(max_chars * sizeof(char)); // max size of char input
	memset(input, 0, max_chars); // set string to all null chars
	if(fgets(input, max_chars, file) != NULL)
	{
		while(EOF != input[0]) // check next input for EOF
		{
			if(input[0] == '\0' || input[0] == '\n') // check empty input
			{
				if(fgets(input, max_chars, file) == NULL)
				{
					if(row_i != 9)
					{
						free(input);
						return SUDOKU_BAD_FORMAT;
					}
					else break;
				}
				else continue;
			}
			char *replace;
			if((replace = strchr(input, '\n')) != NULL)
				*replace = '\0';
			int str_len = strlen(input);
			if(str_len != 9)
				return SUDOKU_BAD_FORMAT;

			// validate input
			int j;
			char c;
			for(j = 0; j < 9; j++)
			{
				if((c = val_convert_check(input[j])) == '\0')
					return SUDOKU_BAD_FORMAT;
				else if(Sudoku_set_square((char)(row_i+'A'), j+1, c) 
					== SUDOKU_CONTRADICTION)
					return SUDOKU_BAD_FORMAT; // invalid load file arrangement
			}
			row_i++;
			memset(input, 0, max_chars); // clear out input to null chars
		}
	}
	else
		return SUDOKU_NO_FILE_ERROR;

	fclose(file);
	free(input);

	return SUDOKU_SUCCESS;
}

/**
 *	Saves a sudoku board state to file given a filename
 *	Args:
 *		filename (char*): A string representing the filename
 *	Returns:
 *		SUDOKU_SUCCESS: File successfully to disk
 *		SUDOKU_NO_FILE_ERROR: Was unable to successfully to create the file
 *		SUDOKU_UNKNOWN_ERROR: Unknown error
 */
int Sudoku_savefile(const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return SUDOKU_NO_FILE_ERROR;
	char *line = malloc(11 * sizeof(char));
	memset(line, 0, 11); // clear out input to null chars
	int i, j;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
			Sudoku_get_square((char)(i+'A'), j+1, line+j);
			//line[j] = game.board[i][j];

		if(i != 8)
		{
			line[9] = '\n';
			line[10] = '\0';
		}
		else
			line[9] = '\0';
		fputs(line, file);
		memset(line, 0, 11); // clear out input to null chars
	}
	free(line);
	fclose(file);

	return SUDOKU_SUCCESS;
}

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
int Sudoku_set_square(char row, int col, char digit)
{
	int row_i, col_i;
	char digit_c;
	// Validate input
	if(((row_i = row_convert_check(row)) == -1) || 
		 ((col_i = col_convert_check(col)) == -1))
		return SUDOKU_ILLEGAL_INDEX;
	if((digit_c = val_convert_check(digit)) == '\0')
		return SUDOKU_ILLEGAL_VALUE;

	if(digit == '-')
	{
		game.board[row_i][col_i] = digit;
		return SUDOKU_SUCCESS;
	}

	// Check contradictions
	char i;
	int j;
	char d;
	// check row
	for(j = 1; j <= 9; j++)
	{
		Sudoku_get_square(row, j, &d);
		if(d == digit)
			return SUDOKU_CONTRADICTION;
	}
	// check col
	for(i = 'A'; i <= 'I'; i++)
	{
		Sudoku_get_square(i, col, &d);
		if(d == digit)
			return SUDOKU_CONTRADICTION;
	}
	// check box
	char row_start = (row-'A')/3*3+'A';
	int col_start = (col-1)/3*3+1;
	for(i = row_start; i < row_start+3; i++)
		for(j = col_start; j < col_start+3; j++)
		{
			Sudoku_get_square(i, j, &d);
			if(d == digit)
				return SUDOKU_CONTRADICTION;
		}

	game.board[row_i][col_i] = digit;

	return SUDOKU_SUCCESS;
}

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
int Sudoku_get_square(char row, int col, char *value)
{
	int row_i, col_i;
	if(((row_i = row_convert_check(row)) == -1) || 
		 ((col_i = col_convert_check(col)) == -1))
		return SUDOKU_ILLEGAL_INDEX;

	*value = game.board[row_i][col_i];
	return SUDOKU_SUCCESS;
}

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
int Sudoku_give_hint(char *r, int *c, char *value)
{
	// using current board state update the hint array
	update_hints();

	// determine the hint to provide
	int i, j, k;
	int valid_options = 0;
	char candidate;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			char c_1;
			Sudoku_get_square((char)(i+'A'), j+1, &c_1);
			if(c_1 != '-')
				continue;
			for(k = 0; k < 9; k++)
			{
				char c;
				if(valid_options > 1)
					break;
				if((c = game.hints[i][j][k]) != '-')
				{
					candidate = c;
					valid_options++;
				}
			}
			if(valid_options == 1)
			{
				*r = (char)(i+'A');
				*c = j+1;
				*value = candidate;
				return SUDOKU_SUCCESS;
			}
			valid_options = 0;
		}
	}
	return SUDOKU_NO_HINT;
}

/**
 *	Pretty prints the board to the console.
 */
void Sudoku_print(void)
{
	char row = 'A', value;
	int col;

	printf("   | 1  2  3 | 4  5  6 | 7  8  9 |\n");
	for(row = 'A'; row <= 'I'; row++)
	{
		if((row-'A') % 3 == 0)
			printf("   |---------+---------+---------|\n");

		printf(" %c |", row);
		col = 1;

		for(; col <= 3; col++)
		{
			Sudoku_get_square(row, col, &value);
			printf(" %c ", value);
		}
		printf("|");
		for(; col <= 6; col++)
		{
			Sudoku_get_square(row, col, &value);
			printf(" %c ", value);
		}
		printf("|");
		for(; col <= 9; col++)
		{
			Sudoku_get_square(row, col, &value);
			printf(" %c ", value);
		}
		printf("|\n");
	}
	printf("   |---------+---------+---------|\n");
}
