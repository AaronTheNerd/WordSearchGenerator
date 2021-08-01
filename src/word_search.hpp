// ============================================================================
// ========================== Written by Aaron Barge ==========================
// ============================== Copyright 2021 ==============================
// ============================================================================

#ifndef _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_
#define _WORDSEARCHGENERATOR_WORD_SEARCH_HPP_

#include <algorithm>
#include <array>
#include <cstdlib>
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
    static bool _comp(
            std::pair<std::string, bool>&, std::pair<std::string, bool>&);
    static std::string to_upper(std::string);
    static bool check_word(
            board<width, height>, std::string, direction<width, height>, pos);
    static std::vector<pos> find_places_for_word(
            board<width, height>, std::string, direction<width, height>);
    static board<width, height> place_word(
            board<width, height>, std::string, direction<width, height>, pos);
    static std::pair<bool, board<width, height>> generate_word_search(
            board<width, height>, std::vector<std::pair<std::string, bool>>);
    static std::vector<std::string> remove_duplicates(
            std::vector<std::string>);
    static void check_word_lengths(
            std::vector<std::string>);
    static std::vector<std::string> to_upper(std::vector<std::string>);
    void fill_empty_spots();
    std::string get_word(const pos&, const pos&) const;
    void check_won();
  public:
    int seed;
    std::vector<std::pair<std::string, bool>> word_bank;
    board<width, height> puzzle;
    board<width, height> solution;
    bool won;
    explicit word_search(int, std::vector<std::string>);
    char at(const pos&) const;
    bool check_selection(const pos&, const pos&);
    bool get_won() const;
    std::string to_string() const;
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
        std::pair<std::string, bool>& str1,
        std::pair<std::string, bool>& str2) {
    return str1.first.size() > str2.first.size();
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
std::vector<pos> word_search<width, height>::find_places_for_word(
        board<width, height> puzzle,
        std::string word,
        direction<width, height> dir) {
    std::vector<pos> result;
    for (size_t x = dir.start_x(word); x < dir.end_x(word); ++x) {
        for (size_t y = dir.start_y(word); y < dir.end_y(word); ++y) {
            if (word_search<width, height>::check_word(
                    puzzle, word, dir, pos{x, y})) {
                result.emplace_back(pos{x, y});
            }
        }
    }
    return result;
}

// ============================================================================

// Places a word at a specific direction and position
template<size_t width, size_t height>
board<width, height> word_search<width, height>::place_word(
        board<width, height> puzzle, std::string word,
        direction<width, height> dir,
        pos p) {
    for (size_t i = 0, x_curr = p.x, y_curr = p.y;
            i < word.size();
            ++i, x_curr += dir.dx, y_curr += dir.dy) {
        puzzle[x_curr][y_curr] = word[i];
    }
    return puzzle;
}

// ============================================================================

// Recursive method for generating a word search
template<size_t width, size_t height>
std::pair<bool, board<width, height>> word_search<width, height>::generate_word_search(
        board<width, height> puzzle,
        std::vector<std::pair<std::string, bool>> placed_words) {
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
        std::random_shuffle(
                directions<width, height>.begin(),
                directions<width, height>.end(),
                word_search<width, height>::_rand);
        for (auto dir : directions<width, height>) {
            auto poses = word_search<width, height>::find_places_for_word(
                    puzzle, word, dir);
            std::random_shuffle(poses.begin(), poses.end(),
                    word_search<width, height>::_rand);
            for (pos p : poses) {
                auto placed_words_copy = placed_words;
                board<width, height> new_puzzle = 
                        word_search<width, height>::place_word(
                                puzzle, word, dir, p);
                placed_words_copy[i].second = true;
                auto puzzle_pair = 
                        word_search<width, height>::generate_word_search(
                                new_puzzle, placed_words_copy);
                if (puzzle_pair.first) {
                    return puzzle_pair;
                }
            }
        }
    }
    return std::make_pair(false, puzzle);
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

template<size_t width, size_t height>
std::string word_search<width, height>::get_word(
    const pos& p1, const pos& p2) const {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if (dx != 0) {
        dx /= abs(dx);
    }
    if (dy != 0) {
        dy /= abs(dy);
    }
    std::string ret;
    for (pos p = p1; p.x != p2.x || p.y != p2.y; p.x += dx, p.y += dy) {
        ret += this->puzzle[p.x][p.y];
    }
    ret += this->puzzle[p2.x][p2.y];
    return ret;
}

// ============================================================================

template<size_t width, size_t height>
void word_search<width, height>::check_won() {
    bool res = true;
    for (std::pair<std::string, bool> word_found_pair : this->word_bank) {
        res &= word_found_pair.second;
    }
    this->won = res;
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
    word_bank = this->to_upper(word_bank); // Convert strings to uppercase
    this->puzzle.fill({}); // Initialize blank puzzle
    std::vector<std::pair<std::string, bool>> placed_words;
    for (auto str : word_bank) {
        placed_words.emplace_back(std::make_pair(str, false));
    }
    this->word_bank = placed_words;
    std::sort(placed_words.begin(),
            placed_words.end(),
            word_search<width, height>::_comp);
    auto result = word_search<width, height>::generate_word_search(
            this->puzzle, placed_words); // Place words in puzzle
    if (!result.first) {
        throw std::runtime_error("ERROR: No valid puzzles found.");
    }
    this->puzzle = result.second;
    this->solution = this->puzzle;
    this->fill_empty_spots(); // Fill in empty slots
}

// ============================================================================

template<size_t width, size_t height>
char word_search<width, height>::at(const pos& p) const {
    return this->puzzle[p.x][p.y];
}

// ============================================================================

template<size_t width, size_t height>
bool word_search<width, height>::check_selection(
        const pos& p1, const pos& p2) {
    std::string selected_word = word_search<width, height>::get_word(p1, p2);
    std::string rev_selected_word(
            selected_word.rbegin(), selected_word.rend());
    for (size_t i = 0; i < this->word_bank.size(); ++i) {
        if ((this->word_bank[i].first == selected_word
                || this->word_bank[i].first == rev_selected_word)
                && !this->word_bank[i].second) {
            this->word_bank[i].second = true;
            this->check_won();
            return true;
        }
    }
    return false;
}

// ============================================================================

template<size_t width, size_t height>
bool word_search<width, height>::get_won() const {
    return this->won;
}

// ============================================================================

// Simple to_string
template<size_t width, size_t height>
std::string word_search<width, height>::to_string() const {
    std::string str;
    str += "Puzzle:\n";
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            str += (this->puzzle[x][y] + std::string(" "));
        }
        str += "\n\n";
    }
    str += "Word Bank:\n";
    for (auto word : this->word_bank) {
        str += (word.first + "\n");
    }
    str += ("\nSeed: " + std::to_string(this->seed) + "\n\n");
    str += "Sloution: \n";
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            str += (this->solution[x][y] == EMPTY_CHAR ?
                    ' ' : this->solution[x][y]);
        }
        str += "\n";
    }
    return str;
}

}; // End namespace atn

#endif // End define guard
