//! @file gui_updater.h
//! @brief クリック判定を行うクラス．

#ifndef DESIGNLAB_GUI_UPDATER_H_
#define DESIGNLAB_GUI_UPDATER_H_

#include <map>
#include <memory>
#include <optional>

#include "interface_dxlib_clickable.h"
#include "interface_dxlib_draggable.h"
#include "interface_dxlib_gui.h"
#include "interface_dxlib_wheel_handler.h"
#include "mouse.h"


//! @class GuiUpdater
//! @brief クリック判定を行うクラス．
//! @n 一度のクリックで複数のGUIが反応することを防ぐために，優先度を設定する．
//! @n 優先度が高いものから順にクリック判定を行い，クリックされたらそのGUIのActivate関数を実行する．
class GuiUpdater final
{
public:

	//! @brief UpdateとDrawを行うGUIを登録する．
	//! @n IDxlibClickableまたは，IDxlibDraggable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
	//! @param[in] gui_ptr UpdateとDrawを行うGUIのポインタ．
	//! @param[in] priority GUIの優先度．これが高いほど優先的にUpdateとDrawが行われる．
	void Register(const std::shared_ptr<IDxlibGui> gui_ptr, int priority);

	//! @brief クリック可能なGUIを登録する．
	//! @n IDxlibGuiまたは，IDxlibDraggable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
	//! @param[in] clickable_ptr クリック可能なGUIのポインタ．
	//! @param[in] priority クリック入力の優先度．これが高いほど優先的にクリックの判定がされる．
	void Register(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority);

	//! @brief ドラッグ可能なGUIを登録する．
	//! @n IDxlibGuiまたは，IDxlibClickable,IDxlibWheelHandlerを継承している場合は，それらも同時に登録する．
	//! @param[in] draggable_ptr ドラッグ可能なGUIのポインタ．
	//! @param[in] priority ドラッグ入力の優先度．これが高いほど優先的にドラッグの判定がされる．
	void Register(const std::shared_ptr<IDxlibDraggable> draggable_ptr, int priority);

	//! @brief ホイール操作を行うGUIを登録する．
	//! @n IDxlibGuiまたは，IDxlibClickable,IDxlibDraggableを継承している場合は，それらも同時に登録する．
	//! @param[in] wheel_handler_ptr ホイール操作を行うGUIのポインタ．
	//! @param[in] priority ホイール操作の優先度．これが高いほど優先的にホイール操作の判定がされる．
	void Register(const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr, int priority);

	//! @brief GUIのUpdate関数を実行し，クリック，ドラッグなどの判定を行う．
	//! @n 優先度の高いものから順に各種判定を行う．
	//! @param[in] mouse_ptr マウスの状態を管理するクラスのポインタ．
	void Activate(const std::shared_ptr<const Mouse> mouse_ptr);

	//! @brief 登録済みのGUIのDraw関数を実行する．
	void Draw() const;

private:

	//! @struct Priority
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

		int priority{ 0 };		//!< 優先度
		int order{ 0 };			//!< 追加された順番
	};

	void RegisterGui(const std::shared_ptr<IDxlibGui> gui_ptr, int priority);
	void RegisterClickable(const std::shared_ptr<IDxlibClickable> clickable_ptr, int priority);
	void RegisterDraggable(const std::shared_ptr<IDxlibDraggable> draggable_ptr, int priority);
	void RegisterWheelHandler(const std::shared_ptr<IDxlibWheelHandler> wheel_handler_ptr, int priority);

	void UpdateGui();

	void ActivateClickable(const std::shared_ptr<const Mouse> mouse_ptr);
	void ActivateDraggable(const std::shared_ptr<const Mouse> mouse_ptr);
	void ActivateWheelHandler(const std::shared_ptr<const Mouse> mouse_ptr);

	std::map<Priority, std::shared_ptr<IDxlibGui>> gui_ptrs_;
	std::map<Priority, std::shared_ptr<IDxlibClickable>> clickable_ptrs_;
	std::map<Priority, std::shared_ptr<IDxlibDraggable>> draggable_ptrs_;
	std::map<Priority, std::shared_ptr<IDxlibWheelHandler>> wheel_handler_ptrs_;

	std::optional<Priority> now_dragging_gui_key_{ std::nullopt };		//!< ドラッグ中のGUIの優先度と順番
};


#endif	// DESIGNLAB_GUI_UPDATER_H_