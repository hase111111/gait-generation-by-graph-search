
//! @file      dxlib_3d_renderer_group.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_3D_RENDERER_GROUP_H_
#define DESIGNLAB_DXLIB_3D_RENDERER_GROUP_H_

#include <memory>
#include <vector>

#include "interface_dxlib_3d_renderer.h"


namespace designlab
{

//! @class Dxlib3dRendererGroup
//! @brief dxlib_renderer の draw 関数を呼ぶためのクラス．
class Dxlib3dRendererGroup final
{
public:
    //! @brief 3D描画を行うクラスを登録する．
    void Register(const std::shared_ptr<IDxlib3dRenderer>& renderer);

    //! @brief 登録されているクラスの draw 関数を呼ぶ．
    //! @n また，Zソートを行う．
    void Draw() const;

private:
    std::vector<std::shared_ptr<const IDxlib3dRenderer> > renderers_;
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_3D_RENDERER_GROUP_H_
