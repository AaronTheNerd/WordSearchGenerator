#include "word_search.hpp"
#include <iostream>

int main(int argc, char** argv) {
    size_t width, height;
    srand(time(NULL));
    int seed = rand();
    if (argc > 1) {
        seed = atoi(argv[1]);
    }
    std::vector<std::string> word_bank = {"class", "integer", "double", "struct", "recursion", "random", "objects", "boolean", "string", "forloop", "whileloop", "char", "unsigned", "break", "return"};
    atn::word_search<15, 15> puzzle(seed, word_bank);
    std::cout << puzzle.to_string() << std::endl;
}