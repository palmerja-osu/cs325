#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int executeAlgorithm(int *inputArray, int *coinArray, int numberOfElements, int changeAmount);
void createResultChangeArray(int* resultChangeArray, int* coinArray, int* inputArray, int numberOfElements, int changeAmount);


int executeAlgorithm(int *inputArray, int *coinArray, int numberOfElements, int changeAmount)
{
	// Write algorithm here
	int minNumberOfCoins = INT_MAX;
	int i = 0;
	int temp_i;
	int temp_min;
	int temp_changeAmount;
	for (i = 0; i < numberOfElements; i++) {
	    if (inputArray[i] == changeAmount) {
		    coinArray[changeAmount - 1] = i;
			minNumberOfCoins = 1;
			return minNumberOfCoins;
		}
		else if (inputArray[i] < changeAmount) {
			temp_changeAmount = changeAmount - inputArray[i];
		    temp_min = 1 + executeAlgorithm(inputArray, coinArray, numberOfElements, temp_changeAmount);
			if (temp_min < minNumberOfCoins) {
				minNumberOfCoins = temp_min;
				temp_i = i;
			}
		}
	}
	coinArray[changeAmount - 1] = temp_i;
	return minNumberOfCoins;
}

void createResultChangeArray(int* resultChangeArray, int* coinArray, int* inputArray, int numberOfElements, int changeAmount) {
	int i;
	while (changeAmount > 0) {
		for (i = 0; i < numberOfElements; i++) {
			if (i == coinArray[changeAmount-1]) {
				resultChangeArray[i] = resultChangeArray[i] + 1;
				changeAmount = changeAmount - inputArray[i];
				i = numberOfElements;
			}
		}
	}
}

// main program
int main(int argc, char *argv[])
{
	if ( argc != 2 )
	{
		// A file name was not entered as a program argument
		printf( "usage: %s filename\n", argv[0] );
		return 1;
	}
	else
	{
		// Check if file exists
		if (fileExists(argv[1]) == 0)
		{
			printf("Error: File does not exist\n");
			return 1;
		}
		
		int i, j, k;
		int numberOfElements = 0;
		int changeAmount = 0;
		int lineContainingArray = 0;
		int lineContainingChangeAmount = 0;
		char *inputFileName = argv[1];

		int numberOfLines = numberOfLinesInFile(inputFileName);
		int numberOfProblemsToProcess = numberOfLines / 2;
		int minNumberOfCoins;

		// Run the algorithm problem in the input file. A problem
		//	consists of the array of denominations and the amount
		//  of change.
		for (i = 0; i < numberOfProblemsToProcess; i++)
		{
			lineContainingArray = i + i;
			numberOfElements = getNumberOfElementsInLine(inputFileName, lineContainingArray);
			int *inputArray = malloc(numberOfElements * sizeof(int));
			int *resultChangeArray = malloc(numberOfElements * sizeof(int));
			for (j = 0; j < numberOfElements; j++) {
				resultChangeArray[j] = 0;
			}
			// Fill the input array with the numbers from line i in the file
			fillIntArray(inputFileName, lineContainingArray, inputArray, numberOfElements);

			lineContainingChangeAmount = (2 * i) + 1;
			changeAmount = getChangeAmount(inputFileName, lineContainingChangeAmount);
			int *coinArray = malloc(changeAmount * sizeof(int));
			for (k = 0; k < changeAmount; k++) {
				coinArray[k] = INT_MIN;
			}
			
			//run brute-force / recursive algorithm and then generate result array by walking through the coinArray created by executeAlgorithm
			minNumberOfCoins = executeAlgorithm(inputArray, coinArray, numberOfElements, changeAmount);
			createResultChangeArray(resultChangeArray, coinArray, inputArray, numberOfElements, changeAmount);
			
			// Output the result to results file
			outputResultToFile(resultChangeArray, numberOfElements, minNumberOfCoins, inputFileName);		
			
			// Cleanup dynamically allocated arrays
			free(inputArray);
			free(coinArray);
			free(resultChangeArray);
		}
	}

	return 0;
}
