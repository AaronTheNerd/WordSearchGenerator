#include "word_search.hpp"
#include <iostream>

int main(int argc, char** argv) {
    size_t width, height;
    srand(time(NULL));
    int seed = rand();
    if (argc > 1) {
        seed = atoi(argv[1]);
    }
    std::vector<std::string> word_bank = {"subliminal", "hardworker", "lmao", "determined", "hireme", "cpp", "python", "java", "matlab", "problemsovler", "organized", "bestcandidate", "programmer", "criticalthinker", "resilient", "class", "integer", "double", "objectoriented", "struct", "videogames", "boardgames", "sudoku", "puzzle", "jess", "linux", "verilog"};
    atn::word_search<15, 15> puzzle(seed, word_bank);
    std::cout << puzzle.to_string() << std::endl;
}