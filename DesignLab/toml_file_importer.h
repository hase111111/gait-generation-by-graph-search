//! @file toml_file_importer.h
//! @brief tomlファイルを読み込んで構造体に変換するテンプレートクラス．

#ifndef DESIGNLAB_TOML_FILE_IMPORTER_H_
#define DESIGNLAB_TOML_FILE_IMPORTER_H_

#include <fstream>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "cmdio_util.h"
#include "designlab_impl.h"
#include "interface_toml_data_validator.h"
#include "toml_data_validator_always_true.h"
#include "toml_file_exporter.h"


namespace designlab
{

//! @class TomlFileImporter
//! @brief tomlファイルを読み込んで構造体に変換するテンプレートクラス．
//! @tparam T 変換先の構造体．
template <typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>&& impl::has_from_toml<T>::value> >
class TomlFileImporter final
{
public:

	TomlFileImporter() : validator_(std::make_unique<TomlDataValidatorAlwaysTrue<T>>()) {}

	TomlFileImporter(std::unique_ptr<ITomlDataValidator<T>>&& validator) : validator_(std::move(validator)) {}


	//! @brief 指定したファイルパスのファイルを読み込み，構造体に変換する．
	//! @param file_path 読み込むファイルのパス．
	//! @return std::optional<T> 読み込んだ構造体．失敗した場合はstd::nulloptを返す．
	std::optional<T> Import(const std::string& file_path) const
	{
		if (do_output_message_)
		{
			const std::string type_name = typeid(*this).name();
			::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
			::designlab::cmdio::Output("[" + type_name + "]", ::designlab::enums::OutputDetail::kSystem);
			::designlab::cmdio::Output("ファイルを読み込みます．file_path : " + file_path, ::designlab::enums::OutputDetail::kSystem);
		}

		if (!std::filesystem::exists(file_path))
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("ファイルが存在しません．", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("設定ファイルが見つかりました．パースを開始します．", ::designlab::enums::OutputDetail::kSystem); }

		toml::value toml_value;

		try
		{
			std::ifstream ifs(file_path, std::ios::binary);		//バイナリモードで読み込む

			toml_value = toml::parse(ifs, file_path);
		}
		catch (toml::syntax_error err)
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("設定ファイルのパースに失敗しました．", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::Output("<パースに失敗した箇所>", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::Output(err.what(), ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("設定ファイルのパースに成功しました．データをシリアライズします．", ::designlab::enums::OutputDetail::kSystem); }

		T data;

		try
		{
			data = toml::from<T>::from_toml(toml_value);
		}
		catch (...)
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("データのシリアライズに失敗しました．", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("データのシリアライズに成功しました．データの検証を開始します．", ::designlab::enums::OutputDetail::kSystem); }

		const auto [is_valid, error_message] = validator_->Validate(data);

		if (!is_valid)
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("データの検証に失敗しました．", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::Output("<検証に失敗した理由>", ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::Output(error_message, ::designlab::enums::OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_)
		{
			::designlab::cmdio::Output("データの検証に成功しました．", ::designlab::enums::OutputDetail::kSystem);
			::designlab::cmdio::Output("読み込みは正常に完了しました．", ::designlab::enums::OutputDetail::kSystem);
			::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);
		}

		return data;
	}

	//! @brief 指定したファイルパスのファイルを読み込み，構造体に変換する．
	//! 読込に失敗した場合は，デフォルトの構造体を返す．
	//! また，読込に失敗した場合には，デフォルトの構造体をファイルに出力するかどうかをユーザに問う．
	//! @param file_path 読み込むファイルのパス．
	//! @return T 読み込んだ構造体．
	T ImportOrUseDefault(const std::string& file_path) const
	{
		const auto data = Import(file_path);

		if (data.has_value()) { return data.value(); }

		if (::designlab::cmdio::InputYesNo("デフォルトのファイルを出力しますか"))
		{
			TomlFileExporter<T> exporter;
			exporter.Export(file_path, T());
		}

		::designlab::cmdio::Output("デフォルトのデータを使用します．", ::designlab::enums::OutputDetail::kSystem);
		::designlab::cmdio::OutputNewLine(1, ::designlab::enums::OutputDetail::kSystem);

		return T();
	}

private:

	bool do_output_message_{ true };

	const std::unique_ptr<ITomlDataValidator<T>> validator_;
};

}	//namespace designlab


#endif // DESIGNLAB_TOML_FILE_IMPORTER_H_