//! @file simulation_setting_exporter.h
//! @brief シミュレーションの設定を出力するクラス．

#ifndef DESIGNLAB_SIMULATION_SETTING_EXPORTER_H_
#define DESIGNLAB_SIMULATION_SETTING_EXPORTER_H_

#include <string>

#include "simulation_setting_record.h"


class SimulationSettingExporter final
{
public:

	//! @brief シミュレーションの設定を出力する．
	//! @param [in] record シミュレーションの設定．
	//! @param [in] file_path 出力先のファイルパス．
	void Export(const SimulationSettingRecord& record, std::string file_path) const;

};


#endif	// DESIGNLAB_SIMULATION_SETTING_EXPORTER_H_