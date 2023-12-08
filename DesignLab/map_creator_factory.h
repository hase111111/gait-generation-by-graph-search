//! @file map_creator_factory.h
//! @brief マップを生成するクラスを生成するクラス．

#ifndef DESIGNLAB_MAP_CREATOR_FACTORY_H_
#define DESIGNLAB_MAP_CREATOR_FACTORY_H_

#include "interface_map_creator.h"
#include "simulation_setting_record.h"

#include <memory>


//! @class MapCreatorFactory
//! @brief マップを生成するクラスを生成するクラス．
class MapCreatorFactory final
{
public:

	//! @brief マップを生成するクラスを生成する．
	//! @param [in] record マップを生成する際の設定．
	//! @return std::unique_ptr<IMapCreator> マップを生成するクラスのインスタンス．
	static std::unique_ptr<IMapCreator> Create(const std::shared_ptr<const SimulationSettingRecord>& record);
};


#endif  // DESIGNLAB_MAP_CREATOR_FACTORY_H_