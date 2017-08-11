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
	//Calculate the maximum sum sub-array and keep track of the indices between which that sub-array is contained
	int i, j, k, sum; //initialize index integers and sum integer for for loops
	int max = INT_MIN; //smallest possible int value
	int max_i = -1; //initialize i index for max sum sub-array
	int max_j = -1; //initialize j index for max sum sub-array
	for (i = 0; i < numberOfElements; i++) {
		for (j = i; j < numberOfElements; j++) {
			sum = 0; //reset sum to 0 before calculating next summation between new i and j values
			for (k = i; k <= j; k++) { //Calculate the summation of all elements in array between i and j
				sum = sum + inputArray[k];
			}
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
