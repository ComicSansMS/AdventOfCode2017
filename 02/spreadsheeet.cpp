#include <spreadsheeet.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>

void Spreadsheet::addRow(std::vector<int> const& row)
{
    m_spread.emplace_back(row);
}

std::size_t Spreadsheet::numberOfRows() const
{
    return m_spread.size();
}

std::vector<int> Spreadsheet::row(std::size_t row_index) const
{
    assert(row_index >= 0 && row_index < numberOfRows());
    return m_spread[row_index];
}

Spreadsheet parseInput(std::string_view input)
{
    std::stringstream sstr{std::string(input)};

    Spreadsheet ret;
    std::string line;
    while(std::getline(sstr, line)) {
        std::vector<int> row;
        std::stringstream row_sstr(line);
        std::copy(std::istream_iterator<int>(row_sstr), std::istream_iterator<int>(),
                  std::back_inserter(row));
        ret.addRow(row);
    }

    return ret;
}

int calculateRowDifference(Spreadsheet const& spread, std::size_t row_index)
{
    auto row = spread.row(row_index);
    auto minmax = std::minmax_element(begin(row), end(row));
    return (*minmax.second) - (*minmax.first);
}

int calculateSpreadChecksum(Spreadsheet const& spread)
{
    int acc = 0;
    for(std::size_t i = 0; i < spread.numberOfRows(); ++i)
    {
        acc += calculateRowDifference(spread, i);
    }
    return acc;
}

int calculateRowEvenDivide(Spreadsheet const& spread, std::size_t row_index)
{
    auto row = spread.row(row_index);
    std::optional<int> ret;
    for(std::size_t i = 0; i < row.size() && !ret; ++i) {
        int n1 = row[i];
        for(std::size_t j = 0; j < row.size(); ++j) {
            if(i != j) {
                int n2 = row[j];
                if((n1 > n2) && (n1 % n2 == 0)) {
                    ret = n1 / n2;
                    break;
                }
            }
        }
    }
    assert(ret);
    return *ret;
}

int calculateSpreadChecksumEvenDivide(Spreadsheet const& spread)
{
    int acc = 0;
    for(std::size_t i = 0; i < spread.numberOfRows(); ++i)
    {
        acc += calculateRowEvenDivide(spread, i);
    }
    return acc;
}
