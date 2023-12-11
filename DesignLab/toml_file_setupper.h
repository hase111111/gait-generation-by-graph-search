//! @file toml_file_setupper.h
//! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をするクラス．


#ifndef DESIGNLAB_TOML_FILE_SETUPPER_H_
#define DESIGNLAB_TOML_FILE_SETUPPER_H_

#include <string>


class TomlFileSetupper final
{
public:

	const static std::string kTomlFileDirPath;

	void Setup();

};

#endif // DESIGNLAB_TOML_FILE_SETUPPER_H_