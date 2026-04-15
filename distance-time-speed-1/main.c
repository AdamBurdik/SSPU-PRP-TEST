#include <stdio.h>

#define INPUT_FILENAME "cisla.txt"
#define OUTPUT_FILENAME "prevody.txt"

#define SCREEN_SEPARATOR_LENGTH 49
#define FILE_SEPARATOR_LENGTH 57

typedef struct {
	int distance_m;
	int time_s;
} ENTRY;

double get_speed_ms(int distance_m, int time_s) {
	if (time_s <= 0) {
		return 0.0;
	}

	return (double)distance_m / (double)time_s;
}

double get_speed_kmh(int distance_m, int time_s) {
	return get_speed_ms(distance_m, time_s) * 3.6;
}

FILE *try_fopen(char *filename, char *mode) {
	FILE *file = fopen(filename, mode);
	if (file == NULL) {
		printf("Unable to open file %s\n", filename);
	}
	return file;
}

void print_separator(FILE *output, int length) {
	for (int i = 0; i < length; i++) {
		fprintf(output, "-");
	}
	fprintf(output, "\n");
}

void print_screen_header(FILE *output) {
	print_separator(output, SCREEN_SEPARATOR_LENGTH);
	fprintf(output, "| Index | Distance (m) | Time (s) | Speed (m/s) |\n");
	print_separator(output, SCREEN_SEPARATOR_LENGTH);
}

void print_file_header(FILE *output) {
	print_separator(output, FILE_SEPARATOR_LENGTH);
	fprintf(output, "| Index | Distance (km,m) | Time (min,s) | Speed (km/h) |\n");
	print_separator(output, FILE_SEPARATOR_LENGTH);
}

int read_entry(FILE *input, ENTRY *entry) {
	if (input == NULL || entry == NULL) {
		return 0;
	}

	return fscanf(input, "%d %d", &entry->distance_m, &entry->time_s) == 2;
}

void write_screen_entry(int index, FILE *output, ENTRY *entry) {
	double speed_ms = get_speed_ms(entry->distance_m, entry->time_s);

	fprintf(
		output,
		"| %4d. | %12d | %8d | %11.2f |\n",
		index + 1,
		entry->distance_m,
		entry->time_s,
		speed_ms
	);
}

void write_file_entry(int index, FILE *output, ENTRY *entry) {
	int km = entry->distance_m / 1000;
	int m = entry->distance_m % 1000;
	int min = entry->time_s / 60;
	int sec = entry->time_s % 60;
	double speed_kmh = get_speed_kmh(entry->distance_m, entry->time_s);

	fprintf(
		output,
		"| %4d. | %3d km %3d m   | %3d min %2d s  | %12.2f |\n",
		index + 1,
		km,
		m,
		min,
		sec,
		speed_kmh
	);
}

int main(void) {
	FILE *input = try_fopen(INPUT_FILENAME, "r");
	FILE *output = try_fopen(OUTPUT_FILENAME, "w");

	if (!input || !output) {
		if (input) {
			fclose(input);
		}
		if (output) {
			fclose(output);
		}
		return 1;
	}

	ENTRY entry = {0};
	int read_entries = 0;
	int written_entries = 0;
	long long distance_sum_m = 0;

	print_screen_header(stdout);
	print_file_header(output);

	while (read_entry(input, &entry)) {
		double speed_ms = get_speed_ms(entry.distance_m, entry.time_s);

		write_screen_entry(read_entries, stdout, &entry);

		distance_sum_m += entry.distance_m;

		if (speed_ms > 10.0) {
			write_file_entry(written_entries, output, &entry);
			written_entries++;
		}

		read_entries++;
	}

	print_separator(stdout, SCREEN_SEPARATOR_LENGTH);
	print_separator(output, FILE_SEPARATOR_LENGTH);

	if (read_entries > 0) {
		double average_distance_m = (double)distance_sum_m / (double)read_entries;
		printf("\nAverage distance is %.2f meters.\n", average_distance_m);
	} else {
		printf("\nAverage distance is 0.00 meters.\n");
	}

	printf("\n%d pairs were read from %s.\n", read_entries, INPUT_FILENAME);
	printf("\nFile %s was created.\n", OUTPUT_FILENAME);

	fprintf(output, "\n%d pairs were written to %s.\n", written_entries, OUTPUT_FILENAME);

	fclose(input);
	fclose(output);

	return 0;
}
