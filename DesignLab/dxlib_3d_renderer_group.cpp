
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "dxlib_3d_renderer_group.h"

#include "cassert_define.h"
#include "dxlib_util.h"


namespace designlab
{

void Dxlib3dRendererGroup::Register(const std::shared_ptr<IDxlib3dRenderer>& renderer)
{
    assert(renderer != nullptr);

    renderers_.push_back(renderer);
}

void Dxlib3dRendererGroup::Draw() const
{
    dxlib_util::SetZBufferEnable();

    for (const auto& renderer : renderers_)
    {
        renderer->Draw();
    }
}

}  // namespace designlab
