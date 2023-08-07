#pragma once

#include <vector>

//! @namespacse my_math 基本的な計算を行う関数をまとめた名前空間．
//! @date 2023/08/06
//! @auther 長谷川
//! @brief 基本的な計算を行う関数をまとめた名前空間．
namespace my_math
{
	constexpr double MY_DBL_PI = 3.141592653589793;		//!< double型の円周率
	constexpr float MY_FLT_PI = 3.141592653589793f;		//!< float型の円周率

	constexpr double DBL_ALLOWABLE_ERROR = 0.001;								//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．0.1とか，大きな値は非推奨．
	constexpr float ALLOWABLE_ERROR = static_cast<float>(DBL_ALLOWABLE_ERROR);	//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．0.1とか，大きな値は非推奨．


	//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
	//! @param [in] num1 比較する数字1つ目 
	//! @param [in] num2 比較する数字2つ目
	//! @return bool 等しいならばtrue 
	constexpr bool isEqual(const float num1, const float num2)
	{
		const float dif = num1 - num2;
		if (dif > 0) { return (dif <= ALLOWABLE_ERROR); }
		else { return (dif >= -ALLOWABLE_ERROR); }
	}

	//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
	//! @param [in] num1 比較する数字1つ目 
	//! @param [in] num2 比較する数字2つ目
	//! @return bool 等しいならばtrue 
	constexpr bool isEqual(const double num1, const double num2)
	{
		const double dif = num1 - num2;
		if (dif > 0) { return (dif <= DBL_ALLOWABLE_ERROR); }
		else { return (dif >= -DBL_ALLOWABLE_ERROR); }
	}

	//! @brief 2乗した値を返す関数．
	//! @param [in] num 2乗する数．
	//! @return double 2乗した値． 
	constexpr double squared(const double num) { return num * num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] num 2乗する数．
	//! @return float 2乗した値． 
	constexpr float squared(const float num) { return num * num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] num 2乗する数．
	//! @return int 2乗した値． 
	constexpr int squared(const int num) { return num * num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] num 2乗する数．
	//! @return char 2乗した値． 
	constexpr char squared(const char num) { return num * num; }

	//! @brief 2乗した値を返す関数．
	//! @param [in] num 2乗する数．
	//! @return short 2乗した値．
	constexpr short squared(const short num) { return num * num; }


	//! @brief 指定した範囲内の乱数を生成する．
	//! @param [in] min 乱数の最小値．
	//! @param [in] max 乱数の最大値．
	//! @return double 生成した乱数．
	double generateRandomNumber(const double min, const double max);

	//! @brief 指定した範囲内の乱数を生成する．
	//! @param [in] min 乱数の最小値．
	//! @param [in] max 乱数の最大値．
	//! @return float 生成した乱数．
	float generateRandomNumber(const float min, const float max);

	//! @brief 指定した範囲内の乱数を生成する．
	//! @param [in] min 乱数の最小値．
	//! @param [in] max 乱数の最大値．
	//! @return int 生成した乱数．
	int generateRandomNumber(const int min, const int max);


	//! @brief 角度をradからdegに変換する関数．
	//! @param [in] rad 角度[rad]．
	//! @return double 角度[deg]．
	constexpr double convertRadToDeg(const double rad) { return rad * 180.0 / MY_DBL_PI; };

	//! @brief 角度をradからdegに変換する関数．
	//! @param [in] rad 角度[rad]．
	//! @return float 角度[deg]．
	constexpr float convertRadToDeg(const float rad) { return rad * 180.0f / MY_FLT_PI; };

	//! @brief 角度をdegからradに変換する関数．
	//! @param [in] deg 角度[deg]．
	//! @return double 角度[rad]．
	constexpr double convertDegToRad(const double deg) { return deg * MY_DBL_PI / 180.0; }

	//! @brief 角度をdegからradに変換する関数．
	//! @param [in] deg 角度[deg]．
	//! @return float 角度[rad]．
	constexpr float convertDegToRad(const float deg) { return deg * MY_FLT_PI / 180.0f; }

	float limitRangeAngle(const float angle);

}	// namespace my_math


//! @file my_math.h
//! @date 2023/08/06
//! @auther 長谷川
//! @brief 基本的な計算を行う関数をまとめた名前空間．
//! @n 行数 : @lineinfo