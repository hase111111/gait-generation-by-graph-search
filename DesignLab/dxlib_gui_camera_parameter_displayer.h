
//! @file      dxlib_gui_camera_parameter_displayer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_
#define DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "abstract_dxlib_gui.h"
#include "dxlib_camera.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiCameraParameterDisplayer
//! @brief カメラの情報を表示するGUIの処理・描画を行うクラス．
class DxlibGuiCameraParameterDisplayer final : public AbstractDxlibGui
{
public:
    //!< デフォルトコンストラクタは生成できない．
    DxlibGuiCameraParameterDisplayer() = delete;

    //! @brief コンストラクタでWindowのサイズと，
    //! @param[in] window_x ウィンドウの横幅．
    //! @param[in] window_y ウィンドウの縦幅．
    //! @param[in] camera カメラの管理を行うクラス．
    DxlibGuiCameraParameterDisplayer(
      int window_x,
      int window_y,
      const std::shared_ptr<DxlibCamera> camera_ptr);

    void Update() override;

    void Draw() const override;

private:
    void DrawCameraParameter() const;

    bool IsInWindow() const;

    const int window_x_;  //!< ウィンドウのX座標．
    const int window_y_;  //!< ウィンドウのY座標．

    const std::shared_ptr<const DxlibCamera> camera_ptr_;  //!< カメラのポインタ．

    const int kFontSize{ 16 };  //!< フォントのサイズ．
    const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };  //!< フォントへのパス．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_
