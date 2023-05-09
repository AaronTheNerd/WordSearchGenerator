// ============================================================================
// ========================== Written by Aaron Barge ==========================
// ============================== Copyright 2021 ==============================
// ============================================================================

#ifndef _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_
#define _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#define EMPTY_CHAR '\0'
#define MIN_WORD_LENGTH 3

namespace atn {

// ============================================================================
// ================================ Definitions ===============================
// ============================================================================

template<size_t width, size_t height>
using board = std::array<std::array<char, height>, width>;

struct pos {
    size_t x;
    size_t y;

    pos(size_t x, size_t y) : x(x), y(y) {}
};

template<size_t width, size_t height>
struct direction {
    int dx = 0;
    int dy = 0;
    int start_x(std::string str) {
        return (this->dx < 0) * (str.size() - 1);
    }
    int start_y(std::string str) {
        return (this->dy < 0) * (str.size() - 1);
    }
    int end_x(std::string str) {
        return width + (this->dx > 0) * (1 - str.size());
    }
    int end_y(std::string str) {
        return height + (this->dy > 0) * (1 - str.size());
    }
};

template<size_t width, size_t height>
struct dir_and_pos {
    atn::direction<width, height> dir;
    atn::pos p;
};

template<size_t width, size_t height>
std::vector<direction<width, height>> directions = {
/*
    direction<width, height> {-1, -1},
    direction<width, height> {-1, 0},
    direction<width, height> {-1, 1},
    direction<width, height> {0, -1},
    */
    direction<width, height> {0, 1},
    direction<width, height> {1, -1},
    direction<width, height> {1, 0},
    direction<width, height> {1, 1}
};

struct placed_word {
    std::string word;
    bool placed;
    size_t start_x;
    size_t start_y;
    size_t end_x;
    size_t end_y;
};

template<size_t width, size_t height>
struct generate_result {
    bool success;
    board<width, height> puzzle;
    std::vector<atn::placed_word> placed_words;
};

template<size_t width, size_t height>
class word_search {
  private:
    static int _rand(int);
    static bool _comp(
            atn::placed_word&, atn::placed_word&);
    static std::string to_upper(std::string);
    static bool check_word(
            board<width, height>, std::string, direction<width, height>, pos);
    static std::vector<dir_and_pos<width, height>> find_places_for_word(
            board<width, height>, std::string);
    static atn::generate_result<width, height> place_word(
            board<width, height>, std::string, direction<width, height>, pos, std::vector<atn::placed_word>);
    static atn::generate_result<width, height> generate_word_search(
            board<width, height>, std::vector<atn::placed_word>, size_t);
    static std::vector<std::string> remove_duplicates(
            std::vector<std::string>);
    static void check_word_lengths(std::vector<std::string>);
    static std::vector<std::string> to_upper(std::vector<std::string>);
    void fill_empty_spots();
  public:
    int seed;
    std::vector<std::string> word_bank;
    board<width, height> puzzle;
    std::vector<atn::placed_word> solution;
    bool won;
    explicit word_search(int, std::vector<std::string>);
    char at(const pos&) const;
};

// ============================================================================
// =============================== Declarations ===============================
// ============================================================================

template<size_t width, size_t height>
int word_search<width, height>::_rand(int i) {
    return rand() % i;
}

// ============================================================================

template<size_t width, size_t height>
bool word_search<width, height>::_comp(
        atn::placed_word& str1,
        atn::placed_word& str2) {
    return str1.word.size() > str2.word.size();
}

// ============================================================================

template<size_t width, size_t height>
std::string word_search<width, height>::to_upper(std::string str) {
    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = toupper(str[i]);
    }
    return str;
}

// ============================================================================

// Checks if a word can fit at a position
template<size_t width, size_t height>
bool word_search<width, height>::check_word(
        board<width, height> puzzle,
        std::string word,
        direction<width, height> dir,
        pos p) {
    for (size_t i = 0, x_curr = p.x, y_curr = p.y;
            i < word.size();
            ++i, x_curr += dir.dx, y_curr += dir.dy) {
        if (puzzle[x_curr][y_curr] != EMPTY_CHAR
                && puzzle[x_curr][y_curr] != word[i]) {
            return false;
        }
    }
    return true;
}

// ============================================================================

// Finds all of the possible positions for a word to go in with a specific
// direction
template<size_t width, size_t height>
std::vector<dir_and_pos<width, height>> word_search<width, height>::find_places_for_word(
        board<width, height> puzzle,
        std::string word) {
    std::vector<dir_and_pos<width, height>> result;
    for (auto dir : directions<width, height>) {
        for (size_t x = dir.start_x(word); x < dir.end_x(word); ++x) {
            for (size_t y = dir.start_y(word); y < dir.end_y(word); ++y) {
                if (word_search<width, height>::check_word(
                        puzzle, word, dir, pos{x, y})) {
                    result.emplace_back(dir_and_pos<width, height>{dir, pos{x, y}});
                }
            }
        }
    }
    return result;
}

// ============================================================================

// Places a word at a specific direction and position
template<size_t width, size_t height>
atn::generate_result<width, height> word_search<width, height>::place_word(
        board<width, height> puzzle, std::string word,
        direction<width, height> dir,
        pos p, std::vector<atn::placed_word> placed_words) {
    size_t i, x_curr, y_curr;
    for (i = 0, x_curr = p.x, y_curr = p.y;
            i < word.size();
            ++i, x_curr += dir.dx, y_curr += dir.dy) {
        puzzle[x_curr][y_curr] = word[i];
    }
    for (size_t i = 0; i < placed_words.size(); ++i) {
        if (placed_words[i].word == word) {
            placed_words[i].placed = true;
            placed_words[i].start_x = p.x;
            placed_words[i].start_y = p.y;
            placed_words[i].end_x = x_curr - dir.dx;
            placed_words[i].end_y = y_curr - dir.dy;
        }
    }
    return {false, puzzle, placed_words};
}

// ============================================================================

// Recursive method for generating a word search
template<size_t width, size_t height>
generate_result<width, height> word_search<width, height>::generate_word_search(
        board<width, height> puzzle,
        std::vector<atn::placed_word> placed_words, size_t index) {
    if (index >= placed_words.size()) return {true, puzzle, placed_words};
    std::string word = placed_words[index].word;
    auto positions = word_search<width, height>::find_places_for_word(puzzle, word);
    std::random_shuffle(positions.begin(), positions.end(), word_search<width, height>::_rand);
    for (auto position : positions) {
        auto place_result = word_search<width, height>::place_word(puzzle, word, position.dir, position.p, placed_words);
        auto placed_words_copy = place_result.placed_words;
        auto new_puzzle = place_result.puzzle;
        auto result = 
                word_search<width, height>::generate_word_search(
                        new_puzzle, placed_words_copy, index + 1);
        if (result.success) {
            return result;
        }
    }
    return { false, puzzle, placed_words };
}

// ============================================================================

template<size_t width, size_t height>
std::vector<std::string> word_search<width, height>::remove_duplicates(
        std::vector<std::string> word_bank) {
    std::set<std::string> s(word_bank.begin(), word_bank.end());
    return std::vector<std::string>(s.begin(), s.end());
}

// ============================================================================

template<size_t width, size_t height>
std::vector<std::string> word_search<width, height>::to_upper(
        std::vector<std::string> word_bank) {
    for (size_t i = 0; i < word_bank.size(); ++i) {
        word_bank[i] = word_search<width, height>::to_upper(word_bank[i]);
    }
    return word_bank;
}

// ============================================================================

// Checks all words to be less than the minimum dimension
template<size_t width, size_t height>
void word_search<width, height>::check_word_lengths(
        std::vector<std::string> word_bank) {
    size_t min_bound = std::min(width, height);
    for (auto str : word_bank) {
        if (str.size() >= MIN_WORD_LENGTH && str.size() > min_bound)
            throw std::runtime_error(
                    "ERROR: Words cannot be larger than the size of the puzzle");
    }
}

// ============================================================================

// Fills empty spots with random letters
template<size_t width, size_t height>
void word_search<width, height>::fill_empty_spots() {
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            if (this->puzzle[x][y] == EMPTY_CHAR) {
                this->puzzle[x][y] = 'A' + (rand() % 26);
            }
        }
    }
}

// ============================================================================

// Constructor
template<size_t width, size_t height>
word_search<width, height>::word_search(
        int seed, std::vector<std::string> word_bank)
        : seed(seed), word_bank(), puzzle(), solution(), won(false) {
    srand(seed);
    word_bank = this->remove_duplicates(word_bank); // Remove duplicates
    this->check_word_lengths(word_bank); // Check word lengths
    this->word_bank = this->to_upper(word_bank); // Convert strings to uppercase
    this->puzzle.fill({}); // Initialize blank puzzle
    std::vector<atn::placed_word> placed_words;
    for (auto str : word_bank) {
        placed_words.emplace_back(atn::placed_word{str, false, 0, 0, 0, 0});
    }
    std::sort(placed_words.begin(),
            placed_words.end(),
            word_search<width, height>::_comp);
    auto result = word_search<width, height>::generate_word_search(
            this->puzzle, placed_words, 0); // Place words in puzzle
    if (!result.success) {
        throw std::runtime_error("ERROR: No valid puzzles found.");
    }
    this->puzzle = result.puzzle;
    this->solution = result.placed_words;
    this->fill_empty_spots(); // Fill in empty slots
}

// ============================================================================

template<size_t width, size_t height>
char word_search<width, height>::at(const pos& p) const {
    return this->puzzle[p.x][p.y];
}

// ============================================================================

/*NOTE: This would be a good way to create different difficulties of puzzles*/

class standard_word_search : public word_search<15, 15> {
  public:
    explicit standard_word_search(int seed, std::vector<std::string> word_bank)
            : word_search<15, 15>(seed, word_bank) {}
};

}; // End namespace atn

#endif // End define guard
