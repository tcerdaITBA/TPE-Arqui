#include "videoDriver.h"
#include "keyboardDriver.h"
#include "systemCalls.h"
#include "interrupts.h"

void sys_write(unsigned int fds, const char * str, unsigned int length) {
	if (fds == STDERR) {
		char backup_color = current_color();
		set_color(ERROR_COLOR);
		put(str, length);
		set_color(backup_color);
	}
	else if (fds == STDOUT)
		put(str, length);
}

void sys_read(unsigned int fds, char * buffer, unsigned int bytes) {
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
}