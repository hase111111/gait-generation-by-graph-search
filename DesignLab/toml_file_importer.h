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
#include "interface_toml_data_validator.h"
#include "toml11_define.h"
#include "toml_data_validator_always_true.h"
#include "toml_file_exporter.h"


template <typename T, typename = void>
struct has_from_toml : std::false_type {};

template <typename T>
struct has_from_toml<T, std::void_t<decltype(toml::from<T>())> > : std::true_type {};


//Tはデフォルトコンストラクタを持っている必要がある．
template <typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>&& has_from_toml<T>::value> >
class TomlFileImporter final
{
public:

	TomlFileImporter() : validator_(std::make_unique<TomlDataValidatorAlwaysTrue<T>>()) {}

	TomlFileImporter(std::unique_ptr<ITomlDataValidator<T>>&& validator) : validator_(std::move(validator)) {}


	std::optional<T> Import(const std::string& file_path) const
	{
		if (do_output_message_)
		{
			const std::string type_name = typeid(*this).name();
			::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
			::designlab::cmdio::Output("[" + type_name + "]", OutputDetail::kSystem);
			::designlab::cmdio::Output("ファイルを読み込みます．file_path : " + file_path, OutputDetail::kSystem);
		}

		if (!std::filesystem::exists(file_path))
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("ファイルが存在しません．", OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("設定ファイルが見つかりました．パースを開始します．", OutputDetail::kSystem); }

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
				::designlab::cmdio::Output("設定ファイルのパースに失敗しました．", OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
				::designlab::cmdio::Output("<パースに失敗した箇所>", OutputDetail::kSystem);
				::designlab::cmdio::Output(err.what(), OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("設定ファイルのパースに成功しました．データをシリアライズします．", OutputDetail::kSystem); }

		T data;

		try
		{
			data = toml::from<T>::from_toml(toml_value);
		}
		catch (...)
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("データのシリアライズに失敗しました．", OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_) { ::designlab::cmdio::Output("データのシリアライズに成功しました．データの検証を開始します．", OutputDetail::kSystem); }

		const auto [is_valid, error_message] = validator_->Validate(data);

		if (!is_valid)
		{
			if (do_output_message_)
			{
				::designlab::cmdio::Output("データの検証に失敗しました．", OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
				::designlab::cmdio::Output("<検証に失敗した理由>", OutputDetail::kSystem);
				::designlab::cmdio::Output(error_message, OutputDetail::kSystem);
				::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
			}

			return std::nullopt;
		}

		if (do_output_message_)
		{
			::designlab::cmdio::Output("データの検証に成功しました．", OutputDetail::kSystem);
			::designlab::cmdio::Output("読み込みは正常に完了しました．", OutputDetail::kSystem);
			::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);
		}

		return data;
	}

	T ImportOrUseDefault(const std::string& file_path) const
	{
		const auto data = Import(file_path);

		if (data.has_value()) { return data.value(); }

		if (::designlab::cmdio::InputYesNo("デフォルトのファイルを出力しますか"))
		{
			TomlFileExporter<T> exporter;
			exporter.Export(file_path, T());
		}

		::designlab::cmdio::Output("デフォルトのデータを使用します．", OutputDetail::kSystem);
		::designlab::cmdio::OutputNewLine(1, OutputDetail::kSystem);

		return T();
	}

private:

	bool do_output_message_{ true };

	const std::unique_ptr<ITomlDataValidator<T>> validator_;
};


#endif // DESIGNLAB_TOML_FILE_IMPORTER_H_