#include "simulation_record.h"

#include "leg_state.h"
#include "hexapod_const.h"


std::ofstream& operator<<(std::ofstream& ofs, const SSimulationRecord& record)
{
	const size_t kLength = record.result_nodes.size();


	for (size_t i = 0; i < kLength; i++)
	{
		ofs << i << ",";

		//ノードの状態の出力
		ofs << record.result_nodes[i] << ",";

		//計算時間の出力
		if (record.computation_time.size() > i)
		{
			ofs << record.computation_time[i] << ",";
		}
		else
		{
			ofs << ",";
		}

		//グラフ探索の結果の出力
		if (record.graph_search_results.size() > i)
		{
			ofs << std::to_string(record.graph_search_results[i]) << ",";
		}
		else
		{
			ofs << ",";
		}

		ofs << std::endl;
	}

	//最終的な結果の出力
	ofs << std::to_string(record.simulation_result) << ",";

	ofs << std::endl;

	return ofs;
}
