#pragma once
#include "InterfaceGraphicMain.h"
#include <vector>
#include "MapState.h"
#include "Node.h"
#include "GraphicConst.h"
#include "CameraController.h"
#include "GUIController.h"
#include "HexapodRenderer.h"

//このプロジェクトにおける標準的なロボットの描画機能を持つクラス．
class GraphicMainBasic final : public IGraphicMain
{
public:
	GraphicMainBasic(const GraphicDataBroker* _broker);
	~GraphicMainBasic() = default;

	bool update() override;

	void draw() const override;

private:

	CameraController m_Camera;	//カメラ制御クラス．

	HexapodRenderer m_HexapodRender;

	GUIController m_GUI;		// GUI (ロボットの状態とか表示する窓) を制御するクラス．
	
	int m_counter = 0;			// このクラスが実行されてから何回update関数が呼ばれたかカウントする

	std::vector<SNode> m_node;	//ロボットの動きの遷移を記録するvector

	int m_display_node = 0;		//描画しているノード

	MapState m_Map;				//表示するマップ．


	const int CHANGE_NEXT_NODE = (int)(0.2 * GraphicConst::GRAPHIC_FPS);	//次のノードを再生するまでの時間．

	const int GET_NODE_COUNT = 2 * GraphicConst::GRAPHIC_FPS;	//2秒ごとに読み出す．
};
