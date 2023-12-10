#include "map_creator_factory.h"

#include "map_creator_by_csv.h"
#include "map_creator_for_simulation.h"
#include "simulation_setting_importer.h"
#include "simulation_map_parameter_importer.h"


std::unique_ptr<IMapCreator> MapCreatorFactory::Create(const SimulationSettingRecord& record)
{
	std::unique_ptr<IMapCreator> map_creator;

	if (record.map_create_mode == MapCreateMode::kFromFile)
	{
		map_creator = std::make_unique<MapCreatorByCsv>(SimulationSettingImporter::kFilePath + record.map_file_name);
	}
	else if (record.map_create_mode == MapCreateMode::kForSimulation)
	{
		SimulationMapParameterImporter simulation_map_parameter_importer;

		const SimulationMapParameter simulation_map_parameter = simulation_map_parameter_importer.ImportOrUseAndOutputDefault(record.simulation_map_param_file_name);

		map_creator = std::make_unique<MapCreatorForSimulation>(simulation_map_parameter);
	}

	return std::move(map_creator);
}