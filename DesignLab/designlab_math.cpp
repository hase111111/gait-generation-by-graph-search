#include "designlab_math.h"

#include <random>

double dl_math::generateRandomNumber(const double min, const double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
}

float dl_math::generateRandomNumber(const float min, const float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return (float)dis(gen);
}

int dl_math::generateRandomNumber(const int min, const int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return (int)dis(gen);
}

float dl_math::limitRangeAngle(const float angle)
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

