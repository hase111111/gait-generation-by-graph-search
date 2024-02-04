
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "cmdio_util.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <magic_enum.hpp>

#include "cassert_define.h"


namespace designlab
{


enums::OutputDetail CmdIOUtil::output_limit = enums::OutputDetail::kSystem;

bool CmdIOUtil::do_output = true;

bool CmdIOUtil::is_initialized = false;


void CmdIOUtil::SetOutputLimit(const enums::OutputDetail limit)
{
    output_limit = limit;

    if (!is_initialized)
    {
        // これを記述しておくと実行速度が早くなる．
        // そのかわり printf を使用できなくなる．
        std::cin.tie(&std::cout);
        std::ios_base::sync_with_stdio(true);

        is_initialized = true;
    }
}

void CmdIOUtil::DoOutput(const bool do_output_)
{
    do_output = do_output_;
}


void CmdIOUtil::Output(const std::string& str, const enums::OutputDetail detail)
{
    assert(is_initialized);  // SetOutputLimitを呼んでから使用すること.


    // 出力を許可している　かつ　出力する文字列の詳細が設定ファイルで許可されている場合
    // または，出力を許可していない　かつ　出力する文字列の詳細がシステムメッセージの場合．

    if ((detail <= output_limit && do_output) ||
        (detail == enums::OutputDetail::kSystem && !do_output))
    {
        std::cout << str << std::endl;
    }
}

void CmdIOUtil::SpacedOutput(const std::string& str, enums::OutputDetail detail)
{
    OutputNewLine(1, detail);
    Output(str, detail);
    OutputNewLine(1, detail);
}

void CmdIOUtil::OutputCenter(const std::string& str, const enums::OutputDetail detail)
{
    // 改行ごとに文字列を取り出す．
    std::stringstream ss(str);
    std::string line;

    while (std::getline(ss, line))
    {
        if (kHorizontalLineLength > line.length())
        {
            std::string space;

            for (int i = 0; i < (kHorizontalLineLength - static_cast<int>(line.length())) / 2; i++)
            {
                space += " ";
            }

            Output(space + line, detail);
        }
        else
        {
            Output(line, detail);
        }
    }
}

void CmdIOUtil::OutputRight(const std::string& str, const enums::OutputDetail detail)
{
    // 改行ごとに文字列を取り出す．
    std::stringstream ss(str);
    std::string line;

    while (std::getline(ss, line))
    {
        if (kHorizontalLineLength > line.length())
        {
            std::string space;

            for (int i = 0; i < kHorizontalLineLength - static_cast<int>(line.length()); i++)
            {
                space += " ";
            }

            Output(space + line, detail);
        }
        else
        {
            Output(line, detail);
        }
    }
}


void CmdIOUtil::OutputNewLine(const int num, const enums::OutputDetail detail)
{
    if (num <= 0) { return; }

    for (int i = 0; i < num; i++)
    {
        Output("", detail);
    }
}

void CmdIOUtil::OutputHorizontalLine(const std::string& line_visual,
                                     const enums::OutputDetail detail)
{
    if (line_visual.size() != 1) { return; }

    std::string str;

    for (int i = 0; i < kHorizontalLineLength; i++)
    {
        str += line_visual;
    }

    Output(str, detail);
}

void CmdIOUtil::OutputTitle(const std::string& title_name, bool output_copy_right)
{
    enums::OutputDetail detail = enums::OutputDetail::kSystem;

    OutputNewLine(1, detail);
    OutputHorizontalLine("=", detail);
    OutputNewLine(1, detail);
    OutputCenter(title_name, detail);
    OutputNewLine(1, detail);

    if (output_copy_right)
    {
        OutputRight("Copyright 2015 - 2023 埼玉大学 設計工学研究室  ", detail);
        OutputNewLine(1, detail);
    }

    OutputHorizontalLine("=", detail);
    OutputNewLine(1, detail);
}


void CmdIOUtil::WaitAnyKey(const std::string& str)
{
    Output(str, enums::OutputDetail::kSystem);

    // 何かキーを押すまで待機．
    system("PAUSE");
}

int CmdIOUtil::InputInt(const int min, const int max, const int default_num, const std::string& str)
{
    assert(min <= max);  // 最小値は最大値より小さい．

    Output(str + " (" + std::to_string(min) + " ～ " + std::to_string(max) + ") : ",
           enums::OutputDetail::kSystem);

    std::string input_str;
    std::cout << std::flush;
    std::cin >> input_str;

    int res = default_num;

    try
    {
        res = std::stoi(input_str);  // 入力された文字列を int 型に変換．

        if (res < min || res > max)
        {
            Output(
                "入力された値「" + input_str + "」は範囲外です．"
                "デフォルトの値，「" + std::to_string(default_num) + "」を使用します．",
                enums::OutputDetail::kSystem);

            res = default_num;
        }
    }
    catch (...)
    {
        // 整数値への変換で例外が発生した場合，ここに処理が飛ぶ．

        Output(
            "入力された値「" + input_str + "」は評価できません．"
            "デフォルトの値，「" + std::to_string(default_num) + "」を使用します．",
            enums::OutputDetail::kSystem);

        res = default_num;
    }

    return res;
}

bool CmdIOUtil::InputYesNo(const std::string& str)
{
    Output(str + " ( y / n ) ", enums::OutputDetail::kSystem);

    while (true)
    {
        std::string input_str;
        std::cout << std::flush;
        std::cin >> input_str;


        if (input_str == "y" || input_str == "yes" ||
            input_str == "Y" || input_str == "Yes" || input_str == "YES")
        {
            return true;
        }
        else if (input_str == "n" || input_str == "no" ||
                 input_str == "N" || input_str == "No" || input_str == "NO")
        {
            return false;
        }

        Output("入力された値「" + input_str + "」は評価できません．"
               "y / nで入力してください．", enums::OutputDetail::kSystem);
    }
}

std::string CmdIOUtil::InputDirName(const std::string& str)
{
    Output(str, enums::OutputDetail::kSystem);

    const std::vector<std::string> invalid_chars = { "\\", "/", ":", "*", "?", "\"", "<", ">", "|" };
    const int kMaxDirNameLength = 255;

    while (true)
    {
        std::string input_str;
        std::cout << ">>" << std::flush;
        std::cin >> input_str;

        bool is_invalid = true;

        for (const auto& invalid_char : invalid_chars)
        {
            if (input_str.find(invalid_char) != std::string::npos)
            {
                Output("ディレクトリ名には以下の文字を含めることができません．"
                       "「\\ / : * ? \" < > |」", enums::OutputDetail::kSystem);
                is_invalid = false;
                break;
            }
        }

        if (input_str.find(" ") != std::string::npos)
        {
            Output("ディレクトリ名には空白を含めることができません．", enums::OutputDetail::kSystem);
            is_invalid = false;
        }

        if (input_str.length() > kMaxDirNameLength)
        {
            Output("ディレクトリ名は" + std::to_string(kMaxDirNameLength) + "文字以下で入力してください．",
                   enums::OutputDetail::kSystem);
            is_invalid = false;
        }

        if (input_str.length() == 0)
        {
            Output("ディレクトリ名を入力してください．", enums::OutputDetail::kSystem);
            is_invalid = false;
        }

        if (is_invalid)
        {
            return input_str;
        }

        Output("ディレクトリ名を再入力してください．", enums::OutputDetail::kSystem);
    }
}

}  // namespace designlab
