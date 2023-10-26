//! @file designlab_math_util.h
//! @brief 基本的な計算を行う関数．


#ifndef DESIGNLAB_MATH_UTIL_H_
#define DESIGNLAB_MATH_UTIL_H_


#include <string>
#include <vector>

#include "cassert_define.h"


//! @namespace designlab
//! @brief DesignLabの名前空間
//! @details このプロジェクトで作成した関数は全てこの名前空間内に入れる．
//! @n グローバルな空間に関数を入れると，名前の衝突が起こる可能性がある．
namespace designlab 
{
	//! @namespacse math_util 基本的な計算を行う関数をまとめた名前空間．
	//! @brief 基本的な計算を行う関数をまとめた名前空間．
	namespace math_util 
	{
		constexpr double kDoublePi = 3.141592653589793;		//!< double型の円周率
		constexpr float kFloatPi = 3.141592653589793f;		//!< float型の円周率

		constexpr double kDoubleAllowableError = 0.001;									//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．小数点3桁未満の誤差を無視する．
		constexpr float kAllowableError = static_cast<float>(kDoubleAllowableError);	//!< これ以上小さい値は0とみなす．allowable error，許容誤差のこと．小数点3桁未満の誤差を無視する．


		//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
		//! @param [in] num1 比較する数字1つ目 
		//! @param [in] num2 比較する数字2つ目
		//! @return bool 等しいならばtrue 
		constexpr bool IsEqual(const float num1, const float num2)
		{
			const float dif = num1 - num2;
			if (dif > 0) { return (dif <= kAllowableError); }
			else { return (dif >= -kAllowableError); }
		}

		//! @brief C++において，小数同士の計算は誤差が出てしまう．誤差込みで値が等しいか調べる．
		//! @param [in] num1 比較する数字1つ目 
		//! @param [in] num2 比較する数字2つ目
		//! @return bool 等しいならばtrue 
		constexpr bool IsEqual(const double num1, const double num2)
		{
			const double dif = num1 - num2;
			if (dif > 0) { return (dif <= kDoubleAllowableError); }
			else { return (dif >= -kDoubleAllowableError); }
		}

		//! @brief 2乗した値を返す関数．
		//! @n 整数型や，小数型のみを想定して作っているので，他の型で使うとエラーが出るかも．
		//! @param [in] num 2乗する数．
		//! @return T 2乗した値． 
		template <typename T>
		constexpr T Squared(const T num) { return num * num; }

		//! @brief 3辺で三角形が作れるか調べる関数．
		//! @param [in] a 1辺目．
		//! @param [in] b 2辺目．
		//! @param [in] c 3辺目．
		//! @return bool 三角形が作れるならばtrue．
		template <typename T>
		constexpr bool CanMakeTriangle(const T a, const T b, const T c) { return (a + b > c && b + c > a && c + a > b); }
		
		//! @brief 目標値に値を近づける関数．
		//! @n 適当に作っている，線形でもない，描画用なので計算に使いたいなら作り直すこと
		//! @param [in] current 現在の値．
		//! @param [in] target 目標値．
		//! @param [in] rate 近づける割合．0 ～ 1の値を取る．
		//! @return T 近づけた値．
		template <typename T>
		T ApproachTarget(const T& current,const T& target, float rate) 
		{
			assert(0 <= rate);
			assert(rate <= 1);	//0 <= rate <= 1 でなければならない

			if (current == target) { return current; }

			return current * (1 - rate) + target * rate;
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
		constexpr double ConvertRadToDeg(const double rad) { return rad * 180.0 / kDoublePi; };

		//! @brief 角度をradからdegに変換する関数．
		//! @param [in] rad 角度[rad]．
		//! @return float 角度[deg]．
		constexpr float ConvertRadToDeg(const float rad) { return rad * 180.0f / kFloatPi; };

		//! @brief 角度をdegからradに変換する関数．
		//! @param [in] deg 角度[deg]．
		//! @return double 角度[rad]．
		constexpr double ConvertDegToRad(const double deg) { return deg * kDoublePi / 180.0; }

		//! @brief 角度をdegからradに変換する関数．
		//! @param [in] deg 角度[deg]．
		//! @return float 角度[rad]．
		constexpr float ConvertDegToRad(const float deg) { return deg * kFloatPi / 180.0f; }


		[[deprecated]] 
		float limitRangeAngle(const float angle);


		constexpr int kDigit = 3;	//!< 小数点以下の桁数．
		constexpr int kWidth = 10;	//!< 文字列の幅．

		//! @brief 小数を文字列に変換する関数．
		//! @n C++ では C のフォーマットのように %3.3f とかで小数を文字列に変換できないため自作する
		//! @param [in] num 変換する小数．
		//! @param [in] digit 小数点以下の桁数．
		//! @param [in] width 文字列の幅．
		//! @return std::string 変換した文字列．
		std::string ConvertFloatToString(const float num, const int digit = kDigit, const int width = kWidth);

		//! @brief 小数を文字列に変換する関数．
		//! @n C++ では C のフォーマットのように %3.3f とかで小数を文字列に変換できないため自作する
		//! @param [in] num 変換する小数．
		//! @param [in] digit 小数点以下の桁数．
		//! @param [in] width 文字列の幅．
		//! @return std::string 変換した文字列．
		std::string ConvertDoubleToString(const double num, const int digit = kDigit, const int width = kWidth);

	}	// namespace math_util

}	// namespace designlab


#endif	// DESIGNLAB_MATH_UTIL_H_