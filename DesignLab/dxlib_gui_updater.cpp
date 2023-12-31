
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "dxlib_gui_updater.h"

#include <vector>

#include <DxLib.h>

#include "cassert_define.h"
#include "dxlib_gui_terminal.h"



namespace designlab
{

void DxlibGuiUpdater::Register(const std::shared_ptr<IDxlibGui>& gui_ptr, int priority)
{
    assert(kBottomPriority <= priority);
    assert(priority <= kTopPriority);

    RegisterGui(gui_ptr, priority);

    // IDxlibClickable も継承しているならば，clickable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibClickable> clickable_ptr = std::dynamic_pointer_cast<IDxlibClickable>(gui_ptr);

    if (clickable_ptr != nullptr)
    {
        RegisterClickable(clickable_ptr, priority);
    }

    // IDxlibDraggable も継承しているならば，draggable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibDraggable> draggable_ptr = std::dynamic_pointer_cast<IDxlibDraggable>(gui_ptr);

    if (draggable_ptr != nullptr)
    {
        RegisterDraggable(draggable_ptr, priority);
    }

    // IDxlibWheelHandler も継承しているならば，wheel_handler_ptrs_にも登録する．
    const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr = std::dynamic_pointer_cast<IDxlibWheelHandler>(gui_ptr);

    if (wheel_handler_ptr != nullptr)
    {
        RegisterWheelHandler(wheel_handler_ptr, priority);
    }
}

void DxlibGuiUpdater::Register(const std::shared_ptr<IDxlibClickable>& clickable_ptr, int priority)
{
    assert(kBottomPriority <= priority);
    assert(priority <= kTopPriority);

    RegisterClickable(clickable_ptr, priority);

    // IDxlibGui も継承しているならば，gui_ptrs_にも登録する．
    const std::shared_ptr<IDxlibGui> gui_ptr = std::dynamic_pointer_cast<IDxlibGui>(clickable_ptr);

    if (gui_ptr != nullptr)
    {
        RegisterGui(gui_ptr, priority);
    }

    // IDxlibDraggable も継承しているならば，draggable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibDraggable> draggable_ptr = std::dynamic_pointer_cast<IDxlibDraggable>(clickable_ptr);

    if (draggable_ptr != nullptr)
    {
        RegisterDraggable(draggable_ptr, priority);
    }

    // IDxlibWheelHandler も継承しているならば，wheel_handler_ptrs_にも登録する．
    const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr = std::dynamic_pointer_cast<IDxlibWheelHandler>(clickable_ptr);

    if (wheel_handler_ptr != nullptr)
    {
        RegisterWheelHandler(wheel_handler_ptr, priority);
    }
}

void DxlibGuiUpdater::Register(const std::shared_ptr<IDxlibDraggable>& draggable_ptr, int priority)
{
    assert(kBottomPriority <= priority);
    assert(priority <= kTopPriority);

    RegisterDraggable(draggable_ptr, priority);

    // IDxlibGui も継承しているならば，gui_ptrs_にも登録する．
    const std::shared_ptr<IDxlibGui> gui_ptr = std::dynamic_pointer_cast<IDxlibGui>(draggable_ptr);

    if (gui_ptr != nullptr)
    {
        RegisterGui(gui_ptr, priority);
    }

    // IDxlibClickable も継承しているならば，clickable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibClickable> clickable_ptr = std::dynamic_pointer_cast<IDxlibClickable>(draggable_ptr);

    if (clickable_ptr != nullptr)
    {
        RegisterClickable(clickable_ptr, priority);
    }

    // IDxlibWheelHandler も継承しているならば，wheel_handler_ptrs_にも登録する．
    const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr = std::dynamic_pointer_cast<IDxlibWheelHandler>(draggable_ptr);

    if (wheel_handler_ptr != nullptr)
    {
        RegisterWheelHandler(wheel_handler_ptr, priority);
    }
}

void DxlibGuiUpdater::Register(const std::shared_ptr<IDxlibWheelHandler>& wheel_handler_ptr, int priority)
{
    assert(kBottomPriority <= priority);
    assert(priority <= kTopPriority);

    RegisterWheelHandler(wheel_handler_ptr, priority);

    // IDxlibGui も継承しているならば，gui_ptrs_にも登録する．
    const std::shared_ptr<IDxlibGui> gui_ptr = std::dynamic_pointer_cast<IDxlibGui>(wheel_handler_ptr);

    if (gui_ptr != nullptr)
    {
        RegisterGui(gui_ptr, priority);
    }

    // IDxlibClickable も継承しているならば，clickable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibClickable> clickable_ptr = std::dynamic_pointer_cast<IDxlibClickable>(wheel_handler_ptr);

    if (clickable_ptr != nullptr)
    {
        RegisterClickable(clickable_ptr, priority);
    }

    // IDxlibDraggable も継承しているならば，draggable_ptrs_にも登録する．
    const std::shared_ptr<IDxlibDraggable> draggable_ptr = std::dynamic_pointer_cast<IDxlibDraggable>(wheel_handler_ptr);

    if (draggable_ptr != nullptr)
    {
        RegisterDraggable(draggable_ptr, priority);
    }
}


void DxlibGuiUpdater::OpenTerminal()
{
    // 2回以上呼ばれたら何もしない．
    if (is_terminal_opened_)
    {
        return;
    }

    // ターミナルに渡すGUIのリストを作成．
    std::vector<std::shared_ptr<IDxlibGui> > gui_list;

    for (const auto& i : gui_ptrs_)
    {
        gui_list.push_back(i.second);
    }

    // ターミナルを登録．
    const auto terminal_ptr = std::make_shared<DxlibGuiTerminal>(gui_list);
    const int terminal_priority = kTopPriority + 1;

    RegisterGui(terminal_ptr, terminal_priority);
    RegisterClickable(terminal_ptr, terminal_priority);

    is_terminal_opened_ = true;
}

void DxlibGuiUpdater::Activate(const std::shared_ptr<const Mouse>& mouse_ptr)
{
    assert(mouse_ptr != nullptr);

    UpdateGui();
    ActivateClickable(mouse_ptr);
    ActivateDraggable(mouse_ptr);
    ActivateWheelHandler(mouse_ptr);
}

void DxlibGuiUpdater::Draw() const
{
    // 昇順にDrawする．
    for (auto i = gui_ptrs_.begin(); i != gui_ptrs_.end(); ++i)
    {
        if ((*i).second->IsVisible())
        {
            (*i).second->Draw();
        }
    }
}


void DxlibGuiUpdater::RegisterGui(const std::shared_ptr<IDxlibGui> gui_ptr, int priority)
{
    assert(gui_ptr != nullptr);

    // すでに同じポインタが登録されていたら何もしない．
    for (const auto& i : gui_ptrs_)
    {
        if (i.second == gui_ptr)
        {
            return;
        }
    }

    // すでに同じ優先度のものがあったら，orderを1つずつずらす．
    Priority p{ priority, 0 };

    while (gui_ptrs_.find(p) != gui_ptrs_.end())
    {
        p.order++;
    }

    // 登録．
    gui_ptrs_[p] = gui_ptr;
}

void DxlibGuiUpdater::RegisterClickable(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority)
{
    assert(clickable_ptr != nullptr);

    // すでに同じポインタが登録されていたら何もしない．
    for (const auto& i : clickable_ptrs_)
    {
        if (i.second == clickable_ptr)
        {
            return;
        }
    }

    // すでに同じ優先度のものがあったら，orderを1つずつずらす.
    Priority p{ priority, 0 };

    while (clickable_ptrs_.find(p) != clickable_ptrs_.end())
    {
        p.order++;
    }

    // 登録．
    clickable_ptrs_[p] = clickable_ptr;
}

void DxlibGuiUpdater::RegisterDraggable(const std::shared_ptr<IDxlibDraggable> draggable_ptr, int priority)
{
    assert(draggable_ptr != nullptr);

    // すでに同じポインタが登録されていたら何もしない．
    for (const auto& i : draggable_ptrs_)
    {
        if (i.second == draggable_ptr)
        {
            return;
        }
    }

    // すでに同じ優先度のものがあったら，orderを1つずつずらす．
    Priority p{ priority, 0 };

    while (draggable_ptrs_.find(p) != draggable_ptrs_.end())
    {
        p.order++;
    }

    // 登録．
    draggable_ptrs_[p] = draggable_ptr;
}

void DxlibGuiUpdater::RegisterWheelHandler(const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr, int priority)
{
    assert(wheel_handler_ptr != nullptr);

    // すでに同じポインタが登録されていたら何もしない．
    for (const auto& i : wheel_handler_ptrs_)
    {
        if (i.second == wheel_handler_ptr)
        {
            return;
        }
    }

    // すでに同じ優先度のものがあったら，orderを1つずつずらす．
    Priority p{ priority, 0 };

    while (wheel_handler_ptrs_.find(p) != wheel_handler_ptrs_.end())
    {
        p.order++;
    }

    // 登録．
    wheel_handler_ptrs_[p] = wheel_handler_ptr;
}

void DxlibGuiUpdater::UpdateGui()
{
    // 逆順にUpdateする．
    for (auto i = gui_ptrs_.rbegin(); i != gui_ptrs_.rend(); ++i)
    {
        (*i).second->Update();
    }
}

void DxlibGuiUpdater::ActivateClickable(const std::shared_ptr<const Mouse> mouse_ptr)
{
    // 優先度の高いものから順にクリック判定を行う．
    // 昇順(増える順)に並んでいるので，rbegin から rend まで(降順に)走査する．

    for (auto i = clickable_ptrs_.rbegin(); i != clickable_ptrs_.rend(); ++i)
    {
        if ((*i).second->CursorOnGui(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
        {
            (*i).second->ClickedAction(
                mouse_ptr->GetCursorPosX(),
                mouse_ptr->GetCursorPosY(),
                mouse_ptr->GetPressingCount(MOUSE_INPUT_LEFT),
                mouse_ptr->GetReleasingCount(MOUSE_INPUT_MIDDLE),
                mouse_ptr->GetPressingCount(MOUSE_INPUT_RIGHT));

            break;
        }
    }
}

void DxlibGuiUpdater::ActivateDraggable(const std::shared_ptr<const Mouse> mouse_ptr)
{
    unsigned int mouse_key{ 0 };
    int pressing_count{ 0 };
    std::array<unsigned int, 3> mouse_keys = { MOUSE_INPUT_LEFT, MOUSE_INPUT_MIDDLE, MOUSE_INPUT_RIGHT };

    for (auto i : mouse_keys)
    {
        if (mouse_ptr->GetPressingCount(i) > 0)
        {
            mouse_key += i;
            pressing_count = (std::max)(mouse_ptr->GetPressingCount(i), pressing_count);
        }
    }


    if (pressing_count == 0)
    {
        // 左クリックが押されていないならば全てのドラッグを終了する
        for (auto& i : draggable_ptrs_)
        {
            i.second->SetDragged(false);
        }

        now_dragging_gui_key_ = std::nullopt;
    }
    else if (pressing_count == 1)
    {
        // 左クリックが押された瞬間，ドラッグを開始する
        for (auto i = draggable_ptrs_.rbegin(); i != draggable_ptrs_.rend(); ++i)
        {
            if ((*i).second->IsDraggable(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
            {
                (*i).second->SetDragged(true);
                now_dragging_gui_key_ = (*i).first;
                break;
            }
        }
    }
    else if (pressing_count > 0)
    {
        // 左クリックが押され続けているならばドラッグ判定を行う．
        if (now_dragging_gui_key_.has_value() && draggable_ptrs_.count(now_dragging_gui_key_.value()) == 1)
        {
            draggable_ptrs_[now_dragging_gui_key_.value()]->DraggedAction(mouse_ptr->GetDiffPosX(), mouse_ptr->GetDiffPosY(), mouse_key);
        }
    }
    else
    {
        assert(false);  // 冗長
    }
}

void DxlibGuiUpdater::ActivateWheelHandler(const std::shared_ptr<const Mouse> mouse_ptr)
{
    if (mouse_ptr->GetWheelRot() == 0) { return; }

    // 優先度の高いものから順にホイール操作判定を行う
    // 昇順(増える順)に並んでいるので，rbeginからrendまで(降順に)走査する

    for (auto i = wheel_handler_ptrs_.rbegin(); i != wheel_handler_ptrs_.rend(); ++i)
    {
        if ((*i).second->CanHandleWheel(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
        {
            (*i).second->RotMouseWheel(mouse_ptr->GetWheelRot());
            break;
        }
    }
}

}  // namespace designlab
