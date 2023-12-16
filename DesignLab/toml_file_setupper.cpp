#include "toml_file_setupper.h"

#include <filesystem>


namespace designlab
{

const std::string TomlFileSetupper::kTomlFileDirPath = "./simulation_condition/";


void TomlFileSetupper::Setup()
{
	// ディレクトリが存在しない場合は作成
	if (!std::filesystem::exists(kTomlFileDirPath))
	{
		std::filesystem::create_directory(kTomlFileDirPath);
	}
}

} // namespace designlab