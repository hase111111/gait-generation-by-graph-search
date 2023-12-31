
//! @file      toml_directory_exporter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_
#define DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_

#include <string>


namespace designlab
{

//! @class TomlDirectoryExporter
//! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をするクラス．
class TomlDirectoryExporter final
{
public:
    static const char kTomlFileDirPath[];  //!< TOMLファイルを入れるディレクトリのパス．

    //! @brief TOMLファイルを入れるディレクトリを作成し，TOMLファイル読み込みの準備をする．
    void Export();
};

}  // namespace designlab


#endif  // DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_
