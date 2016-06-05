#include "videoDriver.h"
#include "systemCalls.h"

void sys_write(unsigned int fds, const char * str, unsigned int length) {
	if (fds == STDERR) {
		char backup_color = current_color();
		set_color(ERROR_COLOR);
		put(str, length);
		set_color(backup_color);
	}
	else
		put(str, length);
}
