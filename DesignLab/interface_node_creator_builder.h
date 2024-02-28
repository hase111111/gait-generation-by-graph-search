
//! @file      interface_node_creator_builder.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_NODE_CREATOR_BUILDER_H_
#define DESIGNLAB_INTERFACE_NODE_CREATOR_BUILDER_H_

#include <map>
#include <memory>

#include "divided_map_state.h"
#include "hexapod_next_move.h"
#include "interface_node_creator.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_posture_validator.h"


namespace designlab
{

//! @class INodeCreatorBuilder
//! @brief INodeCreatorを生成するためのビルダークラス．
class INodeCreatorBuilder
{
public:
    INodeCreatorBuilder() = default;
    virtual ~INodeCreatorBuilder() = default;

    //! @brief INodeCreatorを生成する．
    //! @param[in] map 分割されたマップ．
    //! @param[out] node_creator INodeCreatorを格納するmap．
    //! @n key : HexapodMove, value : INodeCreator．
    //! つまり，ロボットの動作に対応する INodeCreator を格納する必要がある．
    virtual void Build(
      const DividedMapState& map,
      std::map<enums::HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_NODE_CREATOR_BUILDER_H_
