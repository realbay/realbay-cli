
#include "csv.h"

void csv_init(csv_t *csv, const char separator) {
	csv->separator = separator;
}

void csv_col_clear(csv_col_t *t) {
	t->text[0] = '\0';
	t->len = 0;
}

void csv_col_append(csv_col_t *t, const char c) {
	t->text[t->len] = c;
	t->len++;
	t->text[t->len] = '\0';
}

size_t csv_parse(csv_t *csv, FILE *fp, csv_func *f) {
	size_t rowCount = 0;
	size_t columnCount;
	csv_col_t columns[MAX_CSV_COLUMNS];	
	char line[MAX_CSV_LINE_LEN];
	int i;
	
	for (i=0; i < MAX_CSV_COLUMNS; i++) {
		csv_col_clear(&columns[i]);
	}
	
	line[0] = '\0';
	
	while (!feof(fp)) {
		if (fgets(line, MAX_CSV_LINE_LEN, fp) == NULL) {
			break;
		}
		
		if (line == NULL) {
			break;
		}
		
		columnCount = csv_parse_row(csv, line, columns);
		
		if (f(columns, columnCount) != 0) {
			break;
		}
		
		rowCount++;
	}
	
	return rowCount;
}

size_t csv_parse_row(csv_t *csv, const char *line, csv_col_t *columns) {
	size_t columnIndex = 0;
	char c;
	int i;
	char quote_type;
	int in_quote = 0;
	
	csv_col_clear(&columns[0]);
	
	if (line[0] == '\0' || line[0] == '\n') {
		return 0;
	}
	
	for (i=0; i < MAX_CSV_LINE_LEN; i++) {
		c = line[i];
		
		if (c == 0 || c == '\n') {
			break;
		}
		
		if (in_quote) {
			if (c == quote_type) {
				in_quote = 0;
			} else {
				csv_col_append(&columns[columnIndex], c);
			}
			
			continue;
		} else {
			if (c == '"' || c == '\'') {
				in_quote = 1;
				quote_type = c;
				continue;
			}
		}
		
		if (c == csv->separator) {
			columnIndex++;
			
			if (columnIndex >= MAX_CSV_COLUMNS) {
				break;
			}
			
			csv_col_clear(&columns[columnIndex]);
			continue;
		}
		
		csv_col_append(&columns[columnIndex], c);
	}
	
	return columnIndex;
}
