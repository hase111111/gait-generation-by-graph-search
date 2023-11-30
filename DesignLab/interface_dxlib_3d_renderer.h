//! @file interface_dxlib_3d_renderer.h
//! @brief DxLibの描画処理を行うクラスのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_
#define DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_


//! @class IDxlib3dRenderer
//! @brief DxLibの描画処理を行うクラスのインターフェース．
class IDxlib3dRenderer
{
public:

	virtual ~IDxlib3dRenderer() = default;

	virtual void Draw() const = 0;
};


#endif // DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_