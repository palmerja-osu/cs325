#ifndef _FILEFUNCTIONS_H_
#define _FILEFUNCTIONS_H_

#include <stdio.h>

// Constants
#define MAX_INT_ELEMENTS 50 
#define MAX_INPUT_LINE_SIZE 1000

void displayFile();
int numberOfLinesInFile(char *fileName);
void initializeIntArray(int *intArray, int numberofElements);
void displayIntArray(int *intArray, int numberOfElements);
void fillIntArray(char *inputFileName, int inputLineNumber, int *inputArray, int numberOfElements);
int getNumberOfElementsInLine(char *inputFileName, int inputLineNumber);
void RemoveNewLineAndAddNullTerm(char *stringValue);
int countElementsInString(char *stringValue);
void getLineFromFile(char *inputFileName, int inputLineNumber, char *stringValue);
void outputResultToFile(int *resultArray, int arraysize, int *originalArray, int originalArraySize);
void alg1_enum(int *inputArray, int numberOfElements);
void alg2_betterEnum(int *inputArray, int numberOfElements);
void alg3_divideAndConquer(int *inputArray, int numberOfElements);
int algo3_maxSubArray(int *inputArray, int lowIdx, int highIdx, int *currentLowIdx, int *currentHighIdx, int *highestSum);
void algo3_maxCrossingSubArray(int *inputArray, int lowIdx, int midIdx, int highIdx, int *crossLow, int *crossHigh, int *crossSum);

#endif