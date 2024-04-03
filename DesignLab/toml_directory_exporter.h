
//! @file      toml_directory_exporter.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_
#define DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_

#include <string>


namespace designlab
{

//! @class TomlDirectoryExporter
//! @brief TOMLファイルを入れるディレクトリを作成し，
//! TOMLファイル読み込みの準備をするクラス．
class TomlDirectoryExporter final
{
public:
    //! TOMLファイルを入れるディレクトリのパス．
    static const char kTomlFileDirPath[];

    //! @brief TOMLファイルを入れるディレクトリを作成し，
    //! TOMLファイル読み込みの準備をする．
    void Export();
};

}  // namespace designlab


#endif  // DESIGNLAB_TOML_DIRECTORY_EXPORTER_H_
