//! @file node_creator_com_move.h
//! @brief 重心の平行移動を行うクラス．波東さんの手法．

#ifndef DESIGNLAB_NODE_CREATOR_COM_MOVE_H_
#define DESIGNLAB_NODE_CREATOR_COM_MOVE_H_


#include "interface_node_creator.h"

#include <memory>

#include "com_candidate_polygon_maker.h"
#include "com_selecter_hato.h"
#include "designlab_polygon2.h"
#include "devide_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_vaild_checker.h"


//! @class NodeCreatorComMove
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
class NodeCreatorComMove final : public INodeCreator
{
public:

	NodeCreatorComMove(
		const DevideMapState& devide_map, 
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		HexapodMove next_move
	);
	~NodeCreatorComMove() = default;

	void Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const override;


private:

	const ComCandidatePolygonMaker maker_;	//!< 候補地点を含む多角形を作成するクラス
	const ComSelecterHato selecter_;		//!< 多角形から最適な地面を選択するクラス

	const DevideMapState map_;		//!< 地面の状態を格納したクラス
	const HexapodMove next_move_;	//!< 次の動作

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif // DESIGNLAB_NODE_CREATOR_COM_MOVE_H_