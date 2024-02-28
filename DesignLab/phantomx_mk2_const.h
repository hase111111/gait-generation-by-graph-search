
//! @file      phantomx_mk2_const.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_PHANTOMX_MK2_CONST_H_
#define DESIGNLAB_PHANTOMX_MK2_CONST_H_

#include <array>

#include "cassert_define.h"
#include "math_util.h"


namespace designlab
{

//! @class PhantomXMkIIConst
//! @brief PhantomX mk-Ⅱ のパラメータを定数で表現したもの．
//! @details
//! コンストラクタを削除したので，実体は生成できない．呼び出す際は，
//! @code
//! float coxa_angle_min = PhantomXMkIIConst::kCoxaAngleMin;
//! @endcode
//! として呼び出す．
//! 簡単のため値をここにまとめたが，むやみにここの値を参照せずに PhantomXMkII を使うこと．
//! @n 座標系はロボット前方にx軸，左方向にy軸，上方向にz軸をとる右手座標系である．
class PhantomXMkIIConst final
{
private:
    constexpr static  int kPhantomXLegNum = 6;

public:
    // コンストラクタとコピーコンストラクタを削除し，実体を生成できないようにする．
    PhantomXMkIIConst() = delete;
    PhantomXMkIIConst(const PhantomXMkIIConst& other) = delete;
    PhantomXMkIIConst& operator=(const PhantomXMkIIConst& other) = delete;
    PhantomXMkIIConst(PhantomXMkIIConst&& other) = delete;

    //! 第1関節の初期角度[rad]
    constexpr static std::array<float, kPhantomXLegNum> kCoxaDefaultAngle = {
      math_util::ConvertDegToRad(-45.0f),
      math_util::ConvertDegToRad(-90.0f),
      math_util::ConvertDegToRad(-135.0f),
      math_util::ConvertDegToRad(135.0f),
      math_util::ConvertDegToRad(90.0f),
      math_util::ConvertDegToRad(45.0f)
    };

    //!< 第1関節の可動範囲の最小値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kCoxaAngleMin = math_util::ConvertDegToRad(-81.0f);

    //!< 第1関節の可動範囲の最大値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kCoxaAngleMax = math_util::ConvertDegToRad(81.0f);


    //!< 第2関節の可動範囲の最小値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kFemurAngleMin = math_util::ConvertDegToRad(-105.0f);

    //!< 第2関節の可動範囲の最大値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kFemurAngleMax = math_util::ConvertDegToRad(99.0f);


    //!< 第2関節の可動範囲の最小値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kTibiaAngleMin = math_util::ConvertDegToRad(-145.0f);

    //!< 第2関節の可動範囲の最大値[rad]．詳しくは referenceフォルダ参照．
    constexpr static float kTibiaAngleMax = math_util::ConvertDegToRad(25.5f);


    constexpr static float kCoxaLength = 52.0f;     //!< 第1関節部の長さ[mm]．
    constexpr static float kFemurLength = 66.0f;    //!< 第2関節部の長さ[mm]．
    constexpr static float kTibiaLength = 130.0f;   //!< 第3関節部の長さ[mm]．

    //!< coxa linkの付け根(前方・後方)までの長さ[mm]．
    constexpr static float kCoxaBaseOffsetY = 61.f;

    //!< coxa linkの付け根(中央)までの長さ[mm]．
    constexpr static float kCenterCoxaBaseOffsetY = 103.4f;

    //!< coxa linkの付け根(前方・後方)までの長さ[mm]．
    constexpr static float kCoxaBaseOffsetX = 122.f;

    //!< coxa linkの付け根までの長さ(上方向)[mm]．
    constexpr static float kCoxaBaseOffsetZ = 1.116f;

    //!< 胴体の高さ[mm]．
    constexpr static float kBodyHeight = 40.0f;


    //! @brief 第1関節の角度が有効な範囲内かどうかを判定する．
    //! @param[in] leg_index 脚の番号．
    //! @param[in] angle 判定する角度．
    //! @return 有効な範囲内なら true．
    constexpr static bool IsValidCoxaAngle(const int leg_index, const float angle)
    {
        // 0 <= leg_index < kPhantomXLegNum であることを保証する．
        assert(0 <= leg_index);
        assert(leg_index < kPhantomXLegNum);

        return (kCoxaAngleMin + kCoxaDefaultAngle[leg_index] <= angle &&
          angle <= kCoxaAngleMax + kCoxaDefaultAngle[leg_index]);
    };

    //! @brief 第2関節の角度が有効な範囲内かどうかを判定する．
    //! @param[in] angle 判定する角度．
    //! @return 有効な範囲内なら true．
    constexpr static bool IsValidFemurAngle(const float angle)
    {
        return kFemurAngleMin <= angle && angle <= kFemurAngleMax;
    };

    //! @brief 第3関節の角度が有効な範囲内かどうかを判定する．
    //! @param[in] angle 判定する角度．
    //! @return 有効な範囲内なら true．
    constexpr static bool IsValidTibiaAngle(const float angle)
    {
        return kTibiaAngleMin <= angle && angle <= kTibiaAngleMax;
    };


    static_assert(kCoxaAngleMin < kCoxaAngleMax, "kCoxaAngleMax > Minである必要があります．");
    static_assert(kFemurAngleMin < kFemurAngleMax, "kFemurAngleMax > Minである必要があります．");
    static_assert(kTibiaAngleMin < kTibiaAngleMax, "kTibiaAngleMax > Minである必要があります．");
    static_assert(kCoxaLength > 0.f, "kCoxaLength，Coxa Linkの長さは正である必要があります．");
    static_assert(kFemurLength > 0.f, "kFemurLength，Femur Linkの長さは正である必要があります．");
    static_assert(kTibiaLength > 0.f, "kTibiaLength，Tibia Linkの長さは正である必要があります．");
};

}  // namespace designlab


#endif  // DESIGNLAB_PHANTOMX_MK2_CONST_H_
