#pragma once

//基本的な計算を行う関数
namespace my_math
{
	//2乗した値を返す関数．メジャーな型はオーバーロードした．
	constexpr double squared(const double _num) { return _num * _num; }
	constexpr float squared(const float _num) { return _num * _num; }
	constexpr int squared(const int _num) { return _num * _num; }
	constexpr char squared(const char _num) { return _num * _num; }
	constexpr short squared(const short _num) { return _num * _num; }
}