#include "designlab_math_util.h"

#include <iomanip>
#include <random>
#include <sstream>

#include "cassert_define.h"



namespace designlab
{
	namespace math_util
	{
		double GenerateRandomNumber(const double min, const double max)
		{
			assert(min < max);	//min < max ‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(min, max);
			return dis(gen);
		}

		float GenerateRandomNumber(const float min, const float max)
		{
			assert(min < max);	//min < max ‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(min, max);
			return (float)dis(gen);
		}

		int GenerateRandomNumber(const int min, const int max)
		{
			assert(min < max);	//min < max ‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(min, max);
			return (int)dis(gen);
		}

		float limitRangeAngle(const float angle)
		{
			//atan2‚É‡‚í‚¹‚é‚½‚ßCŠp“x‚ğ -ƒÎ < angle < ƒÎ ‚É‚·‚é (—á‚¦‚Î 340‹¨-20‹,-340‹¨20‹)
			float _res = angle + kFloatPi;

			_res = std::fmodf(_res, 2.0 * kFloatPi);

			if (_res < 0)
			{
				_res += kFloatPi;
			}
			else
			{
				_res -= kFloatPi;
			}

			return _res;
		}


		std::string ConvertFloatToString(const float num, const int digit, const int width)
		{
			std::ostringstream ss;

			ss << std::fixed << std::setprecision(digit) << std::setw(width) << std::setfill(' ') << num;
			
			return std::string();
		}

		std::string ConvertDoubleToString(const double num, const int digit, const int width)
		{
			std::ostringstream ss;

			ss << std::fixed << std::setprecision(digit) << std::setw(width) << std::setfill(' ') << num;

			return std::string();
		}



	}	// namespace math_util

}	// namespace designlab