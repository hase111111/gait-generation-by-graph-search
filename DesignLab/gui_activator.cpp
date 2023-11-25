#include "gui_activator.h"

#include <DxLib.h>


void GuiActivator::Activate(const std::shared_ptr<const Mouse> mouse_ptr)
{
	for (auto& clickable_ptr : clickable_ptrs_)
	{
		if (clickable_ptr->OnCursor(mouse_ptr->GetCursorPosX(), mouse_ptr->GetCursorPosY()))
		{
			clickable_ptr->Activate(mouse_ptr);
			break;
		}
	}
}