#include "SystemMain.h"
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include "Define.h"
#include "GraphSearch.h"
#include "CmdIO.h"
#include "hexapod.h"

SystemMain::SystemMain()
{
	//ロボットのデータを初期化する．
	Hexapod::makeLegROM_r();

	//マップを生成する．
	m_Map.init(EMapCreateMode::Flat, MapCreator::OPTION_PERFORATED | MapCreator::OPTION_STEP, false);

	//仲介人にマップを渡す．
	m_Broker.setMapState(m_Map);

	//画像ウィンドウを表示するクラスに仲介人のアドレスを渡して，初期化処理をする．
	m_Graphic.init(&m_Broker);

	//この探索での目標を設定する．
	m_target.TargetDirection = myvector::VGet(0, 1, 0);					//目標方向x,y,z(直進移動)
	m_target.TargetPosition = myvector::VGet(0, 10000, 0);				//目標位置
	m_target.TargetRotation = myvector::VGet(0, 0, 1);					//目標旋回方向P,R,Y(現在Y方向しか考えていない値は1が左回転、-1が右回転)
	m_target.TargetAngle = myvector::VGet(0, 0, Define::MY_PI / 2.0f);	//目標旋回角度(胴体の角度)
	m_target.RotationCenter = myvector::VGet(-10000, 0, 0);				//回転中心x,y,z
	m_target.TargetMode = ETargetMode::TURN_DIRECTION;					// ETargetModeの中から目標の評価方法を設定する．
	m_target.TurningRadius = abs(m_target.RotationCenter.x);
	m_target.TurningRadius = myvector::VMag(m_target.RotationCenter);	//旋回半径 これだと、原点と旋回中心との距離,いや更新してないから現状では一応よさげ、y軸上を直進させるなら、ｘの距離つまり、旋回中心とy軸との距離

}

void SystemMain::main()
{
	//画像表示ウィンドウを別スレッドで立ち上げる．初期化に失敗したり，そもそも画像表示をしない設定になっていると立ち上がらない．
	boost::thread _thread_graphic(&GraphicSystem::main, &m_Graphic);

	CmdIO _cmd;	//コマンドラインに文字を描画するクラスを用意する．


	//シミュレーションを行う回数分ループする．
	for (int i = 0; i < Define::SIMURATE_NUM; i++)
	{
		SNode _current_node;									//現在のノードの状態を格納する変数．
		const bool _do_random_init = (i == 0) ? false : true;	// i の値が 0 ならばランダムな場所に初期化はしない．(i == 0)を評価して，trueならば前者(false)，falseならば後者(true)を代入する．
		initNode(_current_node, _do_random_init);				//ノードの位置を初期化する．

		if (Define::FLAG_GRAPHIC_AVAILABLE == true) { m_Broker.pushNode(_current_node); }	//グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．

		_cmd.outputGraphSearchStaretMessage(i + 1);	//コマンドラインに開始時のメッセージを出力する．
		_cmd.outputNode(_current_node, 0);			//コマンドラインに最初のノードの状態を出力する．


		//最大歩容生成回数分までループする．
		for (int i = 0; i < Define::GATE_PATTERN_GENERATE_NUM; i++)
		{
			GraphSearch _GraphSearch;		//グラフ探索用クラスを用意する．
			SNode _result_node;				//グラフ探索の結果を格納する変数．
			boost::timer::cpu_timer _timer;	//グラフ探索にかかった時間を出力するためのタイマー


			_timer.start();		//タイマースタート．
			bool _is_sucess = _GraphSearch.getNextNodebyGraphSearch(_current_node, &m_Map, m_target, _result_node);		//グラフ探索を行う．
			_timer.stop();		//タイマーストップ．


			if (_is_sucess == false) 
			{
				//次の歩容が生成できなかったら，ループを一つ抜け，次のシミュレーションへ進む．
				_cmd.outputErrorMessageInGraphSearch("Failed to generate the next gait.");
				break;
			}

			_current_node = _result_node;		//次の歩容が生成できているならば，ノードを更新する．

			if (Define::FLAG_GRAPHIC_AVAILABLE == true) { m_Broker.pushNode(_current_node); }	//グラフィックが有効ならば仲介人に結果を通達する．
			_cmd.outputNode(_current_node, i + 1);												//コマンドラインに現在のノードを出力する．


			m_Checker.setNode(_current_node);		//動作チェッカーにもノードを通達する．

			if (m_Checker.isLoopMove() == true) 
			{
				//動作がループしてしまっているならば，ループを一つ抜け，次のシミュレーションへ進む．
				_cmd.outputErrorMessageInGraphSearch("Motion stuck in a loop.");
				break;
			}
		}


	}

	//画像表示ウィンドウの終了を待つ．
	_thread_graphic.join();
}
