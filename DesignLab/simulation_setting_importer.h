#ifndef DESIGNLAB_SIMULATION_SETTINGS_IMPORTER_H_
#define DESIGNLAB_SIMULATION_SETTINGS_IMPORTER_H_

#include <string>

#include "simulation_setting_record.h"


class SimulationSettingImporter final
{
public:

	SimulationSettingRecord ImportOrUsDefault() const;

	static const std::string kFilePath;

private:

	void OutputDefault() const;

	const std::string kFileName = "simulation_setting.toml";

};


#endif	// DESIGNLAB_SIMULATION_SETTINGS_IMPORTER_H_