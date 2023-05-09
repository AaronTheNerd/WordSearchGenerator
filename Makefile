# Written by Aaron Barge
CC = g++
OUTPUT_BUILD = ./build/run
PUZZLE = puzzle.txt
CFLAGS = -O3 -o $(OUTPUT_BUILD)
FORMAT = --angular
SEED = 0

random: src/main.cpp src/word_search.hpp
	$(CC) $(CFLAGS) src/main.cpp
	$(OUTPUT_BUILD) $(FORMAT) > $(PUZZLE)

set_seed: src/main.cpp src/word_search.hpp
	$(CC) $(CFLAGS) src/main.cpp
	$(OUTPUT_BUILD) $(FORMAT) $(SEED) > $(PUZZLE)

clean:
	rm *.out $(OUTPUT_BUILD)