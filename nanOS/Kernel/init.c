
#include "init.h"
#include "dirs.h"
#include "processManager.h"
#include "videoDriver.h"

static void * const sampleCodeModuleAddress = (void *) CODE_ADDRESS;

void _hlt();
void _sti();

void init() {
	_sti();
	exec_process((uint64_t)sampleCodeModuleAddress, 0);
	while (1) {
		_hlt();
	}
}