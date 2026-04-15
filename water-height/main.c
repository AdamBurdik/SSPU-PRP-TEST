#include <stdio.h>

#define INPUT_FILENAME "cisla.txt"
#define OUTPUT_FILENAME "hladiny.txt"

#define SCREEN_SEPARATOR_LENGTH 67
#define FILE_SEPARATOR_LENGTH 80

#define PI 3.14159265358979323846

typedef struct {
	int radius_cm;
	int height_cm;
	int water_l;
} CYLINDER;

double get_cylinder_volume_dm3(CYLINDER *cylinder) {
	if (cylinder == NULL) {
		return 0.0;
	}

	return PI * cylinder->radius_cm * cylinder->radius_cm * cylinder->height_cm / 1000.0;
}

double get_water_level_cm(CYLINDER *cylinder) {
	if (cylinder == NULL || cylinder->radius_cm <= 0) {
		return 0.0;
	}

	return (cylinder->water_l * 1000.0) / (PI * cylinder->radius_cm * cylinder->radius_cm);
}

FILE *try_fopen(char *filename, char *mode) {
	FILE *file = fopen(filename, mode);
	if (file == NULL) {
		printf("Unable to open %s\n", filename);
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
	fprintf(output, "| Index | Radius    | Height    | Cylinder volume  | Water amount |\n");
	fprintf(output, "|       | (cm)      | (cm)      | (dm3)            | (l)          |\n");
	print_separator(output, SCREEN_SEPARATOR_LENGTH);
}

void print_file_header(FILE *output) {
	print_separator(output, FILE_SEPARATOR_LENGTH);
	fprintf(output, "| Index | Radius   | Height   | Cylinder volume  | Water amount | Water level  |\n");
	fprintf(output, "|       | (cm)     | (cm)     | (dm3)            | (l)          | (cm)         |\n");
	print_separator(output, FILE_SEPARATOR_LENGTH);
}

int read_cylinder(FILE *input, CYLINDER *cylinder) {
	if (input == NULL || cylinder == NULL) {
		return 0;
	}

	return fscanf(input, "%d %d %d", &cylinder->radius_cm, &cylinder->height_cm, &cylinder->water_l) == 3;
}

void write_screen_entry(int index, FILE *output, CYLINDER *cylinder) {
	double volume_dm3 = get_cylinder_volume_dm3(cylinder);

	fprintf(
		output,
		"| %4d. | %6d cm | %6d cm | %12.2f dm3 | %10d l |\n",
		index + 1,
		cylinder->radius_cm,
		cylinder->height_cm,
		volume_dm3,
		cylinder->water_l
	);
}

void write_file_entry(int index, FILE *output, CYLINDER *cylinder) {
	double volume_dm3 = get_cylinder_volume_dm3(cylinder);
	double water_level_cm = get_water_level_cm(cylinder);

	fprintf(
		output,
		"| %4d. | %5d cm | %5d cm | %12.2f dm3 | %10d l | %9.2f cm |\n",
		index + 1,
		cylinder->radius_cm,
		cylinder->height_cm,
		volume_dm3,
		cylinder->water_l,
		water_level_cm
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

	CYLINDER cylinder = {};
	int read_entries = 0;
	int written_entries = 0;

	print_screen_header(stdout);
	print_file_header(output);

	while (read_cylinder(input, &cylinder)) {
		double volume_dm3 = get_cylinder_volume_dm3(&cylinder);

		write_screen_entry(read_entries, stdout, &cylinder);

		if (cylinder.water_l <= volume_dm3) {
			write_file_entry(written_entries, output, &cylinder);
			written_entries++;
		}

		read_entries++;
	}

	print_separator(stdout, SCREEN_SEPARATOR_LENGTH);
	print_separator(output, FILE_SEPARATOR_LENGTH);

	printf("%d cylinder entries were read from %s\n", read_entries, INPUT_FILENAME);
	printf("%d valid entries were written to %s\n", written_entries, OUTPUT_FILENAME);

	fclose(input);
	fclose(output);

	return 0;
}
