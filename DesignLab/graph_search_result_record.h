
//! @file      graph_search_result_record.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_
#define DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_

#include <string>

#include <magic_enum.hpp>

#include "robot_state_node.h"


namespace designlab::enums
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

}  // namespace designlab::enums


namespace designlab
{

//! @struct GraphSearchResult
//! @brief グラフ探索の結果を表す構造体．
struct GraphSearchResult final
{
    GraphSearchResult() :
        result(enums::Result::kFailure),
        message("GraphSearchResultは初期化されていません")
    {};

    GraphSearchResult(const enums::Result result, const std::string& message) :
        result(result),
        message(message)
    {};

    enums::Result result;   //!< 成功か失敗か．
    std::string message;    //!< 成功時・失敗時のメッセージ．

    std::string ToString() const;
};


//! @struct GraphSearchResultRecord
//! @brief グラフ探索の結果を格納する構造体．
//! @note 変数をごちゃごちゃさせたくないので作成した．
struct GraphSearchResultRecord final
{
    GraphSearchResultRecord() :
        result_node{},
        computation_time(0.0),
        graph_search_result{}
    {
    }

    GraphSearchResultRecord(const RobotStateNode& node,
                            const double time, const GraphSearchResult result) :
        result_node(node),
        computation_time(time),
        graph_search_result(result)
    {
    }


    //! @brief 構造体の内容をCSV形式の文字列にして返す． , (カンマ) で区切られる．
    //! @return 構造体の内容をCSV形式の文字列にしたもの．
    std::string ToCsvString() const;

    //! @brief CSV形式のヘッダを返す．
    //! @return CSV形式のヘッダ．
    //! @todo 処理を一般化する．
    static std::string GetCsvHeader();

    RobotStateNode result_node;  //!< グラフ探索によって選択されたノード.

    double computation_time;  //!< グラフ探索にかかった計算時間 [milli sec]

    GraphSearchResult graph_search_result;  //!< グラフ探索の結果，成功か失敗か．
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_
