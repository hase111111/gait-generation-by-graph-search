
//! @file gog_type.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

namespace designlab {

enum class GpgType {
  kFlatLegacyAlgorithm,  //!< 平坦な地面を歩く(旧アルゴリズム).
  kFlat,                 //!< 平坦な地面を歩く.
  kPitchRot,             //!< ピッチ回転を行う.
  kStraightMove,         //!< 直進移動を行う.
  kSwichByMap,           //!< マップに応じて歩容を切り替える.
  kWall,                 //!< 壁を歩く.
  kPruningBranch,        //!< 新しく実装した枝刈りアルゴリズム.
  kPruningBranchLegacy,  //!< 旧アルゴリズムで枝刈りを行う.
};

}  // namespace designlab
