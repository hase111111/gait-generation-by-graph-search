
//! @file      simple_button.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_SIMPLE_BUTTON_H_
#define DESIGNLAB_SIMPLE_BUTTON_H_

#include <functional>
#include <string>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_gui.h"


namespace designlab
{

//! @class SimpleButton
//! @brief Dxlibのウィンドウのボタンの処理・描画を管理するクラス．
class SimpleButton final : public IDxlibGui, public IDxlibClickable
{
public:
    SimpleButton() = delete;  //!< デフォルトコンストラクタは生成できない．

    //! @brief コンストラクタでボタンのテキスト，座標，サイズを受け取る．
    //! @param[in] text ボタンに書かれるテキスト．
    //! @param[in] x_pos ボタンのx座標．ボタンの中心座標．
    //! @param[in] y_pos ボタンのy座標．ボタンの中心座標．
    //! @param[in] x_size ボタンの横幅．
    //! @param[in] y_size ボタンの縦幅．
    //! @param[in] fit_size ボタンのサイズをテキストに合わせるかどうか．
    //! defaultでは false．
    SimpleButton(const std::string& text,
                 int x_pos, int y_pos, int x_size, int y_size,
                 bool fit_size = false);

    ~SimpleButton() = default;

    //! @brief ボタンがクリックされたときに実行される関数を設定する．
    //! @param[in] func ボタンがクリックされたときに実行される関数．
    inline void SetActivateFunction(const std::function<void()>& func)
    {
        click_function_ = func;
    }

    //! @brief ボタンの座標を設定する．
    //! @param[in] pos_x ボタンのx座標．
    //! @param[in] pos_y ボタンのy座標．
    //! @param[in] option ボタンのどの地点を起点に座標を設定するかを指定する．
    //! defaultでは左上を起点とする．
    void SetPos(int pos_x, int pos_y, unsigned int option = kDxlibGuiAnchorLeftTop);

    //! @brief ボタンのx座標を取得する．
    //! @return ボタンのx座標．ボタンの中心座標．
    [[nodiscard]] constexpr int GetPosMiddleX() const noexcept
    {
        return pos_middle_x;
    }

    //! @brief ボタンのy座標を取得する．
    //! @return ボタンのy座標．ボタンの中心座標．
    [[nodiscard]] constexpr int GetPosMiddleY() const noexcept
    {
        return pos_middle_y;
    }

    void Update() override;

    void Draw() const override;

    void SetVisible(bool visible) override
    {
        visible_ = visible;
    }

    bool IsVisible() const override
    {
        return visible_;
    }

    void ClickedAction(const DxlibMouseState& state) override;

    bool CursorOnGui(int cursor_x, int cursor_y) const noexcept override;

private:
    int GetFitButtonSizeX(int now_size_x) const noexcept;
    int GetFitButtonSizeY(int now_size_y) const noexcept;

    const int kFontSize{ 16 };  //!< フォントのサイズ
    const std::string kFontPath{ "font/Yu_Gothic_UI.dft" };  //!< フォントへのパス

    //! ボタンに書かれているテキスト，複数行に対応．
    std::vector<std::string> text_;

    //! クリック時，あるいはマウスが乗ったときにボタンの色を変える．
    int now_color_blue_{ 0 };
    int target_color_blue_{ 0 };  //!< そのための色の目標値と現在値．

    bool visible_{ true };  //!< ボタンの表示を行うかどうか．

    int pos_middle_x, pos_middle_y;  //!< ボタンの座標，中心座標．
    const int kSizeX, kSizeY;  //!< ボタンの横幅と縦幅

    //! ボタンがクリックされたときに実行される関数．
    std::function<void()> click_function_;
};

}  // namespace designlab


#endif  // DESIGNLAB_SIMPLE_BUTTON_H_
