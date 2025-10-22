
//! @file graph_search_result_record.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_
#define DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_

#include <magic_enum.hpp>
#include <string>

#include "my_expected.h"
#include "robot_state_node.h"

namespace designlab {

//! @struct GraphSearchResultRecord
//! @brief グラフ探索の結果を格納する構造体.
//! @note 変数をごちゃごちゃさせたくないので作成した.
struct GraphSearchResultRecord final {
  GraphSearchResultRecord()
      : result_node{},
        computation_time(0.0),
        node_expanded_count(0),
        graph_search_result{} {}

  GraphSearchResultRecord(const RobotStateNode& node, const double time,
                          const int count,
                          const nostd::expected<bool, std::string> result)
      : result_node(node),
        computation_time(time),
        node_expanded_count(count),
        graph_search_result{result} {}

  //! @brief 構造体の内容をCSV形式の文字列にして返す. , (カンマ) で区切られる.
  //! @return 構造体の内容をCSV形式の文字列にしたもの.
  std::string ToCsvString() const;

  //! @brief CSV形式のヘッダを返す.
  //! @return CSV形式のヘッダ.
  //! @todo 処理を一般化する.
  static std::string GetCsvHeader();

  RobotStateNode result_node;  //!< グラフ探索によって選択されたノード.

  double computation_time;  //!< グラフ探索にかかった計算時間 [milli sec]

  int node_expanded_count;  //!< 展開したノード数.

  //! @brief グラフ探索の結果,成功か失敗か.
  //! @todo expected<void, std::string> にする
  nostd::expected<bool, std::string> graph_search_result;
};

}  // namespace designlab

#endif  // DESIGNLAB_GRAPH_SEARCH_RESULT_RECORD_H_
