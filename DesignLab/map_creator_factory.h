
//! @file      map_creator_factory.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_CREATOR_FACTORY_H_
#define DESIGNLAB_MAP_CREATOR_FACTORY_H_

#include <memory>

#include "interface_map_creator.h"
#include "simulation_setting_record.h"


namespace designlab
{

//! @class MapCreatorFactory
//! @brief マップを生成するクラスを生成するクラス．
class MapCreatorFactory final
{
public:
    //! @brief マップを生成するクラスを生成する．
    //! @param[in] record マップを生成する際の設定．
    //! @return マップを生成するクラスのインスタンス．
    static std::unique_ptr<IMapCreator> Create(const SimulationSettingRecord& record);
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_CREATOR_FACTORY_H_
