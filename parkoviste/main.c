#include <stdio.h>
#define INPUT_FILENAME "casy.txt"
#define OUTPUT_FILENAME "platby.txt"

#define FREE_HOUR_AMOUNT 2
#define PRICE_PER_HOUR 20

#define BUFFER_SIZE 1024

typedef struct {
	int hour;
	int minute;
	int second;
} TIME;

typedef struct {
	TIME arrival;
	TIME departure;
} CARPARK_LOG;

int read_log(FILE *input, CARPARK_LOG *buffer, int size) {
	TIME arrival = {};
	TIME departure = {};

	int i = 0;
	while (i < size && fscanf(input, "%d %d %d %d %d %d", &arrival.hour, &arrival.minute, &arrival.second, &departure.hour, &departure.minute, &departure.second) == 6) {
		CARPARK_LOG entry = {arrival, departure};
		buffer[i] = entry;
	
		i++;
	}
	return i;
}

int second_amount(TIME *time) {
	return time -> hour * 3600 + time -> minute * 60 + time -> second;
}

void seconds_to_time(int seconds, TIME *out) {
    out->hour = seconds / 3600;
    out->minute = seconds % 3600 / 60;
    out->second = seconds % 60;
}

void print_entry(int i, TIME *arrival, TIME *departure) {
	int arrival_seconds = second_amount(arrival);
	int departure_seconds = second_amount(departure);
	int seconds = departure_seconds - arrival_seconds;
	TIME total_time = {};
	seconds_to_time(seconds, &total_time);
	printf("%5d. %02d:%02d:%02d %02d:%02d:%02d  %6d %02d:%02d:%02d\n", i + 1, arrival -> hour, arrival -> minute, arrival -> second, departure -> hour, departure -> minute, departure -> second, seconds, total_time.hour, total_time.minute, total_time.second);	
}

void print_entries(CARPARK_LOG *entries, int count) {
	printf("-----------------------------------------\n");
	printf("             CARPARK PROGRAM\n");
	printf("-----------------------------------------\n");
	for (int i = 0; i < count; i++) {
		CARPARK_LOG *entry = &entries[i];
		TIME *arrival = &entry -> arrival;
		TIME *departure = &entry -> departure;
		print_entry(i, arrival, departure);
	}
	printf("-----------------------------------------\n");
}

bool write_entry(FILE *file, int i, TIME *arrival, TIME *departure) {
	int arrival_seconds = second_amount(arrival);
	int departure_seconds = second_amount(departure);
	int seconds = departure_seconds - arrival_seconds;
	int seconds_over_limit = seconds - (FREE_HOUR_AMOUNT * 3600);
	if (seconds_over_limit < 0) return false;

	TIME total_time = {};
	seconds_to_time(seconds, &total_time);

	TIME overlimit_time = {};
	seconds_to_time(seconds_over_limit, &overlimit_time);

	int price = (seconds_over_limit / 3600 + 1) * PRICE_PER_HOUR;
	
	fprintf(file, "| %4d. |   %02d:%02d:%02d   |    %02d:%02d:%02d    |   %02d:%02d:%02d   |  %02d:%02d:%02d  | %5d |\n", i + 1, arrival -> hour, arrival -> minute, arrival -> second, departure -> hour, departure -> minute, departure -> second, total_time.hour, total_time.minute, total_time.second, overlimit_time.hour, overlimit_time.minute, overlimit_time.second, price);	
	return true;
}

void write_entries(FILE *file, CARPARK_LOG *entries, int count) {
	fprintf(file, "                            CARPARK PROGRAM - PRICES\n");
	fprintf(file, "-----------------------------------------------------------------------------\n");
	fprintf(file, "| Order | Arrival Time | Departure Time | Parking Time | Over Limit | Price |\n");
	int written_count = 0;
	for (int i = 0; i < count; i++) {
		CARPARK_LOG *entry = &entries[i];
		TIME *arrival = &entry -> arrival;
		TIME *departure = &entry -> departure;
		written_count += write_entry(file, written_count, arrival, departure);
	}
	fprintf(file, "-----------------------------------------------------------------------------\n");
}

int main(void) {
	FILE *input = fopen(INPUT_FILENAME, "r");
	FILE *output = fopen(OUTPUT_FILENAME, "w");

	if (!input || !output) {
		printf("Files cannot be read!");
		return 1;
	}

	CARPARK_LOG entries[BUFFER_SIZE];
	int entry_count = read_log(input, entries, BUFFER_SIZE);
	printf("Entry count: %d\n", entry_count);

	print_entries(entries, entry_count);
	write_entries(output, entries, entry_count);

	fclose(input);
	fclose(output);

	return 0;
}
