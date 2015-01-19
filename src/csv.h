
#ifndef REALBAY_CSV_H
#define REALBAY_CSV_H

#include <stdint.h>
#include <stdio.h>

#define MAX_CSV_COLUMNS    16
#define MAX_CSV_LEN        1024
#define MAX_CSV_LINE_LEN  (MAX_CSV_LEN * MAX_CSV_COLUMNS)

typedef struct csv_t {
	char separator;
} csv_t;

typedef struct csv_col_t {
	char text[MAX_CSV_LEN];
	size_t len;
} csv_col_t;

typedef int (csv_func)(const csv_col_t *columns, const size_t columnCount);

void csv_init(csv_t *csv, const char separator);
size_t csv_parse(csv_t *csv, FILE *fp, csv_func *f);
size_t csv_parse_row(csv_t *csv, const char *text, csv_col_t *columns);

#endif
