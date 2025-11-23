
//! @file system_main_create_map_csv.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "system_main_create_map_csv.h"

#include "cmdio_util.h"

namespace designlab {

void SystemMainCreateMapCsv::Main() {
  using enum OutputDetail;
  // コマンドラインにタイトルを表示する.
  cmdio::OutputTitle("Create Map CSV Mode");
}

}  // namespace designlab
