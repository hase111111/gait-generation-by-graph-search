//! @file node_creator_com_move_straight.h
//! @brief 直線方向に重心の平行移動を行うクラス．

#ifndef DESIGNLAB_NODE_CREATOR_COM_MOVE_STRAIGHT_H_
#define DESIGNLAB_NODE_CREATOR_COM_MOVE_STRAIGHT_H_

#include "interface_node_creator.h"

#include <array>
#include <memory>

#include "designlab_polygon2.h"
#include "devide_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_vaild_checker.h"


namespace designlab
{

//! @class NodeCreatorComMoveStraight
//! @brief 直線方向に重心の平行移動を行うクラス．
class NodeCreatorComMoveStraight final : public INodeCreator
{
public:

	NodeCreatorComMoveStraight(
		const DevideMapState& devide_map,
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		enums::HexapodMove next_move
	);

	~NodeCreatorComMoveStraight() = default;

	void Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const override;


private:

	static constexpr int kCandidateDirectionNum = 8;	//!< 候補となる移動方向の数．
	static constexpr float kMaxMoveDistance = 60.f;		//!< 重心の移動距離の最大値．
	static constexpr float kMoveDistanceStep = 10.f;	//!< 重心の移動距離の刻み幅．

	const DevideMapState map_;							//!< 地面の状態を格納したクラス．
	const enums::HexapodMove next_move_;	//!< 次の動作．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;

	std::array<Vector3, kCandidateDirectionNum> candidate_directions_;	//!< 候補となる移動方向．
};

} // namespace designlab


#endif // DESIGNLAB_NODE_CREATOR_COM_MOVE_STRAIGHT_H_