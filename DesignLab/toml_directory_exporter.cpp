
#include "toml_directory_exporter.h"

#include <filesystem>


namespace designlab
{

const char TomlDirectoryExporter::kTomlFileDirPath[] = "./simulation_condition/";


void TomlDirectoryExporter::Export()
{
    // ディレクトリが存在しない場合は作成．
    if (!std::filesystem::exists(kTomlFileDirPath))
    {
        std::filesystem::create_directory(kTomlFileDirPath);
    }
}

}  // namespace designlab
