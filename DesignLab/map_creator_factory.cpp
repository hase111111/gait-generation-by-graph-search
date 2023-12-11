#include "map_creator_factory.h"

#include <memory>

#include "map_creator_by_csv.h"
#include "map_creator_for_simulation.h"
#include "toml_file_importer.h"
#include "toml_file_setupper.h"

namespace dle = ::designlab::enums;


std::unique_ptr<IMapCreator> MapCreatorFactory::Create(const SimulationSettingRecord& record)
{
	std::unique_ptr<IMapCreator> map_creator;

	if (record.map_create_mode == dle::MapCreateMode::kFromFile)
	{
		map_creator = std::make_unique<MapCreatorByCsv>(TomlFileSetupper::kTomlFileDirPath + record.map_file_name);
	}
	else if (record.map_create_mode == dle::MapCreateMode::kForSimulation)
	{
		TomlFileImporter<SimulationMapParameter> simulation_map_parameter_importer;

		const SimulationMapParameter simulation_map_parameter = simulation_map_parameter_importer.ImportOrUseDefault(TomlFileSetupper::kTomlFileDirPath + record.simulation_map_param_file_name);

		map_creator = std::make_unique<MapCreatorForSimulation>(simulation_map_parameter);
	}

	return std::move(map_creator);
}