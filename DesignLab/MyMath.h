#pragma once
#include "vectorFunc.h"
#include <vector>

//基本的な計算を行う関数
namespace my_math
{
	constexpr float MY_FLT_PI = 3.141592653589793f;
	constexpr double MY_DBL_PI = 3.141592653589793;

	//2乗した値を返す関数．メジャーな型はオーバーロードした．
	constexpr double squared(const double _num) { return _num * _num; }
	constexpr float squared(const float _num) { return _num * _num; }
	constexpr int squared(const int _num) { return _num * _num; }
	constexpr char squared(const char _num) { return _num * _num; }
	constexpr short squared(const short _num) { return _num * _num; }

	double generateRandomNumber(const double _min, const double _max);	//指定した範囲内の乱数を生成する.
	float generateRandomNumber(const float _min, const float _max);		//指定した範囲内の乱数を生成する.
	int generateRandomNumber(const int _min, const int _max);			//指定した範囲内の乱数を生成する.

	constexpr double convertRadToDeg(const double _rad) { return _rad * 180.0 / MY_DBL_PI; };
	constexpr float convertRadToDeg(const float _rad) { return _rad * 180.0f / MY_FLT_PI; };

	constexpr double convertDegToRad(const double _deg) { return _deg * MY_DBL_PI / 180.0; }
	constexpr float convertDegToRad(const float _deg) { return _deg * MY_FLT_PI / 180.0f; }
}
