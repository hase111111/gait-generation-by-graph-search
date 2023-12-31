
//! @file      math_const.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MATH_CONST_H_
#define DESIGNLAB_MATH_CONST_H_


namespace designlab
{

// float と double以外は作成しない．
template<typename T>
struct MathConst
{
    // Tが float でも double でもない場合，エラーを出す．
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
                  "T は float と double 型にのみ提供されます．");
};


template<>
struct MathConst<float>
{
    static constexpr float kAllowableError = 0.001f;
    static constexpr float kRoundAngle = 360.0f;
};

template<>
struct MathConst<double>
{
    static constexpr double kAllowableError = 0.001;
    static constexpr double kRoundAngle = 360.0;
};


}  // namespace designlab

#endif  // DESIGNLAB_MATH_CONST_H_
