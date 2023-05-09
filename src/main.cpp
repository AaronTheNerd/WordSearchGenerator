#include "word_search.hpp"
#include "display.hpp"
#include <iostream>

int main(int argc, char** argv) {
    srand(time(NULL));
    int seed = rand();
    if (argc > 2) {
        seed = atoi(argv[2]);
    }
    std::vector<std::string> word_bank = {
        "PROBLEMSOLVER",
        "CRITICALTHINKER",
        "ORGANIZED",
        "DETERMINED",
        "MOTIVATED",
        "FASTLEARNER",
        "ACCOUNTABLE",
        "RELIABLE",
        "OPENMINDED",
        "PATIENT",
        "DEDICATED",
        "PRODUCTIVE",
        "DETAILORIENTED",
        "HONEST",
        "INNOVATOR",
        "PROFESSIONAL",
        //"PEDANTIC",
        //"PASSIONATE",
        /*==========Easter eggs==========*/
        "HIREME",
        "BESTCANDIDATE",
        "DEVELOPER",
        "CHILL",
        "COOL",
        "NICE",
        "BALD",
        "TALL",
        "SMART",
        "FUNNY",
        "STRONG",
        "COOK",
        "GAMER"
        };
    atn::standard_word_search puzzle(seed, word_bank);
    std::cout << atn::display(puzzle, argv[1]) << std::endl;
}