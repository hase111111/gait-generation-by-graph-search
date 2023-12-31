
//! @file      dxlib_gui_updater.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_UPDATER_H_
#define DESIGNLAB_DXLIB_GUI_UPDATER_H_

#include <map>
#include <memory>
#include <optional>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "interface_dxlib_wheel_handler.h"
#include "mouse.h"


namespace designlab
{

//! @class DxlibGuiUpdater
//! @brief クリック判定を行うクラス．
//! @details 一度のクリックで複数のGUIが反応することを防ぐために，優先度を設定する．
//! @n 優先度が高いものから順にクリック判定を行い，クリックされたらそのGUIの Activate 関数を実行する．
//! @n 同様に，ドラッグ判定，ホイール操作判定も行う．
//! @n 神クラスになっている感は否めないが，また問題が起きたら修正する．
class DxlibGuiUpdater final
{
public:
    static constexpr int kBottomPriority{ 0 };      //!< 最も優先度が低い．
    static constexpr int kTopPriority{ 1000000 };   //!< 最も優先的に処理される．

    //! @brief UpdateとDrawを行うGUIを登録する．
    //! @n IDxlibClickableまたは，IDxlibDraggable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
    //! @param[in] gui_ptr UpdateとDrawを行うGUIのポインタ．
    //! @param[in] priority GUIの優先度．これが高いほど優先的にUpdateとDrawが行われる．
    //! @n メンバ変数のkBottomPriority～kTopPriorityの間の値を設定すること．
    void Register(const std::shared_ptr<IDxlibGui>& gui_ptr, int priority);

    //! @brief クリック可能なGUIを登録する．
    //! @n IDxlibGuiまたは，IDxlibDraggable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
    //! @param[in] clickable_ptr クリック可能なGUIのポインタ．
    //! @param[in] priority クリック入力の優先度．これが高いほど優先的にクリックの判定がされる．
    //! @n メンバ変数のkBottomPriority～kTopPriorityの間の値を設定すること．
    void Register(const std::shared_ptr<IDxlibClickable>& clickable_ptr, int priority);

    //! @brief ドラッグ可能なGUIを登録する．
    //! @n IDxlibGuiまたは，IDxlibClickable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
    //! @param[in] draggable_ptr ドラッグ可能なGUIのポインタ．
    //! @param[in] priority ドラッグ入力の優先度．これが高いほど優先的にドラッグの判定がされる．
    //! @n メンバ変数のkBottomPriority～kTopPriorityの間の値を設定すること．
    void Register(const std::shared_ptr<IDxlibDraggable>& draggable_ptr, int priority);

    //! @brief ホイール操作を行うGUIを登録する．
    //! @n IDxlibGuiまたは，IDxlibClickable,IDxlibDraggableを継承している場合は，それらも同時に登録する．
    //! @param[in] wheel_handler_ptr ホイール操作を行うGUIのポインタ．
    //! @param[in] priority ホイール操作の優先度．これが高いほど優先的にホイール操作の判定がされる．
    //! @n メンバ変数のkBottomPriority～kTopPriorityの間の値を設定すること．
    void Register(const std::shared_ptr<IDxlibWheelHandler>& wheel_handler_ptr, int priority);

    //! @brief Terminalを開く.
    //! @n 他のGUIをRegisterした後に呼び出すこと．
    //! @n 2回目以降の呼び出しは無視される．
    //! @n また，Terminalは最も優先度が高い．
    void OpenTerminal();

    //! @brief GUIのUpdate関数を実行し，クリック，ドラッグなどの判定を行う．
    //! @n 優先度の高いものから順に各種判定を行う．
    //! @param[in] mouse_ptr マウスの状態を管理するクラスのポインタ．
    void Activate(const std::shared_ptr<const Mouse>& mouse_ptr);

    //! @brief 登録済みのGUIのDraw関数を実行する．
    void Draw() const;

private:
    //! @struct Priority
    //! @brief 優先度と判定の順番を管理する構造体．
    struct Priority final
    {
        constexpr Priority(const int p, const int o) noexcept : priority(p), order(o) {}

        //! @brief mapでの優先度の比較に使用する．
        //! @n 優先度を比較し，優先度が同じならば追加された順番で比較する．
        bool operator<(const Priority& other) const noexcept
        {
            if (priority == other.priority)
            {
                return order < other.order;
            }
            else
            {
                return priority < other.priority;
            }
        }

        int priority{ 0 };  //!< 優先度．
        int order{ 0 };     //!< 追加された順番．
    };

    void RegisterGui(const std::shared_ptr<IDxlibGui> gui_ptr, int priority);
    void RegisterClickable(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority);
    void RegisterDraggable(const std::shared_ptr<IDxlibDraggable> draggable_ptr, int priority);
    void RegisterWheelHandler(const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr, int priority);

    void UpdateGui();

    void ActivateClickable(const std::shared_ptr<const Mouse> mouse_ptr);
    void ActivateDraggable(const std::shared_ptr<const Mouse> mouse_ptr);
    void ActivateWheelHandler(const std::shared_ptr<const Mouse> mouse_ptr);

    std::map<Priority, std::shared_ptr<IDxlibGui> > gui_ptrs_;
    std::map<Priority, std::shared_ptr<IDxlibClickable> > clickable_ptrs_;
    std::map<Priority, std::shared_ptr<IDxlibDraggable> > draggable_ptrs_;
    std::map<Priority, std::shared_ptr<IDxlibWheelHandler> > wheel_handler_ptrs_;

    std::optional<Priority> now_dragging_gui_key_{ std::nullopt };  //!< ドラッグ中のGUIの優先度と順番，未ドラッグ時はstd::nullopt．

    bool is_terminal_opened_{ false };  //!< Terminal が開かれているかどうか．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_UPDATER_H_
