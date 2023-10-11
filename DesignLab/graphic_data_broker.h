//! @file graphic_data_broker.h
//! @brief グラフ探索の結果を別スレッドのグラフィッククラスに連絡するための構造体

#ifndef DESIGNLAB_GRAPHIC_DATA_BROKER_H_
#define DESIGNLAB_GRAPHIC_DATA_BROKER_H_


#include "asyncable_data.h"
#include "map_state.h"
#include "robot_state_node.h"


//! @struct GraphicDataBroker
//! @brief 画像表示部と，データ処理部を結びつける仲介人
//! @details Broker:ブローカー，仲介人のこと．
//! @n データ処理部(グラフ探索)が更新したデータをこの構造体に渡し，画像表示部がこの構造体から更新されたデータを持っていき，描画する．
//! @n
//! @n グラフ探索　→　GraphicDataBroker　→　画像表示部
struct GraphicDataBroker final
{
	AsyncableData<MapState> map_state;					//!< マップの情報，カメラをロボットに設置する場合，

	AsyncableData<std::vector<RobotStateNode>> graph;

	AsyncableData<std::vector<size_t>> simu_end_index;
};


#endif // !DESIGNLAB_GRAPHIC_DATA_BROKER_H_