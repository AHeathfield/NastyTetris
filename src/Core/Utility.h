#pragma once

#include <string>
#include <vector>
// A namespace of functions that are not bound to any class but are useful that I make

namespace utility
{
    bool getRandomBool();
    bool getTrueOneXChance(int x);
    bool isIntOdd(int num);
    bool isIntEven(int num);

    void initVectorInt(std::vector<int>& array, int size, int value);
    std::string getProjectPath(const std::string& file);
}
