include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
ASMSOURCES=$(wildcard *.asm)
SOURCES=$(wildcard *.c)
ASMOBJECTS=$(ASMSOURCES:.asm=.o)
COBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(GCC) -c $(GCCFLAGS) $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

all: $(USER)

$(USER): $(ASMOBJECTS) $(COBJECTS)
	$(LD) -T sampleCodeModule.ld $(COBJECTS) $(ASMOBJECTS) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
