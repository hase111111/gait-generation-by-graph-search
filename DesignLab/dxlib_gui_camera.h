
//! @file      dxlib_gui_camera.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_CAMERA_H_
#define DESIGNLAB_DXLIB_GUI_CAMERA_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "abstract_dxlib_gui.h"
#include "dxlib_camera.h"
#include "math_vector3.h"
#include "interface_dxlib_node_setter.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiCamera
//! @brief カメラの操作・管理を行うGUIの処理・描画を行うクラス．
class DxlibGuiCamera final :
    public AbstractDxlibGui,
    public IDxlibNodeSetter
{
public:
    DxlibGuiCamera() = delete;  //!< デフォルトコンストラクタは生成できない．

    //! @brief コンストラクタでWindowのサイズと，
    //! カメラの管理を行うクラスを受け取る．
    //! @param[in] window_x ウィンドウの横幅．
    //! @param[in] window_y ウィンドウの縦幅．
    //! @param[in] camera カメラの管理を行うクラス．
    DxlibGuiCamera(
        int window_x, int window_y, const std::shared_ptr<DxlibCamera> camera);

    void SetNode(const RobotStateNode& node) override;

    void Update() override;

    void Draw() const override;

private:
    static constexpr int kWidth{ 245 };  //!< GUIの横幅．
    static constexpr int kHeight{ 410 };  //!< GUIの縦幅．

    //! @brief GUIの文字を描画する．
    void DrawString() const;

    bool IsInWindow() const;

    const int window_x_;  //!< ウィンドウのX座標．
    const int window_y_;  //!< ウィンドウのY座標．

    const std::shared_ptr<DxlibCamera> camera_;  //!< カメラの管理を行うクラス．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_CAMERA_H_
