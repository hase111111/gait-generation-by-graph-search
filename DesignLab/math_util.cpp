#include "math_util.h"

#include <iomanip>
#include <random>
#include <sstream>

#include "cassert_define.h"


namespace designlab::math_util
{

double GenerateRandomNumber(const double min, const double max)
{
	assert(min < max);	//min < max でなければならない

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
}

float GenerateRandomNumber(const float min, const float max)
{
	assert(min < max);	//min < max でなければならない

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return (float)dis(gen);
}

int GenerateRandomNumber(const int min, const int max)
{
	assert(min < max);	//min < max でなければならない

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return (int)dis(gen);
}

float limitRangeAngle(const float angle)
{
	//atan2に合わせるため，角度を -π < angle < π にする (例えば 340°→-20°,-340°→20°)
	float res = angle + MathConst<float>::kPi;

	res = std::fmodf(res, 2.0 * MathConst<float>::kPi);

	if (res < 0)
	{
		res += MathConst<float>::kPi;
	}
	else
	{
		res -= MathConst<float>::kPi;
	}

	return res;
}


std::string ConvertFloatToString(const float num, const int digit, const int width)
{
	std::ostringstream ss;

	ss << std::fixed << std::setprecision(digit);
	ss << std::setw(width) << std::setfill(' ') << num;

	return ss.str();
}

std::string ConvertDoubleToString(const double num, const int digit, const int width)
{
	std::ostringstream ss;

	ss << std::fixed << std::setprecision(digit);
	ss << std::setw(width) << std::setfill(' ') << num;

	return ss.str();
}


}	// namespace designlab::math_util