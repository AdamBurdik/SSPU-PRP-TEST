#include <stdio.h>
#define INPUT_FILENAME "numbers.txt"
#define OUTPUT_FILENAME "output.txt"
#define BUFFER_SIZE 1024

typedef struct {
	int first;
	int second;
} PAIR;

int find_gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int find_lcm(int a, int b, int gcd) {
    if (gcd == 0) return 0;
    return (a / gcd) * b;
}

void write_seperator(FILE *file, int count) {
	for (int i = 0; i < count; i++) {
		fprintf(file, "-");
	}
	fprintf(file, "\n");
}

void write_pair(
	FILE *file,
	int i,
	int first,
	int second,
	int gcd,
	int lcm
) {
	fprintf(
		file,
		"| %5d | %5d | %6d | %5d | %5d |\n",
		i + 1,
		first,
		second,
		lcm,
		gcd
	);
}

void print_pairs(
	PAIR *array,
	int pair_count
) {
	write_seperator(stdout, 42);
	printf("| Index | First | Second |  LCM  |  GCD  |\n");
	write_seperator(stdout, 42);
	int first_column_sum = 0;
	
	for (int i = 0; i < pair_count; i++) {
		PAIR pair = array[i];
		int first = pair.first;
		int second = pair.second;
		int gcd = find_gcd(first, second);
		int lcm = find_lcm(first, second, gcd);
		first_column_sum += first;

		write_pair(stdout, i, first, second, gcd, lcm);
	}
	write_seperator(stdout, 42);

	double average = (double) first_column_sum / pair_count;
	printf("Average of first column is %.2lf\n", average);
}

void write_pairs(
	char *filename,
	PAIR *array,
	int pair_count
) {
	FILE *output = fopen(filename, "w");
	if (output == NULL) {
		printf("Unable to open %s", filename);
		return;
	}

	write_seperator(output, 42);
	fprintf(output, "| Index | First | Second |  LCM  |  GCD  |\n");
	write_seperator(output, 42);
	int first_column_sum = 0;

	int index = 0;
	for (int i = 0; i < pair_count; i++) {
		PAIR pair = array[i];
		int first = pair.first;
		int second = pair.second;
		int gcd = find_gcd(first, second);
		if (gcd == 1) {
			continue;
		}
		
		int lcm = find_lcm(first, second, gcd);
		first_column_sum += first;

		write_pair(output, index, first, second, gcd, lcm);
		index++;
	}
	write_seperator(output, 42);

	double average = (double) first_column_sum / index;
	fprintf(output, "Average of first column is %.2lf\n", average);
	fclose(output);
}

int read_input_file(
	char *filename,
	PAIR* array,
	int array_size
) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("Unable to read %s\n", filename);
		return -1;
	}

	PAIR current = {};

	int i = 0;
	while (fscanf(file, "%d %d", &current.first, &current.second) == 2) {
		if (i >= array_size) {
			printf("Input file contains more elements that provider buffer of size %d can hold\n", array_size);
			fclose(file);
			return -2;
		}
		array[i] = current;
		i++;
	}

	fclose(file);
	return i;
}

int main(void) {
	PAIR array[BUFFER_SIZE];

	int count = read_input_file(INPUT_FILENAME, array, BUFFER_SIZE);
	if (count < 0) {
		return 1;
	}

	printf("Read %d pairs\n", count);
	print_pairs(array, count);
	write_pairs(OUTPUT_FILENAME, array, count);
	
	return 0;
}
