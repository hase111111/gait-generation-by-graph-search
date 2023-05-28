#pragma once
#include "GraphicConst.h"
#include "Node.h"
#include "ButtonController.h"
#include "CameraController.h"
#include <memory>
#include <vector>

// UIを表示する関数．表示するものを変更したい場合は編集してください．
class GUIController
{
public:
	GUIController();

	void update(CameraController& _camera);

	void draw(const SNode _node) const;

private:

	const int BOX_X = 300;
	const int BOX_Y = GraphicConst::WIN_Y - 25 * 2;
	const int CENTER_X = 25 + BOX_X / 2;
	const int CENTER_Y = GraphicConst::WIN_Y / 2;

	//UIを表示するかどうか．
	bool m_is_displayed = true;

	//左側にノードの状態を描画する関数．大変汚い関数です．申し訳ない
	void drawNodeByStr(const SNode _node) const;

	//右側にボタンの使い方を描画する関数．同上
	void drawExplanationByStr() const;

	// GUIに表示するボタンを管理するクラス
	std::vector<std::unique_ptr<ButtomController>> m_buttom;
};
