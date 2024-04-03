
//! @file      toml_file_exporter.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_TOML_FILE_EXPORTER_H_
#define DESIGNLAB_TOML_FILE_EXPORTER_H_

#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "cmdio_util.h"
#include "implicit_metafunction_for_toml11.h"
#include "string_util.h"
#include "toml11_define.h"


namespace designlab
{

//! @brief into<T>を持つか判定するコンセプト．
template <typename T>
concept HasIntoToml = impl::has_into_toml<T>::value && std::is_default_constructible_v<T>;


//! @class TomlFileExporter
//! @brief TOMLファイルを出力するテンプレートクラス．
//! @tparam T 出力するデータの型．
//! 条件として，デフォルトコンストラクタを持つことと，
//! toml::into<T>()が定義されていることが必要．
template <HasIntoToml T>
class TomlFileExporter final
{
public:
    //! @brief TOMLファイルを出力する．
    //! @param[in] file_path 出力するファイルのパス．
    //! @param[in] data 出力するデータ．
    void Export(const std::string& file_path, const T& data)
    {
        const toml::basic_value<toml::preserve_comments, std::map> value(data);
        std::string res_str = toml::format(value);  // 設定を文字列に変換．

        InsertNewLine(&res_str);  // #をみたら，改行を挿入する．

        IndentTable(&res_str);  // Tableの中身をインデントする．

        std::ofstream ofs;
        ofs.open(file_path);

        // ファイルが開けなかったら何もしない．
        if (!ofs)
        {
            CmdIOUtil::Output("Failed to output TOML file. file_path : " + file_path, OutputDetail::kSystem);
            return;
        }

        ofs.write(res_str.c_str(), res_str.length());  // ファイルに書き込む．

        ofs.close();  // ファイルを閉じる．

        CmdIOUtil::Output("TOML files are output. file_path : " + file_path, OutputDetail::kSystem);
    }

private:
    //! @brief @#をみたら，改行を挿入する．
    void InsertNewLine(std::string* str)
    {
        if (str == nullptr)
        {
            return;
        }

        std::vector<std::string> split_str = string_util::Split((*str), "\n");

        std::string res_str;
        char past_first_char = ' ';

        for (const auto& s : split_str)
        {
            if (s.size() != 0 &&
                s[0] == '#' &&
                past_first_char != '#' &&
                past_first_char != ' ')
            {
                res_str += "\n";
            }

            past_first_char = s.size() != 0 ? s[0] : ' ';

            res_str += s + "\n";
        }

        *str = res_str;
    }

    //! @brief Tableの中身をインデントする．
    //! @param[in,out] str インデントする文字列．
    void IndentTable(std::string* str)
    {
        if (str == nullptr)
        {
            return;
        }

        std::vector<std::string> split_str = string_util::Split((*str), "\n");

        std::string res_str;
        bool do_indent = false;

        for (size_t i = 0; i < split_str.size(); i++)
        {
            std::string past_s = i != 0 ? split_str[i - 1] : "";
            std::string next_s = i != split_str.size() - 1 ? split_str[i + 1] : "";
            std::string s = split_str[i];

            if (past_s.size() != 0 && past_s[0] == '[')
            {
                do_indent = true;
            }

            if (next_s.size() != 0 && next_s[0] == '[')
            {
                do_indent = false;
            }

            if (do_indent)
            {
                res_str += "    ";
            }

            res_str += s + "\n";
        }

        *str = res_str;
    }
};

}  // namespace designlab


#endif  // DESIGNLAB_TOML_FILE_EXPORTER_H_
