
//! @file      math_util.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_UTIL_H_
#define DESIGNLAB_MATH_UTIL_H_

#include <concepts>
#include <numbers>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <iomanip>

#include "cassert_define.h"
#include "math_const.h"



//! @namespace designlab::math_util
//! @brief 基本的な計算を行う関数をまとめた名前空間．
//! @details
//! この名前空間にある関数は，基本的に constexprである．
//! そのため，コンパイル時に計算が行われる．
namespace designlab::math_util
{

template <typename T> concept Arithmetic = std::is_arithmetic_v<T>;

//! @brief C++において，小数同士の計算は誤差が出てしまう．
//! 誤差込みで値が等しいか調べる．
//! @tparam T float か double のみを想定している．
//! その他の型を使用する場合エラーが出る．
//! @param[in] num1 比較する数字1つ目．
//! @param[in] num2 比較する数字2つ目．
//! @retval true 等しい．または，誤差の範囲で等しい．
//! @retval false 等しくない．誤差の範囲外．
template <::std::floating_point T>
constexpr bool IsEqual(const T num1, const T num2) noexcept
{
    const T dif = num1 - num2;

    return (dif <= MathConst<T>::kAllowableError && dif >= -MathConst<T>::kAllowableError);
}


//! @brief 2乗した値を返す関数．
//! @tparam T float か double のみを想定している．
//! その他の型を使用する場合エラーが出る．
//! @param[in] num 2乗する数．
//! @return T 2乗した値．
template <Arithmetic T>
constexpr T Squared(const T num) noexcept { return num * num; }


//! @brief 3辺で三角形が作れるか調べる関数．
//! @param[in] a 1辺目．
//! @param[in] b 2辺目．
//! @param[in] c 3辺目．
//! @return 三角形が作れるならば true．
template <Arithmetic T>
constexpr bool CanMakeTriangle(const T a, const T b, const T c) noexcept
{
    assert(a >= 0);
    assert(b >= 0);
    assert(c >= 0);
    return (a + b > c && b + c > a && c + a > b);
}


//! @brief 目標値に値を近づける関数．
//! 描画用なので，線形でなく，適当に値を近づける．
//! そのため，計算に使いたいなら作り直すこと．
//! @param[in] current 現在の値．
//! @param[in] target 目標値．
//! @param[in] rate 近づける割合．0 ～ 1の値を取る．
//! @return 近づけた値．
template <typename T>
T ApproachTarget(const T& current, const T& target, float rate)
{
    assert(0 <= rate);
    assert(rate <= 1);  // 0 <= rate <= 1 でなければならない．

    if (current == target) { return current; }

    return static_cast<T>(current * (1 - rate) + target * rate);
}


//! @brief 指定した範囲内の乱数を生成する．
//! @param[in] min 乱数の最小値．
//! @param[in] max 乱数の最大値．
//! @return 生成した乱数．
//! @tparam T 算術型．int, float, doubleなど．
template <Arithmetic T>
T GenerateRandomNumber(T min, T max)
{
    assert(min < max);  // min < max でなければならない．

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return (T)dis(gen);
}


//! @brief 角度を [rad]から [deg] に変換する関数．
//! @param[in] rad 単位 [rad]．
//! @return 単位 [deg]．
template <::std::floating_point T>
constexpr T ConvertRadToDeg(const T rad) noexcept
{
    return rad * (MathConst<T>::kRoundAngle / static_cast<T>(2)) / std::numbers::pi_v<float>;
}

//! @brief 角度を [deg] から [rad] に変換する関数．
//! @param[in] deg 角度[deg]．
//! @return 角度[rad]．
template <::std::floating_point T>
constexpr T ConvertDegToRad(const T deg) noexcept
{
    return deg * std::numbers::pi_v<float> / (MathConst<T>::kRoundAngle / static_cast<T>(2));
}

//! @brief 角度を -180° ～ 180° の範囲に収める関数．
//! @param[in] angle 角度 [deg]．
//! @return 角度 [-180°, 180°)．
template <::std::floating_point T>
T LimitRangeAngleDeg(T angle)
{
    angle = fmod(angle + MathConst<T>::kRoundAngle / 2, MathConst<T>::kRoundAngle);

    if (angle < 0.0)
    {
        angle += MathConst<T>::kRoundAngle;
    }

    return angle - MathConst<T>::kRoundAngle / 2;
}


constexpr int kDigit = 3;   //!< 小数点以下の桁数．
constexpr int kWidth = 10;  //!< 文字列の幅．

//! @brief 小数を文字列に変換する関数．
//! @n C++ では C のフォーマットのように
//! %3.3f とかで小数を文字列に変換できないため自作する．
//! @param[in] num 変換する小数．
//! @param[in] digit 小数点以下の桁数．
//! @param[in] width 文字列の幅．
//! @return 変換した文字列．
//! @tparam T float か double のみを想定している．
//! その他の型を使用する場合エラーが出る．
template <::std::floating_point T>
std::string FloatingPointNumToString(
    const T num, const int digit = kDigit, const int width = kWidth)
{
    std::ostringstream ss;

    ss << std::fixed << std::setprecision(digit);
    ss << std::setw(width) << std::setfill(' ') << num;

    return ss.str();
}


}  // namespace designlab::math_util


#endif  // DESIGNLAB_MATH_UTIL_H_
