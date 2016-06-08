#include "videoDriver.h"
#include "strings.h"
#define MAX_DIGITS 15

#define ROW(p) ((p)/WIDTH)
#define COL(p) ((p)%WIDTH)

static char *video = (char *) 0xB8000;
static char color = DEFAULT_COLOR;
static int cursor = 0; /* solo para put_char por ahora */

static unsigned int count_digits(int num);
static char valid_pos(int row, int col);

static char valid_pos(int row, int col) {
	return row < HEIGHT && col < WIDTH && row >= 0 && col >= 0;
}

static unsigned int count_digits(int num) {
	int digits = 1;
	if (num < 0)
		num *= -1;
	while ((num /= 10) > 0)
		digits++;
	return digits;
}

char current_color() {
	return color;
}

void set_color(char c) {
	color = c;
}

void print_str(const char *str, int row, int col) {
	int backup = cursor;
	cursor = row * WIDTH + col;
	while (*str != '\0')
		put_char(*str++);
	cursor = backup;
}

void print_char(char c, int row, int col) {
	if (valid_pos(row, col)) {
		int position = (WIDTH*row + col)*2; /* duplica debido a los colores */
		video[position] = c;
		video[position+1] = color;
	}
}

void put(const char *str, int len) {
	int i;
	for (i = 0; i < len; i++)
		put_char(str[i]);
}

void put_str(const char *str) {
	while(*str != '\0')
		put_char(*str++);
}


void put_char(char c) {
	if (c == '\b') {
		if (cursor > 0) {
	    	    cursor--;
		    print_char(' ', ROW(cursor), COL(cursor));
		}
	}
	else if (c == '\n') {
		cursor += WIDTH - COL(cursor); /* cursor al comienzo de nueva linea */
	}
	else if (c == '\t') { /* reset cursor */
		cursor = 0;
	}
	else {
		print_char(c, ROW(cursor), COL(cursor));
		cursor++;
	}
}

void print_num(int num, int row, int col) {
	unsigned int digits = count_digits(num);
	char str[MAX_DIGITS];
	int i;
	int k = 0;

	if (num < 0) {
		num *= -1;
		str[0] = '-';
		digits += 1;
		k = 1;
	}

	for (i = digits-1; i >= k; i--) {
		str[i] = (num % 10) + '0';
		num /= 10;
	}

	print(str, digits, row, col);
}

void print(const char *str, int len, int row, int col) {
	if (valid_pos(row, col)) {
		int i;
		int pos = row * WIDTH + col;
		for (i = 0; i < len && valid_pos(ROW(pos), COL(pos)); i++, pos++)
			print_char(str[i], ROW(pos), COL(pos));
	}
}

void clear() {
	int i = 0;
	char backup = color;
	set_color(CLEAR_COLOR);
	for (i = 0; i < WIDTH*HEIGHT; i ++)
		print_char(' ',ROW(i), COL(i));
	set_color(backup);
	cursor = 0;
}
