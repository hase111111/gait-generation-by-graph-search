#pragma once

#include <vector>
#include <fstream>

#include "node.h"
#include "graph_search_result.h"
#include "simulation_result.h"



//! @struct SSimulationRecord
//! @date 2023/08/24
//! @author 長谷川
//! @brief シミュレーションの結果を格納する構造体．変数をごちゃごちゃさせたくないので作成
//! @n 最初のSはStructのS
struct SSimulationRecord final
{
	std::vector<SNode> m_node;	//!< 動作の記録
	std::vector<double> m_time;	//!< グラフ探索にかかった時間
	std::vector<EGraphSearchResult> m_graph_search_result;	//!< グラフ探索の結果
	ESimulationResult m_simulation_result;	//!< シミュレーション全体の結果

	//int m_clear_num;							//一定距離歩行できてシミュレーションが終了した回数
	//int m_failed_by_gate_pattern_loop;			//同じ動作を繰り返してシミュレーションが終了した回数
	//int m_failed_by_no_gate_pattern;			//歩容パターンが得られずにシミュレーションが終了した回数
	//int m_gate_pattern_generate_sum;			//全シミュレーションで出力された歩容パターンの総数

	//int m_distance_move_Y_sum;					//全シミュレーションで進んだ距離
	//int m_distance_move_Y_max;
	//int m_distance_move_Y_min;

	//double m_gate_parttern_generate_time_sum;	//全シミュレーションで歩容パターン生成にかかった時間の総和[s]
	//double m_gate_parttern_generate_time_max;
	//double m_gate_parttern_generate_time_min;
};


std::ofstream& operator<<(std::ofstream& ofs, const SSimulationRecord& record);

//! @file simulation_record.h
//! @date 2023/08/24
//! @author 長谷川
//! @brief シミュレーションの結果を記録するクラス．
//! @n 行数 : @lineinfo
