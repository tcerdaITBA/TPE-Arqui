
#include "init.h"
#include "dirs.h"
#include "processManager.h"
#include "videoDriver.h"
#include "process.h"

static void * const sampleCodeModuleAddress = (void *) CODE_ADDRESS;

void _hlt();
void _sti();

void init() {
	_sti();
	exec_process((uint64_t)sampleCodeModuleAddress, 0);
	set_foreground_process (get_process_by_pid(1));
	while (1) {
		_hlt();
	}
}