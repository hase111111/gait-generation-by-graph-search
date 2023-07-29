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

float my_math::limitRangeAngle(const float angle)
{
	//atan2‚É‡‚í‚¹‚é‚½‚ßCŠp“x‚ğ -ƒÎ < angle < ƒÎ ‚É‚·‚é (—á‚¦‚Î 340‹¨-20‹,-340‹¨20‹)
	float _res = angle + MY_FLT_PI;

	_res = std::fmodf(_res, 2.0 * MY_FLT_PI);

	if (_res < 0)
	{
		_res += MY_FLT_PI;
	}
	else
	{
		_res -= MY_FLT_PI;
	}

	return _res;
}

