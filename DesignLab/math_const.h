
//! @file      math_const.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_CONST_H_
#define DESIGNLAB_MATH_CONST_H_


namespace designlab
{

//! @class MathConst
//! @brief float 型と double 型の定数を提供するクラス．
//! @details
//! float と double以外は作成しない．
//! このクラスはテンプレートクラスであり，
//! テンプレート引数Tには float か double を指定する．
//! それ以外の型を指定した場合，コンパイルエラーを出す．
//! @n なお，円周率のような定数は，std::numbers::pi がC++20から提供される．
template<typename T>
struct MathConst
{
    // Tが float でも double でもない場合，エラーを出す．
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
                  "T is provided only for float and double types.");
};


//! @class MathConst<float>
//! @brief float型の定数を提供するクラス．
template<>
struct MathConst<float>
{
    //! @brief 許容誤差．
    static constexpr float kAllowableError = 0.001f;

    //! @brief 360度の角．
    static constexpr float kRoundAngle = 360.0f;
};

//! @class MathConst<double>
//! @brief double型の定数を提供するクラス．
template<>
struct MathConst<double>
{
    //! @brief 許容誤差．
    static constexpr double kAllowableError = 0.001;

    //! @brief 360度の角．
    static constexpr double kRoundAngle = 360.0;
};


}  // namespace designlab

#endif  // DESIGNLAB_MATH_CONST_H_
