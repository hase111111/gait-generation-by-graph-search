//! @file toml_file_exporter.h
//! @brief TOMLファイルを出力するテンプレートクラス．

#ifndef DESIGNLAB_TOML_FILE_EXPORTER_H_
#define DESIGNLAB_TOML_FILE_EXPORTER_H_

#include <fstream>
#include <filesystem>
#include <map>

#include "cmdio_util.h"
#include "toml11_define.h"


template <typename T, typename = void>
struct has_into_toml : std::false_type {};

template <typename T>
struct has_into_toml<T, std::void_t<decltype(toml::into<T>())> > : std::true_type {};


template <typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>&& has_into_toml<T>::value> >
class TomlFileExporter final
{
public:

	void Export(const std::string& file_path, const T& data)
	{
		const toml::basic_value<toml::preserve_comments, std::map> value(data);
		const std::string res_str = toml::format(value);	// 設定を文字列に変換

		std::ofstream ofs;
		ofs.open(file_path);

		// ファイルが開けなかったら何もしない
		if (!ofs)
		{
			::designlab::cmdio::Output("TOMLファイルの出力に失敗しました．file_path : " + file_path, ::designlab::enums::OutputDetail::kSystem);
			return;
		}

		ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

		ofs.close();	// ファイルを閉じる

		::designlab::cmdio::Output("TOMLファイルを出力しました．file_path : " + file_path, ::designlab::enums::OutputDetail::kSystem);
	}
};


#endif // DESIGNLAB_TOML_FILE_EXPORTER_H_