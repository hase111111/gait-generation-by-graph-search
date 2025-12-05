
//! @file dxlib_3d_renderer_group.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "dxlib_3d_renderer_group.h"

#include "cassert_define.h"
#include "dxlib_util.h"

namespace gaitgen {

void Dxlib3dRendererGroup::Register(
    const std::shared_ptr<IDxlib3dRenderer>& renderer) {
  assert(renderer != nullptr);

  renderers_.push_back(renderer);
}

void Dxlib3dRendererGroup::Draw() const {
  dxlib_util::SetZBufferEnable();

  for (const auto& renderer : renderers_) {
    renderer->Draw();
  }
}

}  // namespace gaitgen
