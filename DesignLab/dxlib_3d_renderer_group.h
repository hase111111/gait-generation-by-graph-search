//! @file dxlib_3d_render_group.h
//! @brief dxlib_rendererのdraw関数を呼ぶためのクラス．

#ifndef DESIGN_DXLIB_3D_RENDER_GROUP_H_
#define DESIGN_DXLIB_3D_RENDER_GROUP_H_

#include <memory>
#include <vector>

#include "interface_dxlib_3d_renderer.h"


//! @class Dxlib3dRendererGroup
//! @brief dxlib_rendererのdraw関数を呼ぶためのクラス．
class Dxlib3dRendererGroup final
{
public:

	//! @breif 3D描画を行うrendererを登録する．
	void Register(const std::shared_ptr<IDxlib3dRenderer>& renderer);

	//! @brief 登録されているrendererのdraw関数を呼ぶ．
	//! @n また，Zソートを行う．
	void Draw() const;

private:
	std::vector<std::shared_ptr<const IDxlib3dRenderer> > renderers_;
};

#endif //DESIGN_DXLIB_3D_RENDER_GROUP_H_