//! @file simulation_result_recorder.h
//! @brief シミュレーションの結果を記録するクラス．

#ifndef DESIGNLAB_SIMULATION_RESULT_RECORDER_H_
#define DESIGNLAB_SIMULATION_RESULT_RECORDER_H_

#include <string>
#include <vector>

#include "graph_search_result_recoder.h"
#include "map_state.h"


namespace designlab::enums 
{

//! @enum SimulationResult
//! @brief シミュレーション全体の結果を表す列挙型
enum class SimulationResult
{
	kSuccess,						//!< 目標座標，姿勢を満たし，シミュレーションに成功した．
	kFailureByGraphSearch,			//!< グラフ探索に失敗しため，シミュレーションに失敗した．
	kFailureByLoopMotion,			//!< 動作がループしてしまったため，シミュレーションに失敗した．
	kFailureByNodeLimitExceeded,	//!< ノード数の上限に達したため，シミュレーションに失敗した．
};

}	// namespace designlab::enums


namespace designlab 
{

//! @struct SimulationResultRecorder
//! @brief シミュレーションの結果を格納する構造体．変数をごちゃごちゃさせたくないので作成．
//! @todo 名前をリネームする．
struct SimulationResultRecorder final
{
	//! @brief このクラスのデータをcsvファイルに出力する用の形式で文字列に変換する．
	//! @return csvファイルに出力する用の形式の文字列．
	std::string ToCsvString() const;


	//!< グラフ探索の結果を格納する構造体の配列．
	std::vector<GraphSearchResultRecoder> graph_search_result_recoder;	

	MapState map_state;					//!< 最新の地面の状態．

	enums::SimulationResult simulation_result;	//!< シミュレーション全体の結果．
};

}	// namespace designlab


#endif	// DESIGNLAB_SIMULATION_RESULT_RECORDER_H_