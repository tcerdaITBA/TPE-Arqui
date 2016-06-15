#include "videoDriver.h"
#include "strings.h"
#include "font.h"
#include "lib.h"

#define MAX_DIGITS 15

#define ROW(p) ((p)/WIDTH)
#define COL(p) ((p)%WIDTH)
#define CURSOR_LIMIT HEIGHT*WIDTH

static char color = DEFAULT_COLOR;
static int cursor = 0; /* solo para put_char por ahora */

static unsigned char ** VBEPhysBasePtr = (unsigned char**) 0x0005C28;
//static int pixeCursor = 0;

static unsigned int count_digits(int num);
static char valid_pos(int row, int col);
static void print_char_line(char line, int x, int y);
static int valid_pixel(int x, int y);
static void scrollUp();
static void clear_line(int line);

static int valid_pixel(int x, int y) {
	return (x >= 0 && x <= SCREEN_WIDTH && y >= 0 && y <= SCREEN_HEIGHT);
}

static void scrollUp() {
	unsigned char * linearVESABuffer = *VBEPhysBasePtr;
	unsigned char * second_line = linearVESABuffer + (3 * SCREEN_WIDTH * CHAR_HEIGHT);
	uint64_t length = (SCREEN_HEIGHT * SCREEN_WIDTH * 3) - 
						(CHAR_HEIGHT * SCREEN_WIDTH * 3);
	memcpy(linearVESABuffer, second_line, length);
	clear_line(HEIGHT - 1);
}

static void clear_line(int line) {
	if (line >= 0 && line < HEIGHT) {
		int i;
		for (i=0; i < WIDTH; i++)
			print_char(' ', line, i);
	}
}

int fill(char red, char green, char blue, int x, int y) {
	if (!valid_pixel(x, y))
		return 0;

	unsigned char * linearVESABuffer = *VBEPhysBasePtr;
	unsigned char * VBEAdress = linearVESABuffer + 3 * (x + y * SCREEN_WIDTH);
	VBEAdress[0] = blue;
	VBEAdress[1] = green;
	VBEAdress[2] = red;
	return 1;
}

static void fillWhite(int x, int y) {
	fill(255, 255, 255, x, y); // pinta de blanco
}

static void fillBlack(int x, int y) {
	fill(0, 0, 0, x, y); // pinta de negro
}

void print_char(char c, int row, int col) {
	if (valid_pos(row, col)) { // Ver que onda los tamaños
		int y = row * CHAR_HEIGHT;
		int x = col * CHAR_WIDTH;

		int i = 0;
		unsigned char * char_map = pixel_map(c); // Donde empiezan los mapeos a ascii en el arreglo font.

		for(i = 0; i < CHAR_HEIGHT; i++) {
			unsigned char pixel = char_map[i];
			print_char_line(pixel, x, y+i);
		}
	}
}

static void print_char_line(char line, int x, int y) {
	char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
	int j = 0;
	char aux;
	for (j = 0; j < CHAR_WIDTH; j++) {
		aux = line & mask[j];
		if (aux != 0) { // Tiene que escribir en ese lugar
			fillWhite(x+j, y);
		} else {
			fillBlack(x+j, y);
		}
	}
}

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

/*
void print_char(char c, int row, int col) {
	if (valid_pos(row, col)) {
		int position = (WIDTH*row + col)*2;  duplica debido a los colores
		video[position] = c;
		video[position+1] = color;
	}
}
*/

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
		cursor--;
		print_char(' ', ROW(cursor), COL(cursor));
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

	if (cursor < 0)
		cursor = 0;
	if (cursor >= CURSOR_LIMIT) {
		scrollUp();
		cursor = CURSOR_LIMIT - WIDTH;
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
