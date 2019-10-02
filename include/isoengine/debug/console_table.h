#ifndef CONSOLETABLE_H
#define CONSOLETABLE_H

#include <string>
#include <vector>

class ConsoleTable
{
    using Row = std::vector<std::string>;
    using Rows = std::vector<std::vector<std::string>>;

public:
    ConsoleTable() = default;

    ConsoleTable(size_t columnCount, size_t rowCount);

    template <typename Iterator>
    ConsoleTable(Iterator begin, Iterator end);

    explicit ConsoleTable(Row header);
    explicit ConsoleTable(std::initializer_list<std::string> header);

    ConsoleTable(Row header, Rows rows);

    void reserve(size_t rowCount) { rows.reserve(rowCount); }
    void setRows(Rows rows) { this->rows = std::move(rows); }
    void addRow(Row row) { rows.push_back(row); }
    void addColumn(std::vector<std::string> col);
    void display(std::ostream & os) const;
    size_t headerSize() const { return header.size(); }
    size_t tableSize() const { return rows.size(); }

private:
    Row header;
    Rows rows;

    std::string createCenteredString(const std::string & str, size_t width) const;
};

#endif