#include "word_search.hpp"
#include <iostream>

int main(int argc, char** argv) {
    size_t width, height;
    srand(time(NULL));
    int seed = rand();
    if (argc > 1) {
        seed = atoi(argv[1]);
    }
    std::vector<std::string> word_bank = {"BOOLEAN", "BREAK", "CHAR", "CLASS", "DOUBLE", "FORLOOP", "INTEGER", "OBJECTS", "RANDOM", "RECURSION", "RETURN", "STRING", "STRUCT", "UNSIGNED", "WHILELOOP"};
    atn::standard_word_search puzzle(seed, word_bank);
    std::cout << puzzle.to_string() << std::endl;
}