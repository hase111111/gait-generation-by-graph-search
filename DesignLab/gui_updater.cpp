#include "gui_updater.h"

#include <DxLib.h>

#include "cassert_define.h"


void GuiUpdater::RegisterGui(const std::shared_ptr<IDxlibGui> gui_ptr, int priority)
{
	assert(gui_ptr != nullptr);

	Priority p{ priority, 0 };

	// すでに同じ優先度のものがあったら，orderを1つずつずらす
	while (gui_ptrs_.find(p) != gui_ptrs_.end())
	{
		p.order++;
	}

	gui_ptrs_[p] = gui_ptr;
}

void GuiUpdater::RegisterClickable(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority)
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

void GuiUpdater::RegisterDraggable(const std::shared_ptr<IDxlibDraggable> draggable_ptr, int priority)
{
	assert(draggable_ptr != nullptr);

	Priority p{ priority, 0 };

	// すでに同じ優先度のものがあったら，orderを1つずつずらす
	while (draggable_ptrs_.find(p) != draggable_ptrs_.end())
	{
		p.order++;
	}

	draggable_ptrs_[p] = draggable_ptr;
}


void GuiUpdater::Activate(const std::shared_ptr<const Mouse> mouse_ptr)
{
	assert(mouse_ptr != nullptr);

	UpdateGui();
	ActivateClickable(mouse_ptr);
	ActivateDraggable(mouse_ptr);
}

void GuiUpdater::Draw() const
{
	//昇順にDrawする．
	for (auto i = gui_ptrs_.begin(); i != gui_ptrs_.end(); ++i)
	{
		(*i).second->Draw();
	}
}

void GuiUpdater::UpdateGui()
{
	//逆順にUpdateする．
	for (auto i = gui_ptrs_.rbegin(); i != gui_ptrs_.rend(); ++i)
	{
		(*i).second->Update();
	}
}

void GuiUpdater::ActivateClickable(const std::shared_ptr<const Mouse> mouse_ptr)
{
	// 優先度の高いものから順にクリック判定を行う
	// 昇順(増える順)に並んでいるので，rbeginからrendまで(降順に)走査する

	for (auto i = clickable_ptrs_.rbegin(); i != clickable_ptrs_.rend(); ++i)
	{
		if ((*i).second->CursorOnGui(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
		{
			(*i).second->ClickedAction(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY(),
				mouse_ptr->GetPressingCount(MOUSE_INPUT_LEFT), mouse_ptr->GetReleasingCount(MOUSE_INPUT_MIDDLE), mouse_ptr->GetPressingCount(MOUSE_INPUT_RIGHT));
			break;
		}
	}
}

void GuiUpdater::ActivateDraggable(const std::shared_ptr<const Mouse> mouse_ptr)
{
	unsigned int mouse_key{ 0 };
	int pressing_count{ 0 };
	std::array<unsigned int, 3> mouse_keys = { MOUSE_INPUT_LEFT, MOUSE_INPUT_MIDDLE, MOUSE_INPUT_RIGHT };

	for (auto i : mouse_keys)
	{
		if (mouse_ptr->GetPressingCount(i) > 0)
		{
			mouse_key = i;
			pressing_count = mouse_ptr->GetPressingCount(i);
			break;
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
		// 左クリックが押され続けているならばドラッグ判定を行う
		if (now_dragging_gui_key_.has_value() && draggable_ptrs_.count(now_dragging_gui_key_.value()) == 1)
		{
			draggable_ptrs_[now_dragging_gui_key_.value()]->DraggedAction(mouse_ptr->GetDiffPosX(), mouse_ptr->GetDiffPosY(), mouse_key);
		}
	}
	else
	{
		assert(false);	//冗長	
	}
}