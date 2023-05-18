#include "MyMath.h"
#include <random>

double my_math::generateRandomNumber(const double _min, const double _max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(_min, _max);
    return dis(gen);
}

float my_math::generateRandomNumber(const float _min, const float _max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(_min, _max);
    return (float)dis(gen);
}

int my_math::generateRandomNumber(const int _min, const int _max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(_min, _max);
    return (int)dis(gen);
}
