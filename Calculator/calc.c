/* calc --- A simple calculator with multiple features
 * Usage --- ./calc
 * Adithya Balaji, 02-05-2018
 * Tested on a EOS Linux System
 */

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *	A function that finds the binary representation of an integer
 *	Args:
 *		a (int): The integer that is to be binarized
 *		bin_str (char**): A string pointer is allocated representing the string to 
 *						  represent the binary form of an integer
 *						  Note: callee must deallocate bin_str
 */
void binaryString(int a, char** bin_str)
{
	int bit_count = sizeof(int) * CHAR_BIT; // max size of string rep of int
	*bin_str = (char*)malloc((bit_count+1) * sizeof(char)); // alloc string pointer
	memset(*bin_str, 0, bit_count+1); // set string to all null chars
	int i = 0;
	int arr_pos = 0;
	int should_skip = 1;
	for(i = 0; i < bit_count; i++)
	{
		// A binary mask that checks each bit one by one and converts to the ASCII
		// representation of that bit
		char temp = (char)(((a >> (bit_count - 1 - i)) & 1) + 48);
		if(temp == '1')
			should_skip = 0;
		if(should_skip) continue;
		(*bin_str)[arr_pos] = temp;
		arr_pos++;
	}
	if(arr_pos == 0) (*bin_str)[arr_pos++] = '0'; // if a is zero set it to be so
}

/**
 *	A function that interprets and processes the requested operation
 *	Args:
 *		nums (double[]): An array of doubles representing the last two inputs
 *		op (char): A character representing the requested operation
 */
void calculate(double nums[], char op)
{
	switch(op)
	{
		case 'a' :
			nums[1] = nums[1] + nums[0];
			printf("=%.3lf\n", nums[1]);
			break;
		case 's':
			nums[1] = nums[1] - nums[0];
			printf("=%.3lf\n", nums[1]);
			break;
		case 'x':
			nums[1] = nums[1] * nums[0];
			printf("=%.3lf\n", nums[1]);
			break;
		case 'd':
			if(nums[0] == 0.)
			{
				printf("Divide by zero error.\n");
				break;
			}
			nums[1] = nums[1] / nums[0];
			printf("=%.3lf\n", nums[1]);
			break;
		case '^':
			nums[1] = pow(nums[1], nums[0]);
			printf("=%.3lf\n", nums[1]);
			break;
		case 'l':
			if(nums[0] < 0 || nums[1] < 0)
			{
				printf("Neither argument can be negative\n");
				break;
			}
			nums[1] = log(nums[0]) / log(nums[1]);
			printf("=%.3lf\n", nums[1]);
			break;
		case 'w':
		{
			double temp = nums[0];
			nums[0] = nums[1];
			nums[1] = temp;
			break;
		}
		case 'c':
			nums[0] = nums[1] = 0.;
			break;
		case 't':
		case 'b':
		{
			nums[0] = (float)(int)nums[0];
			nums[1] = (float)(int)nums[1];
			if(op == 't') break;
			char* bin_str1 = NULL;
			char* bin_str2 = NULL;
			binaryString(nums[0], &bin_str1);
			binaryString(nums[1], &bin_str2);
			printf("%s\n", bin_str1);
			printf("%s\n", bin_str2);
			free(bin_str1);
			free(bin_str2);
			break;
		}
		case 'p':
		{
			printf("%.3lf\n", nums[0]);
			printf("%.3lf\n", nums[1]);
			break;
		}
		case 'h':
			printf("Calculator Usage Help:\n");
			printf("Note: earlier refers to the first input value\n");
			printf("      later refers to the second input value\n");
			printf("      values refers to both of them\n");
			printf("x: multiplies the values --> later *= earlier\n");
			printf("d: divides the values --> later /= earlier\n");
			printf("   Note: divide by zero error is caught and printed\n");
			printf("a: adds the values --> later += earlier\n");
			printf("s: subtracts the values --> later -= earlier\n");
			printf("^: exponentiates the values --> later = pow(later, earlier)\n");
			printf("l: computes the logarithm of the values --> later = "
				"log(later, earlier)\n");
			printf("   Note: both values must be positive and error is caught\n");
			printf("w: swaps earlier and later\n");
			printf("c: clears the values setting them to 0.\n");
			printf("t: truncates the values (cast to int)\n");
			printf("b: performs t and prints the values\n");
			printf("p: prints the values in double precision\n");
			printf("h: prints this help message\n");
			printf("q: quits the program\n");
			break;
		case 'q':
			break;
		default:
			printf("The operator %c is not supported.\n", op);
			break;
	}
}
int main(void)
{
	double nums[2] = {0., 0.}; // earlier input, later input
	char* input = NULL;
	int max_digits = 3 + DBL_MANT_DIG - DBL_MIN_EXP; // max size of double input
	input = malloc(max_digits * sizeof(char)); // max size of char input
	memset(input, 0, max_digits); // set string to all null chars
	// load input into stdin and check for EOF at beginning of file
	if(fgets(input, max_digits, stdin) != NULL)
	{
		while(EOF != input[0]) // check next input for EOF
		{
			if(input[0] == '\0' || input[0] == '\n') // check empty input
			{
				if(fgets(input, max_digits, stdin) == NULL) break; // get next input
				else continue;
			}
			if(input[0] == 'q') break; // exit if quit is requested
			if(!isdigit(input[0]) && input[0] != '-') // test non-numerical input
				calculate(nums, input[0]);
			else
			{
				// update earlier and later store
				nums[0] = nums[1];
				nums[1] = atof(input); 
			}
			memset(input, 0, max_digits); // clear out input to null chars
		}
	}
	calculate(nums, 'p'); // final print before exit
	free(input);

	return 0;
}