#include "word_search.hpp"
#include <string>
#include <sstream>

#ifndef _SRC_DISPLAY_HPP_
#define _SRC_DISPLAY_HPP_

namespace atn {

std::string display_text(atn::standard_word_search puzzle) {
    std::stringstream ss;
    ss << "Puzzle:\n";
    for (size_t y = 0; y < 15; ++y) {
        for (size_t x = 0; x < 15; ++x) {
            ss << puzzle.puzzle[x][y] << " ";
        }
        ss << "\n\n";
    }
    ss << "Word Bank:\n";
    for (auto word : puzzle.word_bank) {
        ss << word << "\n";
    }
    ss << "\nSeed: " << puzzle.seed << "\n\n";
    ss << "Sloution: \n";
    for (auto placement : puzzle.solution) {
      ss << placement.word << " (" << placement.start_x << ", " << placement.start_y << "), (" << placement.end_x << ", " << placement.end_y << ")\n";
    }
    return ss.str();
}

std::string display_angular(atn::standard_word_search puzzle) {
    std::stringstream ss;
    ss << "puzzle: string[][] = [\n";
    for (size_t y = 0; y < 15; ++y) {
        ss << "  [";
        for (size_t x = 0; x < 15; ++x) {
            if (x != 0) ss << ", ";
            ss << "\"" << puzzle.puzzle[x][y] << "\"";
        }
        ss << "],\n";
    }
    ss << "];\n\n";
    ss << "answers: IAnswer[] = [";
    for (size_t i = 0; i < puzzle.solution.size(); ++i) {
        auto ans = puzzle.solution[i];
        ss << "  {\n";
        ss << "    answer: \"" << ans.word << "\",\n";
        ss << "    found: false,\n";
        ss << "    start: {\n";
        ss << "      x: " << ans.start_x << ",\n";
        ss << "      y: " << ans.start_y << "\n";
        ss << "    },\n";
        ss << "    end: {\n";
        ss << "      x: " << ans.end_x << ",\n";
        ss << "      y: " << ans.end_y << "\n";
        ss << "    }\n";
        ss << "  },\n";
    }
    ss << "];";
    return ss.str();
}

std::string display(atn::standard_word_search puzzle, std::string format) {
  if (format == "--angular") return atn::display_angular(puzzle);
  return atn::display_text(puzzle);
}

}

#endif
