// TODO Add comments and header
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <ctype.h>

void binaryString(int a, char** bin_str)
{
	int bit_count = sizeof(int) * CHAR_BIT;
	*bin_str = (char*)malloc((bit_count+1) * sizeof(char));
	int i = 0;
	int arr_pos = 0;
	int first_bit = 0;
	int should_skip = 1;
	for(i = 0; i < bit_count; i++)
	{
		char temp = (char)(((a >> (bit_count - 1 - i)) & 1) + 48);
		if(i == 0)
			first_bit = temp;
		else
		{	if(should_skip)
			{
				if(temp == first_bit) continue;
				else should_skip = 0;
			}
		}
		(*bin_str)[arr_pos] = temp;
		arr_pos++;
	}
	(*bin_str)[i] = '\0';
}

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
			printf("l: computes the logarithm of the values --> later = log(later, earlier)\n");
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
	double nums[2] = {0., 0.}; // first element is EARLIER, second is LATER
	char* input = NULL;
	int max_digits = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
	input = malloc(max_digits * sizeof(char)); // allocates memory 
	memset(input, 0, max_digits); // sets all characters to the null char
	if(fgets(input, max_digits, stdin) != NULL)
	{
		while(EOF!=input[0])
		{
			if(input[0] == '\0' || input[0] == '\n')
			{
				if(fgets(input, max_digits, stdin) == NULL) break;
				else continue;
			}
			if(!isdigit(input[0]) && input[0] != '-')
				calculate(nums, input[0]);
			else
			{
				nums[0] = nums[1];
				nums[1] = atof(input);
			}
			memset(input, 0, max_digits);
		}
	}
	calculate(nums, 'p');
	free(input);
	return 0;
}