//! @file toml_directory_exporter.h
//! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をするクラス．

#ifndef DESIGNLAB_TOML_FILE_SETUPPER_H_
#define DESIGNLAB_TOML_FILE_SETUPPER_H_

#include <string>


namespace designlab
{

//! @class TomlDirectoryExporter
//! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をするクラス．
class TomlDirectoryExporter final
{
public:

	const static std::string kTomlFileDirPath;	//!< TOMLファイルを入れるディレクトリのパス．

	//! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をする．
	void Export();

};

}  // namespace designlab


#endif // DESIGNLAB_TOML_FILE_SETUPPER_H_