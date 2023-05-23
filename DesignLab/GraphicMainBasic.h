#pragma once
#include "InterfaceGraphicMain.h"
#include "MapState.h"
#include <vector>
#include "listFunc.h"
#include "GraphicConst.h"
#include "CameraController.h"
#include "Hexapod.h"

class GraphicMainBasic final : public IGraphicMain
{
public:
	GraphicMainBasic(const GraphicDataBroker* _broker);
	~GraphicMainBasic() = default;

	bool update() override;

	void draw() const override;

private:

	CameraController m_Camera;	//カメラ制御クラス．

	MapState m_Map;				//表示するマップ．

	std::vector<SNode> m_node;	//ロボットの動きの遷移を記録するvector

	Hexapod m_hexapod;			//ロボットの状態を管理するクラス．
	
	int m_counter = 0;			// このクラスが実行されてから何回update関数が呼ばれたかカウントする

	int m_display_node = 0;		//描画しているノード

	const int CHANGE_NEXT_NODE = (int)(0.2 * GraphicConst::GRAPHIC_FPS);	//次のノードを再生するまでの時間．

	const int GET_NODE_COUNT = 2 * GraphicConst::GRAPHIC_FPS;	//2秒ごとに読み出す．
};
