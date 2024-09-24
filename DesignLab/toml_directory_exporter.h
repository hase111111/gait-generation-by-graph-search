
//! @file toml_directory_exporter.h

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

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
