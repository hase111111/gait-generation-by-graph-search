//! @file graph_search_result_recoder.h
//! @brief グラフ探索の結果を格納する構造体．

#ifndef DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_
#define DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_

#include <string>

#include <magic_enum.hpp>

#include "robot_state_node.h"


namespace designlab
{
	namespace enums
	{
		//! @enum Result
		//! @brief 成功か失敗かを表す列挙型．
		//! @details この列挙型は，関数の戻り値として使うことを想定している．
		//! 関数の戻り値として使うときに，bool型よりも意図が明確になると思ったため作成した．
		enum class Result : int
		{
			kSuccess,
			kFailure
		};
	}
}


//! @enum GraphSearchReslut
//! @brief グラフ探索の結果を表す列挙型
struct GraphSearchResult final
{
	GraphSearchResult() :
		result(designlab::enums::Result::kFailure),
		message("GraphSearchResultは初期化されていません")
	{};

	GraphSearchResult(const designlab::enums::Result result, const std::string& message) :
		result(result),
		message(message)
	{};

	::designlab::enums::Result result;	//!< 成功か失敗か
	std::string message;				//!< 成功時・失敗時のメッセージ

	std::string ToString() const;
};


//! @struct GraphSearchResultRecoder
//! @brief グラフ探索の結果を格納する構造体．変数をごちゃごちゃさせたくないので作成した．
struct GraphSearchResultRecoder final
{
	GraphSearchResultRecoder() :
		result_node{},
		computation_time(0.0),
		graph_search_result{}
	{
	};

	GraphSearchResultRecoder(const RobotStateNode& node, const double time, const GraphSearchResult result) :
		result_node(node),
		computation_time(time),
		graph_search_result(result)
	{
	};


	//! @brief 構造体の内容をCSV形式の文字列にして返す． , (カンマ) で区切られる．
	//! @return 構造体の内容をCSV形式の文字列にしたもの
	std::string ToCsvString() const;

	//! @brief CSV形式のヘッダを返す．
	//! @return CSV形式のヘッダ
	static std::string GetCsvHeader();

	RobotStateNode result_node;		//!< グラフ探索によって選択されたノード

	double computation_time;		//!< グラフ探索にかかった計算時間 [msec]

	GraphSearchResult graph_search_result;	//!< グラフ探索の結果，成功か失敗か

};


#endif	// DESIGNLAB_GRAPH_SEARCH_RESULT_RECODER_H_