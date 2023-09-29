//! @file graph_search_result_recoder.h
//! @brief グラフ探索の結果を格納する構造体．


#ifndef DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_
#define DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_


#include "robot_state_node.h"


//! @enum GraphSearchReslut
//! @brief グラフ探索の結果を表す列挙型
enum class GraphSearchResult
{
	kSuccess,							//!< グラフ探索に成功した
	kFailure,							//!< グラフ探索に失敗した
	kFailureByInitializationFailed,		//!< グラフ探索を行うクラスの初期化に失敗した
	kFailureByNodeLimitExceeded,		//!< ノード数の上限に達したためグラフ探索に失敗した
	kFailureByNoNode,					//!< グラフ木を作成したが，ノードが1つも生成できなかった．
	kFailureByNotReachedDepth,			//!< グラフ木を作成したが，目標深さに到達できなかった．
	kFailureByLegPathGenerationError,	//!< 脚の軌道生成に失敗した
};


//! @struct GraphSearchResultRecoder
//! @brief グラフ探索の結果を格納する構造体．変数をごちゃごちゃさせたくないので作成
struct GraphSearchResultRecoder final
{
	GraphSearchResultRecoder() : 
		result_node{}, 
		computation_time(0.0), 
		graph_search_result(GraphSearchResult::kFailure),
		did_reevaluation(false)
	{
	};

	GraphSearchResultRecoder(const RobotStateNode& node,const double time, const GraphSearchResult result) : 
		result_node(node), 
		computation_time(time), 
		graph_search_result(result),
		did_reevaluation(false)
	{
	};


	RobotStateNode result_node;		//!< グラフ探索によって選択された動作の記録

	double computation_time;		//!< グラフ探索にかかった時間

	GraphSearchResult graph_search_result;	//!< グラフ探索の結果，成功か失敗か

	bool did_reevaluation;			//!< 再評価を行ったかどうか

};



#endif // !DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_