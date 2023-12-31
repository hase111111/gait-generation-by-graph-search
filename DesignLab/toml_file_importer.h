
//! @file      toml_file_importer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_TOML_FILE_IMPORTER_H_
#define DESIGNLAB_TOML_FILE_IMPORTER_H_

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "cmdio_util.h"
#include "implicit_metafunction_for_toml11.h"
#include "interface_toml_data_validator.h"
#include "toml_data_validator_always_true.h"
#include "toml_file_exporter.h"


namespace designlab
{

//! @class TomlFileImporter
//! @brief tomlファイルを読み込んで構造体に変換するテンプレートクラス．
//! @tparam T 変換先の構造体．デフォルトコンストラクタが実装されていること．
//! toml::from<T>()が定義されている必要がある．
template <typename T,
    typename = std::enable_if_t<
    std::is_default_constructible_v<T>&& impl::has_from_toml<T>::value>
>
class TomlFileImporter final
{
public:
    TomlFileImporter() : validator_(std::make_unique<TomlDataValidatorAlwaysTrue<T>>()) {}

    explicit TomlFileImporter(std::unique_ptr<ITomlDataValidator<T>>&& validator) :
        validator_(std::move(validator)) {}


    //! @brief 指定したファイルパスのファイルを読み込み，構造体に変換する．
    //! @param file_path 読み込むファイルのパス．
    //! @return 読み込んだ構造体．失敗した場合は std::nulloptを返す．
    std::optional<T> Import(const std::string& file_path) const
    {
        if (!FileIsExist(file_path)) { return std::nullopt; }

        toml::value toml_value;

        if (!ParseTomlFile(file_path, &toml_value)) { return std::nullopt; }

        T data;

        if (!SerializeTomlData(&toml_value, &data)) { return std::nullopt; }

        if (!ValidateData(data)) { return std::nullopt; }

        if (do_output_message_)
        {
            CmdIOUtil::Output("データの検証に成功しました．", enums::OutputDetail::kSystem);
            CmdIOUtil::Output("読み込みは正常に完了しました．", enums::OutputDetail::kSystem);
            CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
        }

        return data;
    }

    //! @brief 指定したファイルパスのファイルを読み込み，構造体に変換する．
    //! 読込に失敗した場合は，デフォルトの構造体を返す．
    //! また，読込に失敗した場合には，デフォルトの構造体をファイルに出力するかどうかをユーザに問う．
    //! @param file_path 読み込むファイルのパス．
    //! @return 読み込んだ構造体．
    T ImportOrUseDefault(const std::string& file_path) const
    {
        const auto data = Import(file_path);

        if (data.has_value()) { return data.value(); }

        if (CmdIOUtil::InputYesNo("デフォルトのファイルを出力しますか"))
        {
            TomlFileExporter<T> exporter;
            exporter.Export(file_path, T());
        }

        CmdIOUtil::Output("デフォルトのデータを使用します．", enums::OutputDetail::kSystem);
        CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);

        return T();
    }

private:
    bool FileIsExist(const std::string& file_path) const
    {
        if (do_output_message_)
        {
            CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
            CmdIOUtil::Output("[" + string_util::GetTypeName(*this) + "]",
                              enums::OutputDetail::kSystem);
            CmdIOUtil::Output("ファイルを読み込みます．file_path : " + file_path,
                              enums::OutputDetail::kSystem);
        }

        if (!std::filesystem::exists(file_path))
        {
            if (do_output_message_)
            {
                CmdIOUtil::Output("ファイルが存在しません．", enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
            }

            return false;
        }

        return true;
    }

    bool ParseTomlFile(const std::string& file_path, toml::value* toml_value) const
    {
        if (do_output_message_)
        {
            CmdIOUtil::Output("設定ファイルが見つかりました．パースを開始します．",
                              enums::OutputDetail::kSystem);
        }

        try
        {
            std::ifstream ifs(file_path, std::ios::binary);  // バイナリモードで読み込む．

            *toml_value = toml::parse(ifs, file_path);
        }
        catch (toml::syntax_error err)
        {
            if (do_output_message_)
            {
                CmdIOUtil::Output("設定ファイルのパースに失敗しました．",
                                  enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
                CmdIOUtil::Output("<パースに失敗した箇所>", enums::OutputDetail::kSystem);
                CmdIOUtil::Output(err.what(), enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
            }

            return false;
        }

        return true;
    }

    bool SerializeTomlData(toml::value* toml_value, T* data) const
    {
        if (do_output_message_)
        {
            CmdIOUtil::Output("設定ファイルのパースに成功しました．データをシリアライズします．",
                              enums::OutputDetail::kSystem);
        }

        try
        {
            *data = toml::from<T>::from_toml(*toml_value);
        }
        catch (...)
        {
            if (do_output_message_)
            {
                CmdIOUtil::Output("データのシリアライズに失敗しました．",
                                  enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
            }

            return false;
        }

        return true;
    }

    bool ValidateData(const T& data) const
    {
        if (do_output_message_)
        {
            CmdIOUtil::Output("データのシリアライズに成功しました．データの検証を開始します．",
                              enums::OutputDetail::kSystem);
        }

        const auto [is_valid, error_message] = validator_->Validate(data);

        if (!is_valid)
        {
            if (do_output_message_)
            {
                CmdIOUtil::Output("データの検証に失敗しました．", enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
                CmdIOUtil::Output("<検証に失敗した理由>", enums::OutputDetail::kSystem);
                CmdIOUtil::Output(error_message, enums::OutputDetail::kSystem);
                CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
            }

            return false;
        }

        return true;
    }

    bool do_output_message_{ true };

    const std::unique_ptr<ITomlDataValidator<T>> validator_;
};

}  // namespace designlab


#endif  // DESIGNLAB_TOML_FILE_IMPORTER_H_
