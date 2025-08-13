#include "Utility.h"
#include <SDL3/SDL.h>
#include <cstdlib>

namespace utility
{
    bool getRandomBool()
    {
        int num = rand() % 2;
        return (num == 0);
    }

    bool getTrueOneXChance(int x)
    {
        int num = rand() % x;
        return (num == 0);
    }

    bool isIntOdd(int num)
    {
        return ((num + 1) % 2 == 0);
    }

    bool isIntEven(int num)
    {
        return (num % 2 == 0);
    }

    void initVectorInt(std::vector<int>& array, int size, int value)
    {
        for (int i = 0; i < size; i++)
        {
            array.push_back(value);
        }
    }

    std::string getProjectPath(const std::string& file) {
        // const char* basePath = SDL_GetBasePath();
        std::string path = SDL_GetBasePath();
// #ifdef __APPLE__
//         path += "../Resources/";
// #endif
        path += file; // append relative asset path
        return path;
    }
}
