#include "robot_operator_factory.h"

#include "cassert_define.h"
#include "robot_operation.h"
#include "robot_operator_for_gpg.h"
#include "robot_operator_fixed.h"
#include "robot_operator_free.h"
#include "toml_file_importer.h"
#include "toml_file_setupper.h"

namespace dle = ::designlab::enums;


std::unique_ptr<IRobotOperator> RobotOperatorFactory::Create(const SimulationSettingRecord& setting)
{
	if (setting.operate_mode == dle::RobotOperateMode::kForGloabalPathGenerate)
	{
		return std::make_unique<RobotOperatorForGpg>();
	}
	else if (setting.operate_mode == dle::RobotOperateMode::kFixed)
	{
		TomlFileImporter<RobotOperation> importer;
		return std::make_unique<RobotOperatorFixed>(importer.ImportOrUseDefault(TomlFileSetupper::kTomlFileDirPath + setting.fixed_operate_file_name));
	}
	else if (setting.operate_mode == dle::RobotOperateMode::kFree)
	{
		return std::make_unique<RobotOperatorFree>();
	}
	else
	{
		assert(false);
		return nullptr;
	}
}