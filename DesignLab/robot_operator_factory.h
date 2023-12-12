﻿//! @file robot_operator_factory.h
//! @brief TargetUpdaterを生成するファクトリクラス．

#ifndef DESIGNLAB_ROBOT_OPERATOR_FACTORY_H_
#define DESIGNLAB_ROBOT_OPERATOR_FACTORY_H_

#include <memory>

#include "interface_robot_operator.h"
#include "simulation_setting_record.h"


//! @class RobotOperatorFactory
//! @brief TargetUpdaterを生成するファクトリクラス．
class RobotOperatorFactory final
{
public:

	//! @brief 設定ファイルからTargetUpdaterを生成する．
	//! @param [in] setting 設定ファイルのデータ．
	//! @return std::unique_ptr<IRobotOperator> 生成したTargetUpdater．
	static std::unique_ptr<IRobotOperator> Create(const SimulationSettingRecord& setting);
};


#endif // DESIGNLAB_ROBOT_OPERATOR_FACTORY_H_