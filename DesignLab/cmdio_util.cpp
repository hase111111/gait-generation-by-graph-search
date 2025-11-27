
//! @file cmdio_util.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "cmdio_util.h"

#include <cstdlib>
#include <iostream>
#include <magic_enum.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "cassert_define.h"
#include "string_util.h"

namespace designlab {

OutputDetail output_limit = OutputDetail::kSystem;

bool do_output = true;

bool is_initialized = false;

void cmdio::SetOutputLimit(const OutputDetail limit) {
  output_limit = limit;

  if (!is_initialized) {
    // これを記述しておくと実行速度が早くなる.
    // そのかわり printf を使用できなくなる.
    std::cin.tie(&std::cout);
    std::ios_base::sync_with_stdio(true);

    is_initialized = true;
  }

  // 出力の許可範囲を設定したことを通知.
  OutputF(OutputDetail::kSystem, "Output limit is set to '{}'.",
          string_util::EnumToStringRemoveTopK(output_limit));
}

void cmdio::DoOutput(const bool do_output_) { do_output = do_output_; }

void cmdio::Output(const std::string& str, const OutputDetail detail) {
  assert(is_initialized);  // SetOutputLimitを呼んでから使用すること.

  // 出力を許可している かつ
  // 出力する文字列の詳細が設定ファイルで許可されている場合
  // または,出力を許可していない　かつ
  // 出力する文字列の詳細がシステムメッセージの場合.

  if ((detail <= output_limit && do_output) ||
      (detail == OutputDetail::kSystem && !do_output)) {
    if (str == "") {
      std::cout << std::endl;
      return;
    }

    // システムメッセージでない場合は,タグをつける.
    const std::string tag =
        (detail == OutputDetail::kSystem
             ? ""
             : " [" + string_util::EnumToStringRemoveTopK(detail) + "] ");

#if defined(DESIGNLAB_USE_COLOR_OUTPUT)

    if (detail == OutputDetail::kError) {
      // 赤色.
      std::cout << "\x1b[31m";
    } else if (detail == OutputDetail::kWarning) {
      // 黄色.
      std::cout << "\x1b[33m";
    } else if (detail == OutputDetail::kInfo) {
      // シアン.
      std::cout << "\x1b[36m";
    } else if (detail == OutputDetail::kDebug) {
      // 緑色.
      std::cout << "\x1b[32m";
    } else {
      // デフォルトの色.
      std::cout << "\x1b[0m";
    }

#endif  // DESIGNLAB_USE_COLOR_OUTPUT

    std::cout << tag;

    // タグと同じ長さの空白を出力する.
    const std::string space(tag.size(), ' ');

    // 改行ごとに文字列を取り出す.
    const auto line = string_util::Split(str, "\n");

    for (size_t i = 0; i < line.size(); i++) {
      if (i != 0) {
        std::cout << space;
      }

      if (detail != OutputDetail::kSystem) {
        std::cout << "| ";
      }

      std::cout << line[i] << std::endl;
    }

#if defined(DESIGNLAB_USE_COLOR_OUTPUT)
    std::cout << "\x1b[0m";  // 色をリセット.
#endif                       // DESIGNLAB_USE_COLOR_OUTPUT
  }
}

void cmdio::SpacedOutput(const std::string& str, OutputDetail detail) {
  OutputNewLine(1, detail);
  Output(str, detail);
  OutputNewLine(1, detail);
}

void cmdio::OutputCenter(const std::string& str, const OutputDetail detail) {
  // 改行ごとに文字列を取り出す.
  std::stringstream ss(str);
  std::string line;

  while (std::getline(ss, line)) {
    if (kHorizontalLineLength > line.length()) {
      std::string space;

      const int space_num =
          (kHorizontalLineLength - static_cast<int>(line.length())) / 2;

      for (int i = 0; i < space_num; ++i) {
        space += " ";
      }

      Output(space + line, detail);
    } else {
      Output(line, detail);
    }
  }
}

void cmdio::OutputRight(const std::string& str, const OutputDetail detail) {
  // 改行ごとに文字列を取り出す.
  std::stringstream ss(str);
  std::string line;

  while (std::getline(ss, line)) {
    if (kHorizontalLineLength > line.length()) {
      std::string space;

      const int space_num =
          kHorizontalLineLength - static_cast<int>(line.length());

      for (int i = 0; i < space_num; ++i) {
        space += " ";
      }

      Output(space + line, detail);
    } else {
      Output(line, detail);
    }
  }
}

void cmdio::OutputNewLine(const int num, const OutputDetail detail) {
  if (num <= 0) {
    return;
  }

  for (int i = 0; i < num; i++) {
    Output("", detail);
  }
}

void cmdio::OutputHorizontalLine(const std::string& line_visual,
                                 const OutputDetail detail) {
  if (line_visual.size() != 1) {
    return;
  }

  std::string str;

  for (int i = 0; i < kHorizontalLineLength; i++) {
    str += line_visual;
  }

  Output(str, detail);
}

void cmdio::OutputTitle(const std::string& title_name, bool output_copy_right) {
  OutputDetail detail = OutputDetail::kSystem;

  OutputNewLine(1, detail);
  OutputHorizontalLine("=", detail);
  OutputNewLine(1, detail);
  OutputCenter(title_name, detail);
  OutputNewLine(1, detail);

  if (output_copy_right) {
    OutputRight("(C) 2023 Design Engineering Laboratory  ", detail);
    OutputNewLine(1, detail);
  }

  OutputHorizontalLine("=", detail);
  OutputNewLine(1, detail);
}

void cmdio::WaitAnyKey(const std::string& str) {
  Output(str, OutputDetail::kSystem);

  // 何かキーを押すまで待機.
  system("PAUSE");
}

int cmdio::InputInt(const int min, const int max, const int default_num,
                    const std::string& str) {
  assert(min <= max);  // 最小値は最大値より小さい.

  OutputF(OutputDetail::kSystem, "{} ( {} ～ {} ) ", str, min, max);

  std::string input_str;
  std::cout << ">> " << std::flush;
  std::cin >> input_str;

  int res = default_num;

  try {
    res = std::stoi(input_str);  // 入力された文字列を int 型に変換.

    if (res < min || res > max) {
      OutputF(OutputDetail::kSystem,
              "The entered value '{}' is out of range. Use the default "
              "value, '{}'.",
              input_str, default_num);

      res = default_num;
    }
  } catch (...) {
    // 整数値への変換で例外が発生した場合,ここに処理が飛ぶ.
    OutputF(OutputDetail::kSystem,
            "The entered value '{}' cannot be evaluated. Use the default "
            "value, '{}'.",
            input_str, default_num);

    res = default_num;
  }

  return res;
}

bool cmdio::InputYesNo(const std::string& str) {
  Output(str + " ( y / n ) ", OutputDetail::kSystem);

  while (true) {
    std::string input_str;
    std::cout << ">> " << std::flush;
    std::cin >> input_str;

    if (input_str == "y" || input_str == "yes" || input_str == "Y" ||
        input_str == "Yes" || input_str == "YES") {
      return true;
    } else if (input_str == "n" || input_str == "no" || input_str == "N" ||
               input_str == "No" || input_str == "NO") {
      return false;
    }

    OutputF(OutputDetail::kSystem,
            "The entered value '{}' cannot be evaluated. Enter 'y' or 'n'.",
            input_str);
  }
}

std::string cmdio::InputDirName(const std::string& str) {
  Output(str, OutputDetail::kSystem);

  const std::vector<std::string> invalid_chars = {"\\", "/", ":", "*", "?",
                                                  "\"", "<", ">", "|"};
  constexpr int kMaxDirNameLength = 255;

  while (true) {
    std::string input_str;
    std::cout << ">> " << std::flush;
    std::cin >> input_str;

    bool is_invalid = true;

    for (const auto& invalid_char : invalid_chars) {
      if (input_str.find(invalid_char) != std::string::npos) {
        SystemOutput(
            "Directory names cannot contain the following characters : \\ / : "
            "* ? \" < > |");
        is_invalid = false;
        break;
      }
    }

    if (input_str.find(" ") != std::string::npos) {
      SystemOutput("Directory names cannot contain spaces.");
      is_invalid = false;
    }

    if (input_str.length() > kMaxDirNameLength) {
      OutputF(OutputDetail::kSystem,
              "The entered value '{}' is too long. Enter a value of {} "
              "characters or less.",
              input_str, kMaxDirNameLength);
      is_invalid = false;
    }

    if (input_str.length() == 0) {
      SystemOutput("Directory names cannot be empty.");
      is_invalid = false;
    }

    if (is_invalid) {
      return input_str;
    }

    SystemOutput("Retype the directory name.");
  }
}

}  // namespace designlab
