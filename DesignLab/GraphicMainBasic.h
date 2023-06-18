#pragma once
#include "InterfaceGraphicMain.h"
#include <vector>
#include "MapState.h"
#include "Node.h"
#include "GraphicConst.h"
#include "CameraController.h"
#include "GUIController.h"
#include "HexapodRenderer.h"


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

	std::vector<SNode> m_node;	//ロボットの動きの遷移を記録するvector

	int m_display_node = 0;		//描画しているノード

	MapState m_Map;				//表示するマップ．

	int m_counter = 0;			//このクラスが実行されてから何回update関数が呼ばれたかカウントする．

	const int GET_NODE_COUNT = 2 * GraphicConst::GRAPHIC_FPS;	//2秒ごとに読み出す．
};


//! @file GraphicMainBasic.h
//! @brief 基本的な描画クラスの実装．
//! @author 長谷川

//! @class GraphicMainBasic
//! @brief このプロジェクトにおける標準的なロボットの描画機能を持つクラス．
//! @details 波東さんのプログラムのロボット表示機能を書き直したもの．<br>
//! 基本的な処理の内容は変化していないが，より表示するデータの内容が詳しくなっている．<br>
//! UIによってランタイムで表示方法を制御することができるようになったため，よりロボットの状態を理解しやすくなっている．<br>
//! @note 処理を大きく書き換えたい場合はそもそも新しいクラスを書くようにするとよいと思う．<br>
//! GraphicSampleを参考にして，作成するようにすると楽．
//! @author 長谷川
