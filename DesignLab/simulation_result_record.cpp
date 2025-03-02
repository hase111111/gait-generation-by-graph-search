﻿
//! @file simulation_result_record.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "simulation_result_record.h"

#include <sstream>

#include "string_util.h"
#include "hexapod_const.h"
#include "leg_state.h"


namespace designlab
{

std::string SimulationResultRecord::ToCsvString() const
{
    // シミュレーションの最終的な結果の出力．
    std::stringstream ss;
    ss << "Simulation Result," <<
        string_util::EnumToStringRemoveTopK(simulation_result) << std::endl;
    ss << std::endl;

    ss << GraphSearchResultRecord::GetCsvHeader() << std::endl;

    const size_t kLength = graph_search_result_recorder.size();

    for (size_t i = 0; i < kLength; i++)
    {
        ss << i << ",";

        // グラフ探索の結果の出力．
        ss << graph_search_result_recorder[i].ToCsvString() << ",";

        ss << std::endl;
    }


    ss << std::endl;

    return ss.str();
}

}  // namespace designlab
