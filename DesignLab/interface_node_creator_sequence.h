
//! @file interface_node_creator_sequence.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <map>
#include <memory>

#include "divided_map_state.h"
#include "hexapod_next_move.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_node_creator.h"

namespace gaitgen {

//! @class INodeCreatorSequence
//! @brief INodeCreator を生成するためのビルダークラス.
class INodeCreatorSequence {
 public:
  INodeCreatorSequence() = default;
  virtual ~INodeCreatorSequence() = default;

  //! @brief INodeCreatorを生成する.
  //! @param[in] map 分割されたマップ.
  //! @param[out] node_creator INodeCreator を格納する map.
  //! @n key : HexapodMove, value : INodeCreator.
  //! つまり, ロボットの動作に対応する INodeCreator を格納する必要がある.
  virtual std::map<HexapodMove, std::unique_ptr<INodeCreator>> Build(
      const DividedMapState& map) const = 0;
};

}  // namespace gaitgen
