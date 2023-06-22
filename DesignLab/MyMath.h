#pragma once
#include <vector>

//基本的な計算を行う関数
namespace my_math
{
	constexpr float MY_FLT_PI = 3.141592653589793f;		//!< float型の円周率
	constexpr double MY_DBL_PI = 3.141592653589793;		//!< double型の円周率
	constexpr double DBL_ALLOWABLE_ERROR = 0.001;					//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．0.1とか，大きな値は非推奨．
	constexpr float ALLOWABLE_ERROR = (float)DBL_ALLOWABLE_ERROR;	//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．0.1とか，大きな値は非推奨．

	//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
	//! @param [in] _num1 比較する数字1つ目 
	//! @param [in] _num2 比較する数字2つ目
	//! @return bool 等しいならばtrue 
	constexpr bool isEqual(const float _num1, const float _num2) 
	{
		const float dif = _num1 - _num2;
		if (dif > 0) { return (dif <= ALLOWABLE_ERROR); }
		else { return (dif >= -ALLOWABLE_ERROR); }
	}

	//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
	//! @param [in] _num1 比較する数字1つ目 
	//! @param [in] _num2 比較する数字2つ目
	//! @return bool 等しいならばtrue 
	constexpr bool isEqual(const double _num1, const double _num2)
	{
		const double dif = _num1 - _num2;
		if (dif > 0) { return (dif <= DBL_ALLOWABLE_ERROR); }
		else { return (dif >= -DBL_ALLOWABLE_ERROR); }
	}

	//! @brief 2乗した値を返す関数．
	//! @param [in] _num 2乗する数．
	//! @return double 2乗した値． 
	constexpr double squared(const double _num) { return _num * _num; }	

	//! @brief 2乗した値を返す関数．
	//! @param [in] _num 2乗する数．
	//! @return float 2乗した値． 
	constexpr float squared(const float _num) { return _num * _num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] _num 2乗する数．
	//! @return int 2乗した値． 
	constexpr int squared(const int _num) { return _num * _num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] _num 2乗する数．
	//! @return char 2乗した値． 
	constexpr char squared(const char _num) { return _num * _num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] _num 2乗する数．
	//! @return short 2乗した値．
	constexpr short squared(const short _num) { return _num * _num; }


	double generateRandomNumber(const double _min, const double _max);	//指定した範囲内の乱数を生成する.
	float generateRandomNumber(const float _min, const float _max);		//指定した範囲内の乱数を生成する.
	int generateRandomNumber(const int _min, const int _max);			//指定した範囲内の乱数を生成する.

	constexpr double convertRadToDeg(const double _rad) { return _rad * 180.0 / MY_DBL_PI; };
	constexpr float convertRadToDeg(const float _rad) { return _rad * 180.0f / MY_FLT_PI; };

	constexpr double convertDegToRad(const double _deg) { return _deg * MY_DBL_PI / 180.0; }
	constexpr float convertDegToRad(const float _deg) { return _deg * MY_FLT_PI / 180.0f; }
}
