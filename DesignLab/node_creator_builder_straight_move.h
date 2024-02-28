
//! @file      node_creator_builder_straight_move.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_STRAIGHT_MOVE_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_STRAIGHT_MOVE_H_

#include <map>
#include <memory>

#include "interface_node_creator_builder.h"


namespace designlab
{

//! @class NodeCreatorBuilderStraightMove
//! @brief 波東さんが行った処理と同様の方法になるようにノード生成クラスを生成するクラス．
class NodeCreatorBuilderStraightMove final : public INodeCreatorBuilder
{
public:
    NodeCreatorBuilderStraightMove(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

    void Build(
      const DividedMapState& map,
      std::map<enums::HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const override;

private:
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_NODE_CREATOR_BUILDER_STRAIGHT_MOVE_H_
