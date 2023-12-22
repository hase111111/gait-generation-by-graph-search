#ifndef DESIGNLAB_MATH_UTIL_H_
#define DESIGNLAB_MATH_UTIL_H_

#include <string>
#include <vector>

#include "cassert_define.h"
#include "math_const.h"



//! @namespacse designlab::math_util 基本的な計算を行う関数をまとめた名前空間．
//! @brief 基本的な計算を行う関数をまとめた名前空間．
namespace designlab::math_util
{

//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
//! @tparam T floatかdoubleのみを想定している．その他の型を使用する場合エラーが出る．
//! @param [in] num1 比較する数字1つ目．
//! @param [in] num2 比較する数字2つ目．
//! @return bool 等しいならばtrue．
template <typename T, typename = std::enable_if_t<std::is_same<T, float>::value || std::is_same<T, double>::value>>
constexpr bool IsEqual(const float num1, const float num2) noexcept
{
	const float dif = num1 - num2;
	if (dif > 0)
	{
		return (dif <= MathConst<T>::kAllowableError);
	}
	else
	{
		return (dif >= -MathConst<T>::kAllowableError);
	}
}

//! @brief 2乗した値を返す関数．
//! @n 整数型や，小数型のみを想定して作っているので，他の型で使うとエラーが出るかも．
//! @param [in] num 2乗する数．
//! @return T 2乗した値． 
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr T Squared(const T num) noexcept { return num * num; }

//! @brief 3辺で三角形が作れるか調べる関数．
//! @param [in] a 1辺目．
//! @param [in] b 2辺目．
//! @param [in] c 3辺目．
//! @return bool 三角形が作れるならばtrue．
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr bool CanMakeTriangle(const T a, const T b, const T c) noexcept
{
	assert(a > 0);
	assert(b > 0);
	assert(c > 0);
	return (a + b > c && b + c > a && c + a > b);
}

//! @brief 目標値に値を近づける関数．
//! @n 適当に作っている，線形でもない，描画用なので計算に使いたいなら作り直すこと
//! @param [in] current 現在の値．
//! @param [in] target 目標値．
//! @param [in] rate 近づける割合．0 ～ 1の値を取る．
//! @return T 近づけた値．
template <typename T>
T ApproachTarget(const T& current, const T& target, float rate)
{
	assert(0 <= rate);
	assert(rate <= 1);	//0 <= rate <= 1 でなければならない

	if (current == target) { return current; }

	return static_cast<T>(current * (1 - rate) + target * rate);
}

//! @brief 指定した範囲内の乱数を生成する．
//! @param [in] min 乱数の最小値．
//! @param [in] max 乱数の最大値．
//! @return double 生成した乱数．
double GenerateRandomNumber(double min, double max);

//! @brief 指定した範囲内の乱数を生成する．
//! @param [in] min 乱数の最小値．
//! @param [in] max 乱数の最大値．
//! @return float 生成した乱数．
float GenerateRandomNumber(float min, float max);

//! @brief 指定した範囲内の乱数を生成する．
//! @param [in] min 乱数の最小値．
//! @param [in] max 乱数の最大値．
//! @return int 生成した乱数．
int GenerateRandomNumber(int min, int max);


//! @brief 角度をradからdegに変換する関数．
//! @param [in] rad 角度[rad]．
//! @return double 角度[deg]．
constexpr double ConvertRadToDeg(const double rad) noexcept { return rad * 180.0 / kDoublePi; };

//! @brief 角度をradからdegに変換する関数．
//! @param [in] rad 角度[rad]．
//! @return float 角度[deg]．
constexpr float ConvertRadToDeg(const float rad) noexcept { return rad * 180.0f / kFloatPi; };

//! @brief 角度をdegからradに変換する関数．
//! @param [in] deg 角度[deg]．
//! @return double 角度[rad]．
constexpr double ConvertDegToRad(const double deg) noexcept { return deg * kDoublePi / 180.0; }

//! @brief 角度をdegからradに変換する関数．
//! @param [in] deg 角度[deg]．
//! @return float 角度[rad]．
constexpr float ConvertDegToRad(const float deg) noexcept { return deg * kFloatPi / 180.0f; }


[[deprecated]]
float limitRangeAngle(const float angle);


constexpr int kDigit = 3;	//!< 小数点以下の桁数．
constexpr int kWidth = 10;	//!< 文字列の幅．

//! @brief 小数を文字列に変換する関数．
//! @n C++ では C のフォーマットのように %3.3f とかで小数を文字列に変換できないため自作する．
//! @param [in] num 変換する小数．
//! @param [in] digit 小数点以下の桁数．
//! @param [in] width 文字列の幅．
//! @return std::string 変換した文字列．
std::string ConvertFloatToString(const float num, const int digit = kDigit, const int width = kWidth);

//! @brief 小数を文字列に変換する関数．
//! @n C++ では C のフォーマットのように %3.3f とかで小数を文字列に変換できないため自作する．
//! @param [in] num 変換する小数．
//! @param [in] digit 小数点以下の桁数．
//! @param [in] width 文字列の幅．
//! @return std::string 変換した文字列．
std::string ConvertDoubleToString(const double num, const int digit = kDigit, const int width = kWidth);


}	// namespace designlab::math_util


#endif	// DESIGNLAB_MATH_UTIL_H_