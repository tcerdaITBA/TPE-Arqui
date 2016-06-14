#include "videoDriver.h"
#include "keyboardDriver.h"
#include "systemCalls.h"
#include "interrupts.h"
#include "rtc.h"

int sys_write(unsigned int fds, const char * str, unsigned int length) {
	if (fds == STDERR) {
		char backup_color = current_color();
		set_color(ERROR_COLOR);
		put(str, length);
		set_color(backup_color);
	}
	else if (fds == STDOUT)
		put(str, length);
	return length;
}

int sys_read(unsigned int fds, char * buffer, unsigned int bytes) {
	unsigned int i = 0;
	char c;
    if (fds == STDIN) {
			while (i < bytes) {
				c = get_char();
				if (c != -1) {
					buffer[i++] = c;
				} else {
					_hlt();  // TODO: porq no anda sin esto???????????????
				}
			}
    }
    return i;
}

int sys_time(int selection) {
    switch(selection) {
	case 0:
	    return seconds();
	case 1:
	    return minutes();
	case 2:
	    return hour();
    }
    return -1;
}

int sys_paint(int color, int x, int y) {
	char r = (color >> 16) & 0xFF;
	char g = (color >> 8) & 0xFF;
	char b = color & 0xFF;

	return fill(r,g,b,x,y); // 1 si fue un pixel válido, 0 sino.
}
