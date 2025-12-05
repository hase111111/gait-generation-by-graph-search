
//! @file output_detail.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

namespace gaitgen {

//! @enum OutputDetail
//! @brief コマンドラインに文字を出力する際に,その詳細を指定するための列挙体.
//! @details 設定ファイルで指定されたもの以上のメッセージのみ出力する.
enum class OutputDetail : int {
  kSystem = 0,  //!< システムメッセージ,常に出力する.
  kError,       //!< エラーメッセージ.
  kWarning,     //!< 警告メッセージ,エラーではないが注意が必要なメッセージ.
  kInfo,        //!< 優先度低めの情報.
  kDebug,       //!< デバッグ時のみ出力,一番優先度が低い.
};

}  // namespace gaitgen
