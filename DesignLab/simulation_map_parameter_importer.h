#ifndef DESIGNLAB_SIMULATION_MAP_PARAMETER_IMPORTER_H_
#define DESIGNLAB_SIMULATION_MAP_PARAMETER_IMPORTER_H_


#include "simulation_map_parameter.h"


class SimulationMapParameterImporter final
{
public:

	SimulationMapParameter ImportOrUseAndOutputDefault(const std::string& file_path) const;

private:

	void AskOutputDefault(const std::string& file_path) const;
};

#endif	// DESIGNLAB_SIMULATION_MAP_PARAMETER_IMPORTER_H_