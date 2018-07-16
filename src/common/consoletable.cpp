#include "isoengine/common/consoletable.h"
#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>

ConsoleTable::ConsoleTable(size_t columnCount, size_t rowCount)
{
    header.reserve(columnCount);
    rows.reserve(rowCount);
    for (auto & row : rows)
        row.reserve(columnCount);
}

template <typename Iterator>
ConsoleTable::ConsoleTable(Iterator begin, Iterator end) : header(begin, end)
{
}

ConsoleTable::ConsoleTable(Row header) : header(std::move(header))
{
}

ConsoleTable::ConsoleTable(std::initializer_list<std::string> header) : header(header)
{
}

ConsoleTable::ConsoleTable(Row header, Rows rows) : header(std::move(header)), rows(std::move(rows))
{
}

void ConsoleTable::addColumn(std::vector<std::string> col)
{
    if (col.size() != rows.size() + 1)
        throw std::invalid_argument("column size has to be equal to the number of rows + 1");
    header.push_back(col[0]);
    for (size_t i = 1; i < col.size(); ++i)
        rows[i - 1].push_back(col[i]);
}

void ConsoleTable::display(std::ostream & os) const
{
    std::vector<size_t> colSizes;
    std::transform(header.begin(), header.end(), std::back_inserter(colSizes), [](std::string str) { return str.size(); });

    size_t totalHeaderSize = 0;

    for (const Row & row : rows)
        for (size_t i = 0; i < row.size(); ++i)
            if (row[i].size() > colSizes[i])
                colSizes[i] = row[i].size();

    for (size_t i = 0; i < header.size(); ++i) {
        os << '|' << createCenteredString(header[i], colSizes[i] + 2);
        // +3 because of the exra two spaces and a separator for each column
        totalHeaderSize += colSizes[i] + 3;
    }
    ++totalHeaderSize;

    os << '|' << std::endl;
    os << std::string(totalHeaderSize, '-') << std::endl;

    for (const Row & row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            os << '|' << createCenteredString(row[i], colSizes[i] + 2);
        }
        os << '|' << std::endl;
    }
}

std::string ConsoleTable::createCenteredString(const std::string & str, size_t width) const
{
    size_t diff = width - str.size();
    size_t leftPad = diff / 2;
    size_t rightPad = diff - leftPad;
    return std::string(leftPad, ' ') + str + std::string(rightPad, ' ');
}