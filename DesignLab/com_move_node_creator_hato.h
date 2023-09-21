//! @file com_move_node_creator_hato.h
//! @brief 重心の平行移動を行うクラス．波東さんの手法．

#ifndef DESIGNLAB_COM_MOVE_NODE_CREATOR_HATO_H_
#define DESIGNLAB_COM_MOVE_NODE_CREATOR_HATO_H_


#include "interface_node_creator.h"

#include <memory>

#include "com_type.h"
#include "com_candidate_polygon_maker.h"
#include "com_selecter_hato.h"
#include "designlab_polygon2.h"
#include "hexapod_state_calculator.h"
#include "map_state.h"


//! @class ComMoveNodeCreatorHato
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
class ComMoveNodeCreatorHato final : public INodeCreator
{
public:

	ComMoveNodeCreatorHato(const DevideMapState& devide_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, EHexapodMove next_move);
	~ComMoveNodeCreatorHato() = default;

	void Create(const SNode& current_node, int current_num, std::vector<SNode>* output_graph) override;


private:

	bool isStable(const SNode& node) const;

	bool isIntersectGround(const SNode& node) const;


	const float kStableMargin;	//!< 静的安全余裕 15mm程度が妥当らしい(波東さんのプログラムより，MAXで40mm程度)

	//std::vector<designlab::Polygon2> polygon_vec_;


	const DevideMapState map_;	//!< 地面の状態を格納したクラス

	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス

	const ComCandidatePolygonMaker maker_;	//!< 候補地点を含む多角形を作成するクラス

	ComSelecterHato selecter_;	//!< 多角形から最適な地面を選択するクラス

	const EHexapodMove next_move_;	//!< 次の移動方向
};


#endif //DESIGNLAB_COM_MOVE_NODE_CREATOR_HATO_H_