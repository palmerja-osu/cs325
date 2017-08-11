#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


typedef struct {
    int start_index;
    int end_index;
    int sum;
} subarray;

void executeAlgorithm(int input_array[], int input_start_index, int input_end_index);
void write_to_file(int input_array[], int input_end_index, int max_start_index,
				   int max_end_index, int max_subarray_sum);


int main() {
	int i;
	int numberOfElements = 0;
	char *inputFileName = "MSS_Problems.txt";
	int numberOfLines = numberOfLinesInFile(inputFileName);

	for (i = 0; i < numberOfLines; i++) {
    	//Read data from file and store in array
	    numberOfElements = getNumberOfElementsInLine(inputFileName, i);
	    int *inputArray = malloc(numberOfElements * sizeof(int));

	    //Store file data to inputArray
	    fillIntArray(inputFileName, i, inputArray, numberOfElements);

	    //Find max sum subarray
		struct timeval start, stop;
		gettimeofday(&start, NULL);
	    executeAlgorithm(inputArray, 0, numberOfElements);
		gettimeofday(&stop, NULL);
		int time_in_micro = stop.tv_usec - start.tv_usec;
//        printf("Time used in microseconds = %lu\n", time_in_micro);

	    //Free heap
	    free(inputArray);
	}

	return 0;
}


/******** FUNCTION DEFINTIONS ********/
//Find max sum subarray
void executeAlgorithm(int input_array[], int input_start_index, int input_end_index) {
	int i;
	int tmp_sum;
	subarray max_subarray, parent_array;

	max_subarray.sum = 0;
	max_subarray.start_index = -1;
	max_subarray.end_index = -1;

	parent_array.sum = 0;
	parent_array.start_index = -1;

	for (i = input_start_index; i < input_end_index; i++) {
		tmp_sum = parent_array.sum	+ input_array[i];
		if (tmp_sum > 0) {
			if (parent_array.sum == 0) {
					parent_array.start_index = i;
			}
			parent_array.sum = tmp_sum;
		}
		else {
			parent_array.sum = 0;
		}

		if (parent_array.sum > max_subarray.sum) {
			max_subarray.sum = parent_array.sum;
			max_subarray.start_index = parent_array.start_index;
			max_subarray.end_index = i;
		}
	}

    // Output the result to results file
    write_to_file(input_array, input_end_index, max_subarray.start_index, max_subarray.end_index,
        max_subarray.sum);
}


//Write input_array, subarray, and max to output file
void write_to_file(int input_array[], int input_end_index, int max_start_index,
    int max_end_index, int max_subarray_sum) {
    FILE *outputFile = fopen("MSS_Results.txt", "a");

    //Write input_array to file
    fprintf(outputFile, "%c", '[');
    int i;
    for (i = 0; i < input_end_index; i++) {
        if (i != 0) {	//do not write comma if first element
            fprintf(outputFile, "%s", ", ");
        }
        fprintf(outputFile, "%d", input_array[i]);
    }
    fprintf(outputFile, "%c\n", ']');

    //Write subarray to file
    fprintf(outputFile, "%c", '[');
    for (i = max_start_index; i < max_end_index + 1; i++) {
        if (i != max_start_index) {	//do not write comma if first element
            fprintf(outputFile, "%s", ", ");
        }
        fprintf(outputFile, "%d", input_array[i]);
    }
    fprintf(outputFile, "%c\n%d\n", ']', max_subarray_sum);

    fprintf(outputFile, "\n");

    fclose(outputFile);
}

