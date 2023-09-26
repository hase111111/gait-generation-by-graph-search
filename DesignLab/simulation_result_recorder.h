#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "node.h"
#include "graph_search_result.h"



//! @enum SimulationResult
//! @brief シミュレーションの結果を表す列挙型
enum class SimulationResult
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
	std::string to_string(SimulationResult result);

} // namespace std



//! @struct SimulationResultRecorder
//! @brief シミュレーションの結果を格納する構造体．変数をごちゃごちゃさせたくないので作成
//! @n 最初のSはStructのS
struct SimulationResultRecorder final
{
	std::vector<SNode> result_nodes;						//!< 動作の記録
	std::vector<double> computation_time;					//!< グラフ探索にかかった時間
	std::vector<EGraphSearchResult> graph_search_results;	//!< グラフ探索の結果
	SimulationResult simulation_result;					//!< シミュレーション全体の結果
};


std::ofstream& operator<<(std::ofstream& ofs, const SimulationResultRecorder& record);



//! @file simulation_result_recorder.h
//! @brief シミュレーションの結果を記録するクラス．