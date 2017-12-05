#ifndef ADVENT_OF_CODE_02_SPREADSHEET_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_SPREADSHEET_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

class Spreadsheet {
    std::vector<std::vector<int>> m_spread;
public:
    void addRow(std::vector<int> const& row);
    std::size_t numberOfRows() const;
    std::vector<int> row(std::size_t row_index) const;
    
};

Spreadsheet parseInput(std::string_view input);

int calculateRowDifference(Spreadsheet const& spread, std::size_t row_index);

int calculateSpreadChecksum(Spreadsheet const& spread);

#endif
