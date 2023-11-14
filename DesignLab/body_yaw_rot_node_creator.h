//! @file body_yaw_rot_node_creator.h
//! @brief ロボットのヨー軸周りの回転を表すノードを生成するクラス．

#ifndef DESIGNLAB_BODY_YAW_ROT_NODE_CREATOR_H_
#define DESIGNLAB_BODY_YAW_ROT_NODE_CREATOR_H_

#include "interface_node_creator.h"

#include <memory>

#include "designlab_math_util.h"
#include "devide_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_vaild_checker.h"



class BodyYawRotNodeCreator final : public INodeCreator
{
public:

	BodyYawRotNodeCreator(
		const DevideMapState& devide_map,
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		HexapodMove next_move
	);
	~BodyYawRotNodeCreator() = default;

	void Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const override;


private:
	
	static constexpr float kBodyYawRotAngleMax = ::designlab::math_util::ConvertDegToRad(20.f);	//!< ロボットのヨー軸周りの回転角度．
	static constexpr float kBodyYawRotAngleMin = ::designlab::math_util::ConvertDegToRad(-20.f);	//!< ロボットのヨー軸周りの回転角度．
	static constexpr int kBodyYawRotAngleDivNum = 10;	//!< ロボットのヨー軸周りの回転角度の分割数．

	const DevideMapState map_;		//!< 地面の状態を格納したクラス．
	const HexapodMove next_move_;	//!< 次の動作．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;


	static_assert(kBodyYawRotAngleMax > kBodyYawRotAngleMin, "kBodyYawRotAngleMax は kBodyYawRotAngleMinよりも大きい必要があります.");
};


#endif	// DESIGNLAB_BODY_YAW_ROT_NODE_CREATOR_H_