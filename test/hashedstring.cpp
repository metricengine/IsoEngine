#include "hashedstring.h"
#include <iostream>
#include <string>

constexpr size_t testConstExpr()
{
    return iso::HashedString("constexpr").getHash();
}

size_t testNonConstExpr()
{
    std::string str("non constexpr");
    return iso::HashedString(str.c_str()).getHash();
}

int main()
{
    size_t val1 = testConstExpr();
    size_t val2 = testNonConstExpr();

    std::cout << val1 << std::endl;
    std::cout << val2 << std::endl;
}