
//! @file      node_creator_com_up_down.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_COM_UP_DOWN_H_
#define DESIGNLAB_NODE_CREATOR_COM_UP_DOWN_H_

#include <memory>
#include <vector>

#include "divided_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_node_creator.h"


namespace designlab
{

//! @class NodeCreatorComUpDown
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
class NodeCreatorComUpDown final : public INodeCreator
{
public:
    NodeCreatorComUpDown(
      const DividedMapState& devide_map,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      enums::HexapodMove next_move);

    ~NodeCreatorComUpDown() = default;

    void Create(const RobotStateNode& current_node, int current_num,
                std::vector<RobotStateNode>* output_graph) const override;

private:
    static constexpr int kDiscretization = 7;  //!< 離散化数．最大位置を最小位置を何分割するのか．

    // グローバル座標の重心の最低位置と最高位置から，重心を上下に変化させたノードを追加する．
    void pushNodeByMaxAndMinPosZ(const RobotStateNode& current_node, int current_num,
                                 float high, float low,
                                 std::vector<RobotStateNode>* output_graph) const;

    const DividedMapState map_;

    const enums::HexapodMove next_move_;  //!< 次の動作．

    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_NODE_CREATOR_COM_UP_DOWN_H_
