#pragma once
#include "MapState.h"
#include "GraphicDataBroker.h"
#include "GraphicSystem.h"
#include "NodeValidityChecker.h"
#include "Target.h"

//このプログラムの処理をまとめたもの．処理の内容を大きく変えたい場合はint main()から，全く別のクラスを呼べばよい．
class SystemMain final
{
public:
	SystemMain();
	~SystemMain() = default;

	void main();

private:
	MapState m_Map;
	STarget m_target;
	GraphicDataBroker m_Broker;
	GraphicSystem m_Graphic;
	NodeValidityChecker m_Checker;
};
