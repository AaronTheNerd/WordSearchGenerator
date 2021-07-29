// Written by Aaron Barge
// Copyright 2021

#ifndef _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_
#define _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_

#include <algorithm>
#include <array>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#define EMPTY_CHAR '\0'

namespace atn {

// ============================================================================
// ============================== Predefinitions ==============================
// ============================================================================

template<size_t width, size_t height>
using board = std::array<std::array<char, height>, width>;

struct pos {
    size_t x;
    size_t y;
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
std::vector<direction<width, height>> directions = {
    direction<width, height> {-1, -1},
    direction<width, height> {-1, 0},
    direction<width, height> {-1, 1},
    direction<width, height> {0, -1},
    direction<width, height> {0, 1},
    direction<width, height> {1, -1},
    direction<width, height> {1, 0},
    direction<width, height> {1, 1}
};

template<size_t width, size_t height>
class word_search {
  private:
    static int _rand(int);
    static std::string to_upper(std::string);
    static bool check_word(board<width, height>, std::string, direction<width, height>, pos);
    static std::vector<pos> find_places_for_word(board<width, height>, std::string, direction<width, height>);
    static board<width, height> place_word(board<width, height>, std::string, direction<width, height>, pos);
    static std::pair<bool, board<width, height>> generate_word_search(board<width, height>, std::vector<std::pair<std::string, bool>>);
    void check_word_lengths() const;
    void fill_empty_spots();
  public:
    int seed;
    std::vector<std::string> word_bank;
    board<width, height> puzzle;
    explicit word_search(int, std::vector<std::string>);
    std::string to_string() const;
};

// ============================================================================
// ================================ Definitions ===============================
// ============================================================================

template<size_t width, size_t height>
int word_search<width, height>::_rand(int i) {
    return rand() % i;
}

template<size_t width, size_t height>
std::string word_search<width, height>::to_upper(std::string str) {
    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = toupper(str[i]);
    }
    return str;
}

// Checks if a word can fit at a position
template<size_t width, size_t height>
bool word_search<width, height>::check_word(
        board<width, height> puzzle, std::string word, direction<width, height> dir, pos p) {
    for (size_t i = 0, x_curr = p.x, y_curr = p.y; i < word.size(); ++i, x_curr += dir.dx, y_curr += dir.dy) {
        if (puzzle[x_curr][y_curr] != EMPTY_CHAR && puzzle[x_curr][y_curr] != word[i]) {
            return false;
        }
    }
    return true;
}

// Finds all of the possible positions for a word to go in with a specific
// direction
template<size_t width, size_t height>
std::vector<pos> word_search<width, height>::find_places_for_word(
        board<width, height> puzzle, std::string word, direction<width, height> dir) {
    std::vector<pos> result;
    for (size_t x = dir.start_x(word); x < dir.end_x(word); ++x) {
        for (size_t y = dir.start_y(word); y < dir.end_y(word); ++y) {
            if (word_search<width, height>::check_word(puzzle, word, dir, pos{x, y})) {
                result.emplace_back(pos{x, y});
            }
        }
    }
    return result;
}

// Places a word at a specific direction and position
template<size_t width, size_t height>
board<width, height> word_search<width, height>::place_word(
        board<width, height> puzzle, std::string word, direction<width, height> dir, pos p) {
    for (size_t i = 0, x_curr = p.x, y_curr = p.y; i < word.size(); ++i, x_curr += dir.dx, y_curr += dir.dy) {
        puzzle[x_curr][y_curr] = word[i];
    }
    return puzzle;
}

// Recursive method for generating a word search
template<size_t width, size_t height>
std::pair<bool, board<width, height>> word_search<width, height>::generate_word_search(
        board<width, height> puzzle, std::vector<std::pair<std::string, bool>> placed_words) {
    bool all_words_placed = true;
    for (auto word_pair : placed_words) {
        all_words_placed &= word_pair.second;
    }
    if (all_words_placed)
        return std::make_pair(true, puzzle);
    for (size_t i = 0; i < placed_words.size(); ++i) {
        if (placed_words[i].second)
            continue;
        std::string word = placed_words[i].first;
        std::random_shuffle(directions<width, height>.begin(), directions<width, height>.end(),
                word_search<width, height>::_rand);
        for (auto dir : directions<width, height>) {
            auto poses = word_search<width, height>::find_places_for_word(
                    puzzle, word, dir);
            std::random_shuffle(poses.begin(), poses.end(),
                    word_search<width, height>::_rand);
            for (pos p : poses) {
                std::vector<std::pair<std::string, bool>> placed_words_copy = placed_words;
                board<width, height> new_puzzle = word_search<width, height>::place_word(
                        puzzle, word, dir, p);
                placed_words_copy[i].second = true;
                auto puzzle_pair = word_search<width, height>::generate_word_search(
                        new_puzzle, placed_words_copy);
                if (puzzle_pair.first) {
                    return puzzle_pair;
                }
            }
        }
    }
    return std::make_pair(false, puzzle);
}

// Checks all words to be less than the minimum dimension
template<size_t width, size_t height>
void word_search<width, height>::check_word_lengths() const {
    size_t min_bound = std::min(width, height);
    for (auto str : this->word_bank) {
        if (str.size() > min_bound)
            throw std::runtime_error("ERROR: Words cannot be larger than the size of the puzzle");
    }
}

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

// Constructor
template<size_t width, size_t height>
word_search<width, height>::word_search(int seed, std::vector<std::string> word_bank)
        : seed(seed), word_bank(word_bank), puzzle() {
    srand(seed);
    for (size_t i = 0; i < this->word_bank.size(); ++i) {
        this->word_bank[i] = word_search<width, height>::to_upper(
                this->word_bank[i]);
    }
    std::random_shuffle(this->word_bank.begin(), this->word_bank.end(),
            word_search<width, height>::_rand); // Shuffle word bank
    this->check_word_lengths(); // Check word lengths
    this->puzzle.fill({}); // Initialize blank puzzle
    std::vector<std::pair<std::string, bool>> placed_words;
    for (auto str : this->word_bank) {
        placed_words.emplace_back(std::make_pair(str, false));
    }
    auto result = word_search<width, height>::generate_word_search(
            this->puzzle, placed_words); // Place words in puzzle
    if (!result.first) {
        throw std::runtime_error("ERROR: No valid puzzles found.");
    }
    this->puzzle = result.second;
    this->fill_empty_spots(); // Fill in empty slots
}

// Simple to_string
template<size_t width, size_t height>
std::string word_search<width, height>::to_string() const {
    std::string str;
    str += "Puzzle:\n";
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            str += (this->puzzle[x][y] + std::string(" "));
        }
        str += "\n\n";
    }
    str += "Word Bank:\n";
    for (auto word : this->word_bank) {
        str += (word + "\n");
    }
    str += ("\nSeed: " + std::to_string(this->seed));
    return str;
}

}; // End namespace atn

#endif // End define guard
