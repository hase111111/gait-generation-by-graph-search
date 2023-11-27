#include "gui_activator.h"

#include "cassert_define.h"


void GuiActivator::Register(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority)
{
	assert(clickable_ptr != nullptr);

	Priority p{ priority, 0 };

	// すでに同じ優先度のものがあったら，orderを1つずつずらす
	while (clickable_ptrs_.find(p) != clickable_ptrs_.end())
	{
		p.order++;
	}

	clickable_ptrs_[p] = clickable_ptr;
}

void GuiActivator::Activate(const std::shared_ptr<const Mouse> mouse_ptr)
{
	assert(mouse_ptr != nullptr);

	// 優先度の高いものから順にクリック判定を行う
	// 昇順(増える順)に並んでいるので，rbeginからrendまで(降順に)走査する

	for (auto i = clickable_ptrs_.rbegin(); i != clickable_ptrs_.rend(); ++i)
	{
		if ((*i).second->OnCursor(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
		{
			(*i).second->Activate(mouse_ptr);
			break;
		}
	}
}