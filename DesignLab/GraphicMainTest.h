#pragma once
#include "InterfaceGraphicMain.h"
#include "CameraController.h"
#include "HexapodRenderer.h"
#include "GUIController.h"
#include "Node.h"
#include "MapState.h"

//MapStateやHexapodStateClaculatorが動作しているか視覚的に分かりやすくするためのテストシーン
class GraphicMainTest final : public IGraphicMain
{
public:
	GraphicMainTest(const GraphicDataBroker* _broker);
	~GraphicMainTest() = default;

	bool update() override;

	void draw() const override;

private:
	SNode m_node;

	CameraController m_Camera;	//カメラ制御クラス．
	HexapodRenderer m_HexapodRender;
	MapState m_Map;
	GUIController m_GUI;		// GUI (ロボットの状態とか表示する窓) を制御するクラス．
};
