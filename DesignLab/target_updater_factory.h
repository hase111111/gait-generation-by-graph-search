//! @file target_updater_factory.h
//! @brief TargetUpdaterを生成するファクトリクラス．

#ifndef DESIGNLAB_TARGET_UPDATER_FACTORY_H_
#define DESIGNLAB_TARGET_UPDATER_FACTORY_H_

#include <memory>

#include "interface_target_updater.h"
#include "simulation_setting_record.h"


//! @class TargetUpdaterFactory
//! @brief TargetUpdaterを生成するファクトリクラス．
class TargetUpdaterFactory final
{
public:

	//! @brief 設定ファイルからTargetUpdaterを生成する．
	//! @param [in] setting 設定ファイルのデータ．
	//! @return std::unique_ptr<ITargetUpdater> 生成したTargetUpdater．
	static std::unique_ptr<ITargetUpdater> Create(const SimulationSettingRecord& setting);
};


#endif // DESIGNLAB_TARGET_UPDATER_FACTORY_H_