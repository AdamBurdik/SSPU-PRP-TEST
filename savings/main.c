#include <stdio.h>
#define INPUT_IMPLEMENTATION
#include "input.h"
#define DATE_IMPLEMENTATION
#include "date.h"

#define INTEREST_RATE 0.035f
#define TAX_RATE 0.15f

typedef struct {
    char date[12];
    float start;
    float deposit;
    float interest_rate;
    float tax;
    float final;
} Row;

void write_table(Row rows[], const int count) {
    FILE* file = fopen("savings.txt", "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }

    fprintf(file, "\n%-12s | %-13s | %-11s | %-10s | %-11s | %-12s\n", "Date", "Start", "Deposit", "Interest Rate", "Tax", "Final");
    fprintf(file, "--------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%-12s | %10.2f Kc | %8.2f Kc | %10.2f Kc | %8.2f Kc | %10.2f Kc\n", rows[i].date, rows[i].start,
               rows[i].deposit, rows[i].interest_rate, rows[i].tax, rows[i].final);
    }

    fclose(file);
    printf("Data saved to savings.txt\n");
}

int main() {
    const int current_year = 2026;
    const unsigned int starting_balance = get_int("Current bank balance: ");
    const float monthly_income = (float) get_int("Monthly income: ");

    float current_balance = (float)starting_balance;
    Row rows[12];

    for (int i = 1; i <= 12; i++) {
        const float balance_start = current_balance;

        const float monthly_interest = balance_start * (INTEREST_RATE / 12.0f);
        const float tax = monthly_interest * TAX_RATE;

        const float balance_end = balance_start + (monthly_interest - tax) + monthly_income;

        const int day = get_last_day(i, current_year);
        sprintf(rows[i - 1].date, "%02d.%02d.%d", day, i, current_year);

        rows[i - 1].start = balance_start;
        rows[i - 1].deposit = monthly_income;
        rows[i - 1].interest_rate = monthly_interest;
        rows[i - 1].tax = tax;
        rows[i - 1].final = balance_end;

        current_balance = balance_end;
    }

    write_table(rows, 12);

    return 0;
}
