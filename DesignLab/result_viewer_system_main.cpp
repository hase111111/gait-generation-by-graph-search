#include "result_viewer_system_main.h"

#include <fstream>

#include "basic_graphic_main_builder.h"
#include "phantomx_state_calculator.h"
#include "map_state.h"


ResultViewerSystemMain::ResultViewerSystemMain(const SApplicationSettingRecorder* const setting)
{
	MapState map;
	map.init(EMapCreateMode::FLAT, 0, false);
	m_broker.setMapState(map);

	read();

	m_graphic_system.init(std::make_unique<BasicGraphicMainBuilder>(), std::make_shared<PhantomXStateCalclator>(), &m_broker, setting);
}

void ResultViewerSystemMain::main()
{
	m_graphic_system.main();
}

void ResultViewerSystemMain::read()
{
	std::ifstream ifs("result.csv");

	if (!ifs)
	{
		return;
	}

	std::string str;

	std::getline(ifs, str);
	std::getline(ifs, str);
	std::getline(ifs, str);

	while (std::getline(ifs, str))
	{
		std::string str_conma_buf;
		std::istringstream i_stream(str);

		while (std::getline(i_stream, str_conma_buf, ','))
		{
			SNode read_node;
		}
	}
}
