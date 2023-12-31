
//! @file      interface_hexapod_state_presenter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_HEXAPOD_STATE_PRESENTER_H_
#define DESIGNLAB_INTERFACE_HEXAPOD_STATE_PRESENTER_H_

#include "math_vector3.h"


namespace designlab
{

//! @class IHexapodStatePresenter
//! @brief 6脚ロボットの状態を表示するクラス．
class IHexapodStatePresenter
{
public:
    virtual ~IHexapodStatePresenter() = default;

    //! @brief 遊脚する位置を返す，脚座標系．
    //! @param[in] leg_index 脚番号．
    //! @return 遊脚する位置．脚座標系．
    [[nodiscard]]
    virtual Vector3 GetFreeLegPosLegCoordinate(int leg_index) const noexcept = 0;

    //! @brief 脚の付け根の座標(leg base position )を取得する．ロボット座標系．
    //! @param[in] leg_index 脚番号．
    //! @return 脚の付け根の座標．ロボット座標系．
    [[nodiscard]]
    virtual Vector3 GetLegBasePosRobotCoordinate(int leg_index) const noexcept = 0;

    //! @brief 地面の最大高さと重心位置を最小どれだけ離すかを返す．
    //! @return 地面の最大高さと重心位置との最小距離 [mm]．
    [[nodiscard]]
    virtual float GetGroundHeightMarginMin() const noexcept = 0;

    //! @brief 地面の最大高さと重心位置を最大どれだけ離すかを返す．
    //! @return 地面の最大高さと重心位置との最大距離 [mm]．
    [[nodiscard]]
    virtual float GetGroundHeightMarginMax() const noexcept = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_HEXAPOD_STATE_PRESENTER_H_
