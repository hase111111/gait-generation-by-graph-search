
//! @file interface_dxlib_3d_renderer.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

namespace gaitgen {

//! @class IDxlib3dRenderer
//! @brief DxLibの描画処理を行うクラスのインターフェース.
class IDxlib3dRenderer {
 public:
  virtual ~IDxlib3dRenderer() = default;

  //! @brief 描画処理を行う.
  //! const 関数にしているのは,
  //! 描画処理の中でメンバ変数を変更しないようにするため.
  virtual void Draw() const = 0;
};

}  // namespace gaitgen
