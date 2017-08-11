/******************************************************************************
 * Program Filename: main.cpp
 * Author: Group 35
 * 				Jason DiMedio
 *				James Palmer
 *				Babatunde Ogunsaju
 * Date: May 2, 2017
 * Description: This program tests the running time for various implementations
 * of the coin change algorithm using a user specified input file. It also
 * tests the algorithms for correctness by processing arrays from an input file into
 * results in an output file for verification.
 *****************************************************************************/

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <sstream>

void initialize_results(int A[], int n);

int changeslow(int A[], int n, int changeAmount, int results[])
{
	// results is an array containing the number of coins of each index
	// in the optimum solution
	// Ex: original  array is 1, 2, 5
	// changeAmount = 10
	// results = 0, 0, 2
	std::cout << "testing changeslow(..." << changeAmount << "..)" << std::endl;

	int min = 999999999;
	int * min_r1 = NULL;
	int * min_r2 = NULL;

	int * r1;
	int * r2;

	// To make change for A cents, start by letting K = A
	int K = changeAmount;

	// •	If there is a K-cent coin, then that one coin is the minimum
	for (int i = 0; i < n; i++)
	{
		if (A[i] == K)
		{
			results[i]++;
			return 1;
		}
	}

	// •	Otherwise, for each value i < K,
	for (int i = 1; i <= K/2; i++)
	{
		r1 = new int[n];
		r2 = new int[n];

		initialize_results(r1, n);
		initialize_results(r2, n);

		// 1)	Find the minimum number of coins needed to make i cents
		int min1 = changeslow(A, n, i, r1);

		// 2)	Find the minimum number of coins needed to make K - i cents
		int min2 = changeslow(A, n, K-i, r2);

		// •	Choose the i that minimizes the sum of 1) and 2)
		if (min1 + min2 < min)
		{
			min = min1 + min2;

			if (min_r1 != NULL)
				delete[] min_r1;
			if (min_r2 != NULL)
				delete[] min_r2;

			min_r1 = r1;
			min_r2 = r2;

		}
		else
		{
			if (r1 != NULL)
				delete[] r1;
			if (r2 != NULL)
				delete[] r2;
		}

	}

	// sum results arrays
	for (int i = 0; i < n; i++)
		results[i] = min_r1[i] + min_r2[i];


		if (min_r1 != NULL)
			delete[] min_r1;
		if (min_r2 != NULL)
			delete[] min_r2;

	return min;

}

int changegreedy(int A[], int n, int changeAmount, int results[])
{
	int min;
/*
	for (int i = 0; i < n; i++)
		results[i] = 0;
*/
	for (int i = n-1; i >= 0;  i--)
	{
		while (A[i] <= changeAmount)
		{
			changeAmount = changeAmount - A[i];
			results[i]++;
			min++;
		}
	}

	return min;
}

//REF: http://stackoverflow.com/questions/23557648/coin-change-dynamic-programming
int changedp(int T[], int n, int val, int *list){
	//T[] -> array of coin denominations
	// n  -> number of elements in T[]
	//val -> amount to determine min number of coins from
	//list   -> returned list of min denoms needed.

	int min;

	int** count = new int*[val + 1];
	int *C = new int[val+1];

	for(int x = 0; x < (val + 1); x++)
            count[x] = new int[n];    //REF: http://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new

	C[0]=0;
	for(int p = 1; p<=val; p++){
		min = INT_MAX;
		for(int i = 0; i< n; i++){
			if(T[i] <= p)
				if((C[p-T[i]]+1) < min){
					min = (C[p-T[i]]+1);
					for(int k = 0; k < n; ++k) //keep track of min denominations for each C[p]
                    {
                        count[p][k] = count[p-T[i]][k];
                    }
                    count[p][i]++;
				}
		}
		C[p] = min;
	}

	//ger list of minimum demnominations
	for(int t = 0; t < n; ++t)
		list[t] = count[val][t];

	return C[val];
}

int sum_array(int A[], int n)
{
	int sum = 0;

	for (int i = 0; i < n;  i++)
		sum += A[i];

	return sum;
}

void initialize_results(int A[], int n)
{
	for (int i = 0; i < n;  i++)
		A[i] = 0;
}

/*******************************************************************************
 ** Function: menu()
 ** Description: This function displays a general menu for the program. It then
 ** makes sure the selection is within the range of options and returns it.
 ** Parameters: None
 ** Pre-Conditions: None
 ** Post-Conditions: An int from 1 through 3 is returned.
 ******************************************************************************/
int menu()
{
   int selection = 0;
   bool valid_input = false;

   std::cout << std::endl;
   std::cout << "1. Test algorithms using input file" << std::endl;
   std::cout << "2. Run experimental analysis using predefined inputs" << std::endl;
   std::cout << "3. Quit" << std::endl;
   std::cout << std::endl;

   std::cout << "Please enter the number of your selection.";
   std::cout << std::endl;
   std::cin >> selection;

   while (!valid_input)
   {
      if ((selection < 1) || (selection > 3))
      {
         std::cin.ignore();
         std::cout << "Invalid selection." << std::endl;
         std::cout << "Please enter the number of your selection.";
         std::cout << std::endl;
         std::cin >> selection;
      }
      else
         valid_input = true;
   }

   return selection;
}

/*******************************************************************************
 ** Function: algoMenu()
 ** Description: This function displays a general menu for the program. It then
 ** makes sure the selection is within the range of options and returns it.
 ** Parameters: None
 ** Pre-Conditions: None
 ** Post-Conditions: An int from 1 through 4 is returned.
 ******************************************************************************/
int algoMenu()
{
   int selection = 0;
   bool valid_input = false;

   std::cout << std::endl;
   std::cout << "1. Algorithm 1: Brute Force/Divide and Conquer" << std::endl;
   std::cout << "2. Algorithm 2: Greedy" << std::endl;
   std::cout << "3. Algorithm 3: Dynamic Programming" << std::endl;
   std::cout << "4. Go Back" << std::endl;
   std::cout << std::endl;

   std::cout << "Please enter the number of your selection.";
   std::cout << std::endl;
   std::cin >> selection;

   while (!valid_input)
   {
      if ((selection < 1) || (selection > 4))
      {
         std::cin.ignore();
         std::cout << "Invalid selection." << std::endl;
         std::cout << "Please enter the number of your selection.";
         std::cout << std::endl;
         std::cin >> selection;
      }
      else
         valid_input = true;
   }

   return selection;
}

/*******************************************************************************
 ** Function: getMinChange()
 ** Description: This function asks the user to input a minimum number to seek change
 ** for, for testing purposes.
 ** Parameters: None
 ** Pre-Conditions: None
 ** Post-Conditions: An int from 1 to 10000000000 is returned.
 ******************************************************************************/
int getMinChange()
{
   int selection = 0;
   bool valid_input = false;

   std::cout << std::endl;
   std::cout << "Please enter the minimum amount of change A to seek in the series of tests: ";
   std::cout << std::endl;
   std::cin >> selection;

   while (!valid_input)
   {
      if ((selection < 1) || (selection > 10000000000))
      {
         std::cin.ignore();
         std::cout << "Invalid selection." << std::endl;
         std::cout << "Please enter a number from 1 to 10000000000.";
         std::cout << std::endl;
         std::cin >> selection;
      }
      else
         valid_input = true;
   }

   return selection;
}

/*******************************************************************************
 ** Function: getMaxChange()
 ** Description: This function asks the user to input a maximum number to seek change
 ** for, for testing purposes.
 ** Parameters: None
 ** Pre-Conditions: None
 ** Post-Conditions: An int from 1 to 1000000000000 is returned.
 ******************************************************************************/
int getMaxChange()
{
   long selection = 0;
   bool valid_input = false;

   std::cout << std::endl;
   std::cout << "Please enter the maximum amount of change A to seek in the series of tests: ";
   std::cout << std::endl;
   std::cin >> selection;

   while (!valid_input)
   {
      if ((selection < 1) || (selection > 1000000000000))
      {
         std::cin.ignore();
         std::cout << "Invalid selection." << std::endl;
         std::cout << "Please enter a number from 1 to 1000000000000.";
         std::cout << std::endl;
         std::cin >> selection;
      }
      else
         valid_input = true;
   }

   return selection;
}

/*******************************************************************************
 ** Function: getIncrementNum()
 ** Description: This function asks the user to input a number by which to increment
 ** the amount of change to seek a series of tests.
 ** Parameters: None
 ** Pre-Conditions: None
 ** Post-Conditions: An int from 1 to 1000000000 is returned.
 ******************************************************************************/
int getIncrementNum()
{
   int selection = 0;
   bool valid_input = false;

   std::cout << std::endl;
   std::cout << "Amount to increment A between simulations: ";
   std::cout << std::endl;
   std::cin >> selection;

   while (!valid_input)
   {
      if ((selection < 1) || (selection > 1000000000))
      {
         std::cin.ignore();
         std::cout << "Invalid selection." << std::endl;
         std::cout << "Please enter a number from 1 to 1000000000.";
         std::cout << std::endl;
         std::cin >> selection;
      }
      else
         valid_input = true;
   }

   return selection;
}

void output_results(int results[], int A[], int n, std::string outputFileName)
{
	int min = sum_array(results, n);

	std::ofstream output_file;
	output_file.open(outputFileName.c_str(), std::fstream::app);

	if (output_file.is_open()) {
		// output original array
		for (int k = 0; k < n; k++) {
			output_file << A[k];
			if (k != n-1)
				output_file << " ";
		}

		output_file << std::endl;

		// output results array
		for (int k = 0; k < n; k++) {
			output_file << results[k];
			if (k != n-1)
				output_file << " ";
		}

		output_file << std::endl;
		output_file << min << std::endl;

		output_file << std::endl << std::endl;
		output_file.close();
	}
}

void simulateInputFile(int (*change)(int[], int, int, int[]), std::string name, char *inputFileName)
{
	std::string outputFileName = inputFileName;
	std::ifstream inputFile;
	std::string readString = "";
	std::string changeAmountString = "";
	std::string number = "";
	int changeAmount;
	int n = 0;
	bool outputCreated = false;
	int current_line = 1;

	inputFile.open(inputFileName);

	int* A;
	int* r;

	while (getline(inputFile, readString))
	{
		if (current_line % 2 == 1) // odd numbered lines
		{
			A = new int[10000000];

			// Write name to results output file
			std::ofstream output_file;
			if (!outputCreated) {
				outputFileName.insert(outputFileName.find('.'), "change");
				outputCreated = true;
			}
			output_file.open(outputFileName.c_str(), std::fstream::app);
			if (output_file.is_open()) {
				output_file << name << std::endl;
				output_file.close();
			}

			// Load up array from  line input
			for (int i = 0; i < readString.length(); i++)
			{
				if (isdigit(readString.at(i)))
				{
					number += readString[i];

					if (i == readString.length() - 1)
					{
						A[n] = atoi(number.c_str());
						n++;
						number = "";
					}
				}
				else
				{
					A[n] = atoi(number.c_str());
					n++;
					number = "";
				}
			}
		}

		// Even numbered lines
		else
		{
			// REF: http://www.cplusplus.com/forum/articles/9645/
			std::stringstream convert(readString);
			if ( !(convert >> changeAmount) )
			    changeAmount = 0;

			r = new int[n];
			initialize_results(r, n);

			//Pass the input array into the change function
			(*change)(A, n, changeAmount, r);

			output_results(r, A, n, outputFileName);

			//Reset
			n = 0;
			delete[] r;
			delete[] A;
		}

		current_line++;

	}

	std::cout << "Results written to " << outputFileName << "..." << std::endl;

}

void simulatePredefined(int (*change)(int[], int, int, int[]), std::string name)
{
	/* The following code was adapted from "HW1 Problem 5- Help Video" from
	 * CS325 Oregon State University
	 */
	int* V;
	int* r;
	int n;
	int changeAmount;
	std::string simName, timeFileName, output_name;

	int minChange, maxChange, incChange;
	std::string minChangeStr, maxChangeStr;

	minChange = getMinChange();
	// REF: http://www.cplusplus.com/articles/D9j2Nwbp/
	std::ostringstream convertMin;
	convertMin << minChange;
	minChangeStr = convertMin.str();

	maxChange = getMaxChange();
	std::ostringstream convertMax;
	convertMax << maxChange;
	maxChangeStr = convertMax.str();
	incChange = getIncrementNum();

	// For each of V1 through V3
	for (int h = 0; h < 3; h++)
	{
		if (h == 0)
		{
			simName = "V1";
			n = 7;
			int V1[] = {1, 2, 6, 12, 24, 48, 60};
			V = V1;
		}
		else if (h == 1)
		{
			simName = "V2";
			n = 5;
			int V2[] = {1, 5, 10, 25, 50};
			V = V2;
		}
		if (h == 2)
		{
			simName = "V3";
			n = 5;
			int V3[] = {1, 6, 13, 37, 150};
			V = V3;
		}

		// open output file for time data for Vn
		FILE * pFile;
		timeFileName = simName + "_" + minChangeStr + "-" + maxChangeStr + "simulationTimes.txt";
		pFile = fopen(timeFileName.c_str(),"a+");
		float total, average, diff, seconds;

		// Open output file for Vn and write name of alogrithm to it
		std::string output_name = simName + "_" + minChangeStr + "-" + maxChangeStr + "results.txt";
		std::ofstream output_file;
		output_file.open(output_name.c_str(), std::fstream::app);
		if (output_file.is_open()) {
			output_file << name << std::endl;
			output_file.close();
		}

		// Get change for minChange through maxChange
		for (int i = minChange; i <= maxChange; i+=incChange)
		{
			int min;

			fprintf(pFile, "%d  ", i);
			printf("%d  ", i);
			total = 0;

			r = new int[n];
			initialize_results(r, n);

			clock_t t1, t2;
			t1 = clock();

			min = (*change)(V, n, i, r);

			t2 = clock();
			diff = ((float) t2 - (float) t1);
			seconds = diff / CLOCKS_PER_SEC;
			total = total + seconds;

			fprintf(pFile, " %f", seconds);
			printf(" %f", seconds);
			fprintf(pFile, " \n");
			printf(" \n");

			output_file.open(output_name.c_str(), std::fstream::app);
			if (output_file.is_open()) {
				output_file << i << " " << min << std::endl;;
				output_file.close();
			}

			delete[] r;
		}

		fclose(pFile);

		std::cout << "Results written to " << output_name << "..." << std::endl;
		std::cout << "Running time results written to " << timeFileName << std::endl;
	}
}

int main(int argc, char *argv[])
{
	bool filename = true;

	if ( argc != 2 )
	{
		// A file name was not entered as a program argument
		printf( "Warning! No filename specified! Do not select menu option 1.\n", argv[0] );

		filename = false;
	}
	char *inputFileName = argv[1];

	// Menu input
	bool quit = false;
	int choice = 0;

	while (!quit)
	{
		choice = menu();

		// Test algorithms using input file
		if (choice == 1)
		{
			if (!filename)
			{
				std::cout << "No filename specified." << std::endl;
				std::cout << std::endl;
			}
			else
			{
				bool go_back = false;

				while (!go_back)
				{
					int sub_choice = algoMenu();

					// Algorithm 1: Brute Force/Divide and Conquer
					if (sub_choice == 1)
						simulateInputFile(changeslow, "Algorithm 1: Brute Force/Divide and Conquer", inputFileName);

					// Algorithm 2: Greedy
					else if (sub_choice == 2)
						simulateInputFile(changegreedy, "Algorithm 2: Greedy", inputFileName);

					// Algorithm 3: Dynamic Programming
					else if (sub_choice == 3)
						simulateInputFile(changedp, "Algorithm 3: Dynamic Programming", inputFileName);

					// Go back
					else
						go_back = true;
				}
			}
		}

		// Run experimental analysis using predefined inputs
		else if (choice == 2)
		{
			bool go_back = false;

			while (!go_back)
			{
				int sub_choice = algoMenu();

				// Algorithm 1: Brute Force/Divide and Conquer
				if (sub_choice == 1)
					simulatePredefined(changeslow, "Algorithm 1: Brute Force/Divide and Conquer");

				// Algorithm 2: Greedy
				else if (sub_choice == 2)
					simulatePredefined(changegreedy, "Algorithm 2: Greedy");

				// Algorithm 3: Dynamic Programming
				else if (sub_choice == 3)
					simulatePredefined(changedp, "Algorithm 3: Dynamic Programming");

				// Go back
				else
					go_back = true;
			}
		}

		// Quit
		else
			quit = true;

	}


	return 0;
}