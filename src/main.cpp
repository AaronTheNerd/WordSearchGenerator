#include "word_search.hpp"
#include <iostream>

int main(int argc, char** argv) {
    size_t width, height;
    srand(time(NULL));
    int seed = rand();
    if (argc > 1) {
        seed = atoi(argv[1]);
    }
    std::vector<std::string> word_bank = {"cpp", "car", "cup"};
    atn::word_search<3, 3> puzzle(seed, word_bank);
    std::cout << puzzle.to_string() << std::endl;
}