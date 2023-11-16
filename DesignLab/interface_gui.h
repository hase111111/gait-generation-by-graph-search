//! @file interface_gui.h
//! @brief GUIのインターフェース

#ifndef DESIGNLAB_INTERFACE_GUI_H_
#define DESIGNLAB_INTERFACE_GUI_H_


class IGui
{
public:
	IGui() = default;
	virtual ~IGui() = default;

	//! @brief GUIの更新，毎フレーム実行すること．
	virtual void Update() = 0;

	//! @brief GUIの描画．
	virtual void Draw() const = 0;
};

#endif	// DESIGNLAB_INTERFACE_GUI_H_