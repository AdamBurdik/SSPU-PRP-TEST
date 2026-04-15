#include <stdio.h>

#define INPUT_FILENAME "time.txt"
#define OUTPUT_FILENAME "odpoledne.txt"

#define SEPERATOR_LENGTH 61

typedef struct {
	int hours;
	int minutes;
	int seconds;
} TIME;

typedef struct {
	int hours;
	int minutes;
	int seconds;
	int half; // 0 == AM, 1 == PM
} TwelveHourTime;

int time_to_12hour(TIME *input, TwelveHourTime *output) {
    if (input == NULL || output == NULL) {
        return 1;
    }

    if (input->hours >= 12) {
        output->half = 1; // PM
    } else {
        output->half = 0; // AM
    }

    int h = input->hours;
    if (h == 0) {
        output->hours = 12;
    } else if (h > 12) {
        output->hours = h - 12;
    } else {
        output->hours = h;
    }

    output->minutes = input->minutes;
    output->seconds = input->seconds;

    return 0;
}

int get_seconds_from_midnight(TIME *time) {
	return time -> hours * 3600 + time -> minutes * 60 + time -> seconds;
}

FILE* try_fopen(char *filename, char* mode) {
	FILE *file = fopen(filename, mode);
	if (file == NULL) {
		printf("Unable to read %s\n", filename);
	}
	return file;
}

void print_seperator(FILE *output, int n) {
	for (int i = 0; i < n; i++) {
		fprintf(output, "-");
	}
	fprintf(output, "\n");
}

void print_header(FILE *output) {
	print_seperator(output, SEPERATOR_LENGTH);
	fprintf(output, "| Index | Europe Time | Stupid Time | Seconds from midnight |\n");
	print_seperator(output, SEPERATOR_LENGTH);
}

bool read_time(FILE *input, TIME *time) {
	return fscanf(input, "%d %d %d", &time -> hours, &time -> minutes, &time -> seconds) == 3;
}

void write_entry(int i, FILE *output, TIME *time) {
	TwelveHourTime twelve_hour_time = {};
	time_to_12hour(time, &twelve_hour_time);

	int seconds = get_seconds_from_midnight(time);

	fprintf(
		output,
		"| %4d. |   %02d:%02d:%02d  | %02d:%02d:%02d %s | %21d |\n",
		i + 1,
		time -> hours,
		time -> minutes,
		time -> seconds,
		twelve_hour_time.hours,
		twelve_hour_time.minutes,
		twelve_hour_time.seconds,
		twelve_hour_time.half == 0 ? "AM" : "PM",
		seconds
	);
}

int main(void) {

	FILE *input = try_fopen(INPUT_FILENAME, "r");
	FILE *output = try_fopen(OUTPUT_FILENAME, "w");
	if (!input || !output) {
	 	return 1;
	}

	TIME time = {};

	print_header(stdout);
	print_header(output);

	int i = 0;
	int written_to_file = 0;
	while (read_time(input, &time)) {
		write_entry(i, stdout, &time);

		if (time.hours >= 12) {
			write_entry(written_to_file, output, &time);
			written_to_file++;
		}
		
		i++;
	}

	print_seperator(stdout, SEPERATOR_LENGTH);
	print_seperator(output, SEPERATOR_LENGTH);

	printf("%d time entries has been read from %s\n", i, INPUT_FILENAME);
	printf("%d time entries were written to %s\n", written_to_file, OUTPUT_FILENAME);
	fprintf(output, "%d time entries were written to %s\n", written_to_file, OUTPUT_FILENAME);

	fclose(input);
	fclose(output);
	
	
	return 0;
}
