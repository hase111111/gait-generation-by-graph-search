//! @file interface_dxlib_gui.h
//! @brief Dxlibの画面に表示するGUIのインターフェース．

#ifndef DESIGNLAB_INTERFACE_DXLIB_GUI_H_
#define DESIGNLAB_INTERFACE_DXLIB_GUI_H_


//! @class IDxlibGui
//! @brief Dxlibの画面に表示するGUIのインターフェース．
class IDxlibGui
{
public:

	virtual ~IDxlibGui() = default;

	//! @brief GUIの更新，毎フレーム実行すること．
	virtual void Update() = 0;

	//! @brief GUIの描画．
	virtual void Draw() const = 0;

	//! @brief GUIの表示を行うかどうかを設定する．
	//! @param[in] visible GUIの表示を行うかどうか．
	virtual void SetVisible(bool visible) = 0;

	//! @brief GUIの表示を行うかどうかを返す．
	//! @return bool GUIの表示を行うかどうか．
	virtual bool IsVisible() const = 0;
};

#endif	// DESIGNLAB_INTERFACE_DXLIB_GUI_H_