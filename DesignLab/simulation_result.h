#pragma once

#include <string>


//! @enum ESimulationResult
//! @date 2023/08/24
//! @author 長谷川
//! @brief シミュレーションの結果を表す列挙型
enum class ESimulationResult
{
	SUCCESS,						//!< シミュレーションに成功した
	FAILURE,						//!< シミュレーションに失敗した
	FAILURE_BY_GRAPH_SEARCH,		//!< グラフ探索に失敗しため，シミュレーションに失敗した
	FAILURE_BY_LOOP_MOTION,			//!< 動作がループしてしまったため，シミュレーションに失敗した
	FAILURE_BY_NODE_LIMIT_EXCEEDED,	//!< ノード数の上限に達したため，シミュレーションに失敗した
};


namespace std
{
	//! @brief ESimulationResult型を文字列に変換する関数
	//! @param [in] result ESimulationResult型の変数
	//! @return std::string ESimulationResult型の変数を文字列にしたもの
	std::string to_string(ESimulationResult result)
	{
		switch (result)
		{
		case ESimulationResult::SUCCESS:
			return "SUCCESS";
		case ESimulationResult::FAILURE:
			return "FAILURE";
		case ESimulationResult::FAILURE_BY_GRAPH_SEARCH:
			return "FAILURE_BY_GRAPH_SEARCH";
		case ESimulationResult::FAILURE_BY_LOOP_MOTION:
			return "FAILURE_BY_LOOP_MOTION";
		case ESimulationResult::FAILURE_BY_NODE_LIMIT_EXCEEDED:
			return "FAILURE_BY_NODE_LIMIT_EXCEEDED";
		default:
			return "UNKNOWN";
		}
	}

}