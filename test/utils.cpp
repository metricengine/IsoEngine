#include "isoengine/common/utils.h"
#include <iostream>
#include <math.h>

void test_profile()
{
    PROFILE_FUNCTION;

    double s = 0;
    for (int i = 0; i < 50000000; ++i)
        s += std::sin(i);

    for (size_t i = 0; i < 3; ++i) {
        PROFILE_BLOCK("TestBlock");
        double s2 = 0;
        for (int i = 0; i < 1000000; ++i)
            s2 += std::cos(i);
        std::cout << s2 << std::endl;
    }

    for (size_t i = 0; i < 5; ++i) {
        PROFILE_BLOCK("TestBlock2");
        double s2 = 0;
        for (int i = 0; i < 1000000; ++i)
            s2 += std::cos(i) * std::sin(i);
        std::cout << s2 << std::endl;
    }

    std::cout << s << std::endl;
}

void test_assert()
{
    DEBUG_LOG("Print this", 5);
    ASSERT(3 > 2, "Cannot throw");
    ASSERT(2 > 3, "Hurray!");
    std::cout << "Should not print" << std::endl;
}

int main()
{
    test_profile();

    try {
        test_assert();
    } catch (AssertFailure & e) {
        std::cerr << e.what() << std::endl;
    }

    ProfileManager::getInstance().showResult();
}