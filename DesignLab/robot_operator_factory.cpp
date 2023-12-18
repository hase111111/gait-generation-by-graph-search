#include "robot_operator_factory.h"

#include "cassert_define.h"
#include "robot_operation.h"
#include "robot_operator_for_gpg.h"
#include "robot_operator_fixed.h"
#include "robot_operator_for_path.h"
#include "toml_file_importer.h"
#include "toml_directory_exporter.h"


namespace designlab
{

std::unique_ptr<IRobotOperator> RobotOperatorFactory::Create(const SimulationSettingRecord& setting)
{
	if (setting.operate_mode == enums::RobotOperateMode::kForGloabalPathGenerate)
	{
		return std::make_unique<RobotOperatorForGpg>();
	}
	else if (setting.operate_mode == enums::RobotOperateMode::kFixed)
	{
		TomlFileImporter<RobotOperation> importer;
		return std::make_unique<RobotOperatorFixed>(importer.ImportOrUseDefault(TomlDirectoryExporter::kTomlFileDirPath + setting.fixed_operate_file_name));
	}
	else if (setting.operate_mode == enums::RobotOperateMode::kForPath)
	{
		return std::make_unique<RobotOperatorForPath>();
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

} // namespace designlab