CC       = gcc
CCFLAGS  = -Wall
OPTFLAGS = -O2 -fopenmp
INCLUDE  = -I ./include
LIBS     = -lm

# @TODO: find the default installed version of python
PYTHON   = python3.8


sources  = $(wildcard src/*.c)
examples = $(wildcard examples/*.c)

objects  = $(patsubst src/%.c,build/%.o,$(sources))

targets  += $(patsubst %.c,build/%,$(examples))
targets  += $(objects)


all: serial


serial: $(targets)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)


build/%.o: src/%.c
	$(CC) -c $(INCLUDE) $(CCFLAGS) -o $@ $^


output.bin: serial
	./serial data/input.txt $@


report: output.bin
	$(PYTHON) scripts/plotting2D.py $^


test: output.bin
	$(PYTHON) scripts/compare.py cuda/reference.bin $^


.PHONY folder:
folder:
	mkdir -p build


.PHONY clean:
clean:
	rm -f $(targets) serial output.bin
