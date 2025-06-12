﻿
//! @file simulation_result_record.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_SIMULATION_RESULT_RECORD_H_
#define DESIGNLAB_SIMULATION_RESULT_RECORD_H_

#include <string>
#include <vector>

#include "graph_search_result_record.h"
#include "map_state.h"


namespace designlab::enums
{

//! @enum SimulationResult
//! @brief シミュレーション全体の結果を表す列挙型.
enum class SimulationResult
{
    //! 目標座標,姿勢を満たし,シミュレーションに成功した.
    kSuccess,

    //! グラフ探索に失敗しため,シミュレーションに失敗した.
    kFailureByGraphSearch,

    //! 動作がループしてしまったため,シミュレーションに失敗した.
    kFailureByLoopMotion,

    //! ノード数の上限に達したため,シミュレーションに失敗した.
    kFailureByNodeLimitExceeded,
};

}  // namespace designlab::enums


namespace designlab
{

//! @struct SimulationResultRecord
//! @brief シミュレーションの結果を格納する構造体.
//! @details 変数をたくさん宣言させたくないので作成した.
struct SimulationResultRecord final
{
    //! @brief このクラスのデータを,
    //! csvファイルに出力する用の形式で文字列に変換する.
    //! @return csvファイルに出力する用の形式の文字列.
    std::string ToCsvString() const;


    //!< グラフ探索の結果を格納する構造体の配列.
    std::vector<GraphSearchResultRecord> graph_search_result_recorder;

    MapState map_state;  //!< 最新の地面の状態.

    enums::SimulationResult simulation_result;  //!< シミュレーション全体の結果.
};

}  // namespace designlab


#endif  // DESIGNLAB_SIMULATION_RESULT_RECORD_H_
