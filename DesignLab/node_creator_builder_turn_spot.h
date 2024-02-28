
//! @file      node_creator_builder_turn_spot.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_TURN_SPOT_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_TURN_SPOT_H_

#include <map>
#include <memory>

#include "interface_node_creator_builder.h"


namespace designlab
{

//! @class NodeCreatorBuilderTurnSpot
//! @brief 旋回動作用のノード生成クラスを生成するクラス．
class NodeCreatorBuilderTurnSpot final : public INodeCreatorBuilder
{
public:
    NodeCreatorBuilderTurnSpot(
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


#endif  // DESIGNLAB_NODE_CREATOR_BUILDER_TURN_SPOT_H_
