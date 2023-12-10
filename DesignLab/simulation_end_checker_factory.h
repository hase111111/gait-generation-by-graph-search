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
	//! @param [in] record シミュレーションの終了を判定する際の設定．
	//! @return  std::unique_ptr<ISimulationEndChecker> シミュレーションの終了を判定するクラスのインスタンス．
	static std::unique_ptr<ISimulationEndChecker> Create(const SimulationSettingRecord& record);
};


#endif	// DESIGNLAB_SIMULATION_END_CHECKER_FACTORY_H_