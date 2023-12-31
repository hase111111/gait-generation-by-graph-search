
//! @file      interface_dxlib_3d_renderer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_
#define DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_


namespace designlab
{

//! @class IDxlib3dRenderer
//! @brief DxLibの描画処理を行うクラスのインターフェース．
class IDxlib3dRenderer
{
public:
    virtual ~IDxlib3dRenderer() = default;

    //! @brief 描画処理を行う．
    //! const 関数にしているのは，描画処理の中でメンバ変数を変更しないようにするため．
    virtual void Draw() const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_3D_RENDERER_H_
