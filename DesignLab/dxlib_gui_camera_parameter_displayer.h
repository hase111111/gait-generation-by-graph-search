
//! @file      dxlib_gui_camera_parameter_displayer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_
#define DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "dxlib_camera.h"
#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiCameraParameterDisplayer
//! @brief カメラの情報を表示するGUIの処理・描画を行うクラス．
class DxlibGuiCameraParameterDisplayer final :
    public IDxlibGui,
    public IDxlibClickable,
    public IDxlibDraggable
{
public:
    DxlibGuiCameraParameterDisplayer() = delete;  //!< デフォルトコンストラクタは生成できない．

    //! @brief コンストラクタでWindowのサイズと，カメラの管理を行うクラスを受け取る．
    //! @param[in] window_x ウィンドウの横幅．
    //! @param[in] window_y ウィンドウの縦幅．
    //! @param[in] camera カメラの管理を行うクラス．
    DxlibGuiCameraParameterDisplayer(
      int window_x,
      int window_y,
      const std::shared_ptr<DxlibCamera> camera_ptr);

    //! @brief GUIの位置を設定する．
    //! @n Dxlibの画面の座標は左上を原点とし，右下に行くほど値が大きくなる．
    //! @n 横方向にx軸，縦方向にy軸をとる．
    //! @param[in] pos_x GUIのx座標．
    //! @param[in] pos_y GUIのy座標．
    //! @param[in] option GUIのどの地点を起点に座標を設定するかを指定する．
    //! defaultでは左上を起点とする．
    //! @param[in] this_is_first_time この呼び出しが初めてかどうかを指定する．defaultでは false．
    //! trueを指定すると，GUIの位置を設定するだけでなく，GUIの初期位置を更新する．
    void SetPos(int pos_x, int pos_y,
                unsigned int option = kDxlibGuiAnchorLeftTop, bool this_is_first_time = false);

    void Update() override;

    void Draw() const override;

    void SetVisible(bool visible) override;

    bool IsVisible() const override
    {
        return visible_;
    }

    void ClickedAction(int cursor_x, int cursor_y,
                       int left_pushing_count,
                       int middle_pushing_count, int right_pushing_count) override;

    bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

    bool IsDraggable(int cursor_x, int cursor_y) const override;

    bool IsDragged() const override
    {
        return is_dragging_;
    };

    void SetDragged(const bool is_dragged) override
    {
        is_dragging_ = is_dragged;
    };

    void DraggedAction(int cursor_dif_x, int cursor_dif_y, unsigned int mouse_key_bit) override;

private:
    static constexpr int kWidth{ 470 };  //!< GUIの幅．
    static constexpr int kHeight{ 340 };  //!< GUIの高さ．
    static constexpr int kTitleBarHeight{ 32 };  //!< タイトルバーの高さ．

    void DrawBackground() const;

    void DrawCameraParameter() const;

    bool IsInWindow() const;

    int gui_left_pos_x_{ 0 };  //!< GUIの左上のX座標．
    int gui_top_pos_y_{ 0 };  //!< GUIの左上のY座標．

    int set_pos_x_{ 0 };  //!< GUIの左上のX座標(初期位置)
    int set_pos_y_{ 0 };  //!< GUIの左上のY座標(初期位置)

    const int window_x_;  //!< ウィンドウのX座標．
    const int window_y_;  //!< ウィンドウのY座標．

    bool is_dragging_{ false };  //!< ドラッグ中かどうか．
    bool visible_{ true };  //!< ボタンの表示を行うかどうか．

    const std::shared_ptr<const DxlibCamera> camera_ptr_;  //!< カメラのポインタ．

    std::vector<std::unique_ptr<SimpleButton> > button_;  //!< ボタン．

    const int kFontSize{ 16 };  //!< フォントのサイズ．
    const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };  //!< フォントへのパス．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_CAMERA_PARAMETER_DISPLAYER_H_
