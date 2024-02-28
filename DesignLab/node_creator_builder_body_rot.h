
//! @file      node_creator_builder_body_rot.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_BODY_ROT_H
#define DESIGNLAB_NODE_CREATOR_BUILDER_BODY_ROT_H

#include <map>
#include <memory>

#include "interface_node_creator_builder.h"


namespace designlab
{

//! @class NodeCreatorBuilderBodyRot 
//! @brief ノード作成ビルダーのボディ回転クラス
class NodeCreatorBuilderBodyRot final : public INodeCreatorBuilder
{
public:
    NodeCreatorBuilderBodyRot(
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

#endif  // DESIGNLAB_NODE_CREATOR_BUILDER_BODY_ROT_H
