
//! @file      dxlib_gui_display_node_switcher.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_DISPLAY_NODE_SWITCHER_H_
#define DESIGNLAB_DXLIB_GUI_DISPLAY_NODE_SWITCHER_H_

#include <memory>
#include <string>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiDisplayNodeSwitcher
//! @brief ノードの表示・切り替えを行うGUIのクラス．
class DxlibGuiDisplayNodeSwitcher final :
    public IDxlibGui,
    public IDxlibClickable,
    public IDxlibDraggable
{
public:
    DxlibGuiDisplayNodeSwitcher(int window_x, int window_y);


    void SetPos(int pos_x, int pos_y,
                unsigned int option = kDxlibGuiAnchorLeftTop, bool this_is_first_time = false);

    //! @brief GUIに表示するノードの情報を設定する．
    //! @param[in] node_num 全ノード数．
    //! @param[in] simulation_end_index シミュレーションの終了ノード番号．
    void SetGraphData(size_t node_num, const std::vector<size_t>& simulation_end_index);

    //! @brief 現在表示するノードの番号を取得する．
    //! @return 現在表示するノードの番号．
    size_t GetDisplayNodeNum() const;

    //! @brief 現在表示するシミュレーションの番号を取得する．
    //! @return 現在表示するシミュレーションの番号．
    constexpr int GetSimulationNum() const { return simulation_num_; }


    void Update() override;

    void Draw() const override;

    void SetVisible(bool visible) override;

    bool IsVisible() const override
    {
        return visible_;
    }

    void ClickedAction(int cursor_x, int cursor_y,
                       int left_pushing_count, int middle_pushing_count,
                       int right_pushing_count) override;

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


    void InAnimation(const bool in_animation) { in_animation_ = in_animation; }
    int GetAnimeSpeed() const { return animation_speed_; }

private:
    static constexpr int kWidth{ 275 };  //!< GUIの幅．
    static constexpr int kHeight{ 282 };  //!< GUIの高さ．
    static constexpr int kTitleBarHeight{ 32 };  //!< タイトルバーの高さ．

    //! シミュレーションの中で最初のノードに移動する関数．
    void MoveMostPrevNode();

    //! シミュレーションの中で前のノードに移動する関数．
    void MovePrevNode();

    //! シミュレーションの中で最後のノードに移動する関数．
    void MoveMostNextNode();

    //! シミュレーションの中で次のノードに移動する関数．
    void MoveNextNode();

    //! 前のシミュレーションに移動する関数．
    void MovePrevSimulation();

    //! 次のシミュレーションに移動する関数．
    void MoveNextSimulation();

    //! 全シミュレーション数を求める．
    int GetAllSimulationNum() const;

    void DrawBackground() const;

    bool IsInWindow() const;


    const int kAnimeSpeedMax{ 120 };  //!< アニメーション速度の最大値．
    const int kAnimeSpeedMin{ 1 };    //!< アニメーション速度の最小値．

    int gui_left_pos_x_{ 0 };   //!< GUIの左上のX座標．
    int gui_top_pos_y_{ 0 };    //!< GUIの左上のY座標．

    int set_pos_x_{ 0 };  //!< SetされたGUIの左上のX座標．
    int set_pos_y_{ 0 };  //!< SetされたGUIの左上のY座標．

    const int window_x_;  //!< ウィンドウのX座標．
    const int window_y_;  //!< ウィンドウのY座標．

    std::vector<std::unique_ptr<SimpleButton>> button_;  //!< ボタン．

    size_t all_node_num_;  //!< 全ノード数．
    std::vector<size_t> simulation_end_index_;  //!< シミュレーションの終了ノード番号．
    size_t display_node_num_;  //!< 表示するノードの番号．
    int simulation_num_;       //!< 表示するシミュレーションの番号．
    bool do_auto_animation_;   //!< 自動再生中かどうか．
    int animation_speed_;      //!< 再生速度．
    int counter_{ 0 };         //!< カウンター．

    bool is_dragging_{ false };  //!< ドラッグ中か．
    bool visible_{ true };       //!< GUIを表示するか．

    const int kFontSize{ 16 };  //!< フォントのサイズ
    const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };  //!< フォントへのパス
    const int font_handle_;  //!< フォントのハンドル

    bool in_animation_{ false };  //!< アニメーション中か．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_DISPLAY_NODE_SWITCHER_H_
