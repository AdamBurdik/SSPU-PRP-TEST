#include <stdio.h>

#define BUFFER_SIZE 1024
#define FILEINPUT "time.txt"

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
} STUPID_TIME;

int convert_for_stupid(TIME *input, STUPID_TIME *output) {
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

int read_input_file(
	char *filename,
	TIME* array,
	int array_size
) {
	FILE *input = fopen(filename, "r");
	if (!input) {
		printf("%s count be read!\n", filename);
		return -1;
	}

	TIME time = {};

	int i = 0;
	while (fscanf(input, "%d %d %d", &time.hours, &time.minutes, &time.seconds) == 3) {
		if (i >= array_size) {
			printf("Input file contains more elements that provider buffer of size %d can hold\n", array_size);
			fclose(input);
			return -1;
		}
		array[i] = time;
		i++;
	}
	fclose(input);
	return i;
}

void print_entries(TIME *array, int array_size) {
	printf("----------------------------------------------------------------\n");
	printf("Time Index | Europe Time | Stupid Time | Seconds from midnight |\n");
	printf("----------------------------------------------------------------\n");
	STUPID_TIME stupid = {};
	for (int i = 0; i < array_size; i++) {
		TIME *time = &array[i];
		convert_for_stupid(time, &stupid);
		int seconds = get_seconds_from_midnight(time);
		printf(
			"%9d. |   %02d:%02d:%02d  | %02d:%02d:%02d %s | %21d |\n",
			i + 1,
			time -> hours,
			time -> minutes,
			time -> seconds,
			stupid.hours,
			stupid.minutes,
			stupid.seconds,
			stupid.half == 0 ? "AM" : "PM",
			seconds
		);
	}
	printf("----------------------------------------------------------------\n");
}

int main(void) {
	
	TIME array[BUFFER_SIZE];

	int entries_read = read_input_file(FILEINPUT, array, BUFFER_SIZE);
	if (entries_read <= 1) return 1;
	printf("Read %d time entries from %s\n", entries_read, FILEINPUT);

	print_entries(array, entries_read);
	
	return 0;
}
