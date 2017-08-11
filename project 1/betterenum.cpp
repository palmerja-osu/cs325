#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


void executeAlgorithm(int *inputArray, int numberOfElements);

// Program entry point
int main()
{
	int i;
	int numberOfElements = 0;
	char *inputFileName = "MSS_Problems.txt";
	int numberOfLines = numberOfLinesInFile(inputFileName);

	// Run the algorithm for each line in the input file
	for (i = 0; i < numberOfLines; i++)
	{
		numberOfElements = getNumberOfElementsInLine(inputFileName, i);
		int *inputArray = malloc(numberOfElements * sizeof(int));

		// Fill the input array with the numbers from line i in the file
		fillIntArray(inputFileName, i, inputArray, numberOfElements);
		executeAlgorithm(inputArray, numberOfElements);

		// Cleanup dynamically allocated strings
		free(inputArray);
	}
}

void executeAlgorithm(int *inputArray, int numberOfElements)
{
	int i, j, sum; //initialize index integers and sum integer for for loops
	int max = INT_MIN; //smallest possible int value
	int max_i = -1; //initialize i index for max sum sub-array
	int max_j = -1; //initialize j index for max sum sub-array
	for (i = 0; i < numberOfElements; i++) {
		sum = 0; //reset sum to zero when we iterate to the next starting point (i index)
		for (j = i; j < numberOfElements; j++) {
			sum = sum + inputArray[j];
			if (sum > max) { //current summation is greater than previous max
				max = sum;
				max_i = i;
				max_j = j;
			}
		}
	}
	
	// Output the results to file
	int *results = &inputArray[max_i];
	int resultsLength = (max_j - max_i) + 1;
	outputResultToFile(results, resultsLength, inputArray, numberOfElements);
}