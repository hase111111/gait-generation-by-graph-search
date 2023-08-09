#pragma once
#include <memory>
#include "map_state.h"
#include "Target.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "InterfacePassFinder.h"
#include "interface_graphic_main.h"


//! @class SystemMain
//! @date 2023/08/06
//! @author 長谷川
//! @brief 中～大規模な設計において，int mainになんでも詰め込むわけにはいかないため，このクラスにまとめる．
//! @details 処理の内容を書き換えるときには，int mainから呼ぶクラスを変えるだけでいい．
class SystemMain final
{
public:
	SystemMain(std::unique_ptr<IPassFinder>&& graph_search);
	~SystemMain() = default;

	//! @brief いままでint mainで行われた処理をまとめたもの．目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．規定の回数シミュレーションしたら終了する．
	void main();

private:
	MapState m_map_state;
	STarget m_target;
	GraphicDataBroker m_broker;
	GraphicSystem m_graphic_system;
	std::unique_ptr<IPassFinder> mp_pass_finder;
};


//! @file system_main.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief このプログラムの処理をまとめたもの．処理の内容を大きく変えたい場合はint main()から，全く別のクラスを呼べばよい．
//! @n 行数 : @lineinfo