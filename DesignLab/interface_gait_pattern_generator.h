
//! @file      interface_gait_pattern_generator.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_
#define DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_

#include <vector>

#include "graph_search_result_record.h"
#include "map_state.h"
#include "robot_state_node.h"
#include "robot_operation.h"


namespace designlab
{

//! @class IGaitPatternGenerator
//! @brief グラフ探索による歩容パターン生成を行うクラスのインターフェース．
//! @details
//! @n 波東さんのプログラムで言うところのPassFindingクラス．
//! 実体は作成できないのでこれを継承してたクラスを使うこと．
//! @n 継承をするクラスのデストラクタは virtualにしておく．
//!
//! 参考
//! @li C++ でデストラクタを virtual にしなくてはならない条件と理由
//! https://www.yunabe.jp/docs/cpp_virtual_destructor.html (アクセス日 2023/12/27)
class IGaitPatternGenerator
{
public:
    IGaitPatternGenerator() = default;
    virtual ~IGaitPatternGenerator() = default;


    //! @brief グラフ探索を行い，次の動作として最適なノードを返す．
    //! @param[in] current_node 現在のロボットの状態を表すノード．
    //! 親ノードを渡す必要がある．
    //! @param[in] map 現在のマップの状態．
    //! @param[in] operation ロボットの動作．
    //! @param[out] output_node_ptr 結果のノードを代入するポインタ．
    //! @return グラフ探索の結果を返す．成功か失敗か．
    virtual GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map,
      const RobotOperation& operation,
      RobotStateNode* output_node_ptr) = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_GAIT_PATTERN_GENERATOR_H_
