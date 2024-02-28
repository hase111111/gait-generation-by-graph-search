
//! @file      graphic_data_broker.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPHIC_DATA_BROKER_H_
#define DESIGNLAB_GRAPHIC_DATA_BROKER_H_

#include <vector>

#include "asyncable_data.h"
#include "map_state.h"
#include "robot_state_node.h"


namespace designlab
{

//! @struct GraphicDataBroker
//! @brief グラフ探索の結果を別スレッドのグラフィッククラスに連絡するための構造体．
//! @details
//! Broker : ブローカー，仲介人のこと．
//! @n データ処理部(グラフ探索)が更新したデータをこの構造体に渡し，
//! 画像表示部がこの構造体から更新されたデータを持っていき，描画する．
//! @n グラフ探索　→　GraphicDataBroker　→　画像表示部
struct GraphicDataBroker final
{
    //! マップの情報，カメラをロボットに設置する場合，ロボットの位置も含まれる．
    AsyncableData<MapState> map_state;

    AsyncableData<std::vector<RobotStateNode> > graph;

    AsyncableData<std::vector<size_t> > simulation_end_index;
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPHIC_DATA_BROKER_H_
