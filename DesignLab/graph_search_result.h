#pragma once

#include <string>


//! @enum GraphSearchReslut
//! @brief グラフ探索の結果を表す列挙型
enum class GraphSearchResult
{
	Success,						//!< グラフ探索に成功した
	Failure,						//!< グラフ探索に失敗した
	FailureByInitializationFailed,	//!< グラフ探索を行うクラスの初期化に失敗した
	FailureByNodeLimitExceeded,		//!< ノード数の上限に達したためグラフ探索に失敗した
	FailureByNoNode,				//!< グラフ木を作成したが，ノードが1つも生成できなかった．
	FailureByNotReachedDepth,		//!< グラフ木を作成したが，目標深さに到達できなかった．
	FailureByLegPathGenerationError,//!< 脚の軌道生成に失敗した

	SuccessByReevaluation,			//!< 再評価によりグラフ探索に成功した	
	FailureByReevaluation,			//!< 再評価を行ったが，グラフ探索に失敗した
	FailureByReevaluationAndNodeLimitExceeded,		//!< 再評価を行ったが，ノード数の上限に達したためグラフ探索に失敗した
	FailureByReevaluationAndNoNode,					//!< 再評価を行ったが，ノードが1つも生成できなかった．
	FailureByReevaluationAndNotReachedDepth,		//!< 再評価を行ったが，目標深さに到達できなかった．
	FailureByReevaluationAndLegPathGenerationError,	//!< 再評価を行ったが，脚の軌道生成に失敗した
};


//! @brief グラフ探索が成功したかどうかを返す関数
//! @param [in] result グラフ探索の結果
//! @return bool 成功でtrue，失敗でfalse
inline bool graphSeachResultIsSuccessful(const GraphSearchResult result)
{
	return result == GraphSearchResult::Success || result == GraphSearchResult::SuccessByReevaluation;
}