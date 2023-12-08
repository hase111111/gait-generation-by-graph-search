//! @file simulation_end_checker_factory.h
//! @brief シミュレーションの終了を判定するクラスを生成するクラス．


#ifndef DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_
#define DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_

#include <memory>

#include "interface_simulation_end_checker.h"
#include "simulation_setting_record.h"


class SimulationEndCheckerFactory final
{
public:

	//! @brief シミュレーションの終了を判定するクラスを生成する．
	//! @param[in] type 判定する条件の種類．
	//! @return シミュレーションの終了を判定するクラスのインスタンス．
	static std::unique_ptr<ISimulationEndChecker> Create(const std::shared_ptr<const SimulationSettingRecord>& record);
};


#endif	// DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_