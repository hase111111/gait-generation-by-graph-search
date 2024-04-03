
//! @file      simulation_end_checker_factory.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_
#define DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_

#include <memory>

#include "interface_simulation_end_checker.h"
#include "simulation_setting_record.h"


namespace designlab
{

//! @class SimulationEndCheckerFactory
//! @brief シミュレーションの終了を判定するクラスを生成するクラス．
class SimulationEndCheckerFactory final
{
public:
    //! @brief シミュレーションの終了を判定するクラスを生成する．
    //! @param[in] record シミュレーションの終了を判定する際の設定．
    //! @return シミュレーションの終了を判定するクラスのインスタンス．
    static std::unique_ptr<ISimulationEndChecker> Create(const SimulationSettingRecord& record);
};

}  // namespace designlab


#endif  // DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_
