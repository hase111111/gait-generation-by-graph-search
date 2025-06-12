
//! @file xr_r1_const.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_XR_R1_CONST_H_
#define DESIGNLAB_XR_R1_CONST_H_

#include <array>

#include "cassert_define.h"
#include "math_util.h"


namespace designlab {

//! @class XR_R1Const
//! @brief XR-R1 のパラメータを定数で表現したもの.
//! @details
//! 座標系はロボット前方にx軸,左方向にy軸,上方向にz軸をとる右手座標系である.
class XrR1Const final {
private:
    constexpr static  int kPhantomXLegNum = 6;

public:
    // コンストラクタとコピーコンストラクタを削除し,実体を生成できないようにする.
    XrR1Const() = delete;
    XrR1Const(const XrR1Const& other) = delete;
    XrR1Const& operator=(const XrR1Const& other) = delete;
    XrR1Const(XrR1Const&& other) = delete;

    //! 第1関節の初期角度[rad]
    constexpr static std::array<float, kPhantomXLegNum> kCoxaDefaultAngle = {
      math_util::ConvertDegToRad(-45.0f),
      math_util::ConvertDegToRad(-90.0f),
      math_util::ConvertDegToRad(-135.0f),
      math_util::ConvertDegToRad(135.0f),
      math_util::ConvertDegToRad(90.0f),
      math_util::ConvertDegToRad(45.0f)
    };

    //!< 第1関節の可動範囲の最小値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kCoxaAngleMin = math_util::ConvertDegToRad(-90.0f);

    //!< 第1関節の可動範囲の最大値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kCoxaAngleMax = math_util::ConvertDegToRad(90.0f);


    //!< 第2関節の可動範囲の最小値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kFemurAngleMin = math_util::ConvertDegToRad(-90.0f);

    //!< 第2関節の可動範囲の最大値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kFemurAngleMax = math_util::ConvertDegToRad(90.0f);


    //!< 第2関節の可動範囲の最小値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kTibiaAngleMin = math_util::ConvertDegToRad(-170.0f);

    //!< 第2関節の可動範囲の最大値[rad].詳しくは referenceフォルダ参照.
    constexpr static float kTibiaAngleMax = math_util::ConvertDegToRad(0.0f);


    constexpr static float kCoxaLength = 45.0f;     //!< 第1関節部の長さ[mm].
    constexpr static float kFemurLength = 75.0f;    //!< 第2関節部の長さ[mm].
    constexpr static float kTibiaLength = 140.0f;   //!< 第3関節部の長さ[mm].

    //!< coxa linkの付け根(前方・後方)までの長さ[mm].
    constexpr static float kCoxaBaseOffsetY = 59.f;

    //!< coxa linkの付け根(中央)までの長さ[mm].
    constexpr static float kCenterCoxaBaseOffsetY = 91.5f;

    //!< coxa linkの付け根(前方・後方)までの長さ[mm].
    constexpr static float kCoxaBaseOffsetX = 120.f;

    //!< coxa linkの付け根までの長さ(上方向)[mm].
    constexpr static float kCoxaBaseOffsetZ = 0.f;

    //!< 胴体の高さ[mm].
    constexpr static float kBodyHeight = 45.0f;


    //! @brief 第1関節の角度が有効な範囲内かどうかを判定する.
    //! @param[in] leg_index 脚の番号.
    //! @param[in] angle 判定する角度.
    //! @return 有効な範囲内なら true.
    constexpr static bool IsValidCoxaAngle(const int leg_index, const float angle) noexcept {
        // 0 <= leg_index < kPhantomXLegNum であることを保証する.
        assert(0 <= leg_index);
        assert(leg_index < kPhantomXLegNum);

        return (kCoxaAngleMin + kCoxaDefaultAngle[leg_index] <= angle &&
          angle <= kCoxaAngleMax + kCoxaDefaultAngle[leg_index]);
    };

    //! @brief 第2関節の角度が有効な範囲内かどうかを判定する.
    //! @param[in] angle 判定する角度.
    //! @return 有効な範囲内なら true.
    constexpr static bool IsValidFemurAngle(const float angle) noexcept {
        return kFemurAngleMin <= angle && angle <= kFemurAngleMax;
    };

    //! @brief 第3関節の角度が有効な範囲内かどうかを判定する.
    //! @param[in] angle 判定する角度.
    //! @return 有効な範囲内なら true.
    constexpr static bool IsValidTibiaAngle(const float angle) noexcept {
        return kTibiaAngleMin <= angle && angle <= kTibiaAngleMax;
    };


    static_assert(kCoxaAngleMin < kCoxaAngleMax,
                  "kCoxaAngleMax > Min is required.");
    static_assert(kFemurAngleMin < kFemurAngleMax,
                  "kFemurAngleMax > Min is required.");
    static_assert(kTibiaAngleMin < kTibiaAngleMax,
                  "kTibiaAngleMax > Min is required.");

    static_assert(kCoxaLength > 0.f,
                  "kCoxaLength and Coxa Link must be positive.");
    static_assert(kFemurLength > 0.f,
                  "kFemurLength and Femur Link must be positive.");
    static_assert(kTibiaLength > 0.f,
                  "kTibiaLength and Tibia Link must be positive.");
};

}  // namespace designlab


#endif  // DESIGNLAB_XR_R1_CONST_H_
