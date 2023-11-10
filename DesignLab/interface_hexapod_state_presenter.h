//! @file interface_hexapod_state_presenter.h
//! @brief 6脚ロボットの状態を表示するクラス


#ifndef INTERFACE_HEXAPOD_STATE_PRESENTER_H
#define INTERFACE_HEXAPOD_STATE_PRESENTER_H


#include "designlab_vector3.h"


//! @class IHexapodStatePresenter
//! @brief 6脚ロボットの状態を表示するクラス
class IHexapodStatePresenter
{
public:

	virtual ~IHexapodStatePresenter() = default;

	//! @brief 遊脚する位置を返す，脚座標系．
	//! @param [in] leg_index 脚番号．
	//! @return designlab::Vector3 遊脚する位置．脚座標系．
	[[nodiscard]] virtual designlab::Vector3 GetFreeLegPosLegCoodinate(int leg_index) const noexcept = 0;

	//! @brief 脚の付け根の座標( leg base position )を取得する．ロボット座標系．
	//! @param [in] leg_index 脚番号．
	//! @return designlab::Vector3 脚の付け根の座標．ロボット座標系．
	[[nodiscard]] virtual designlab::Vector3 GetLegBasePosRobotCoodinate(int leg_index) const noexcept = 0;

	//! @brief 地面の最大高さと重心位置を最小どれだけ離すかを返す
	[[nodiscard]] virtual float GetGroundHeightMarginMin() const noexcept = 0;

	//! @brief 地面の最大高さと重心位置を最大どれだけ離すかを返す
	[[nodiscard]] virtual float GetGroundHeightMarginMax() const noexcept = 0;

};

#endif	// INTERFACE_HEXAPOD_STATE_PRESENTER_H