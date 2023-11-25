#ifndef DESIGNLAB_GUI_ACTIVATOR_H_
#define DESIGNLAB_GUI_ACTIVATOR_H_

#include <memory>
#include <vector>

#include "interface_dxlib_clickable.h"
#include "mouse.h"


class GuiActivator final
{
public:

	inline void Register(const std::shared_ptr<IDxlibClickable> clickable_ptr) 
	{
		clickable_ptrs_.push_back(clickable_ptr);
	}

	void Activate(const std::shared_ptr<const Mouse> mouse_ptr);

private:
	std::vector<std::shared_ptr<IDxlibClickable>> clickable_ptrs_;
};


#endif	// DESIGNLAB_GUI_ACTIVATOR_H_