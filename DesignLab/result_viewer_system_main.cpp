#include "result_viewer_system_main.h"

#include <fstream>

#include "phantomx_state_calculator.h"
#include "map_state.h"


ResultViewerSystemMain::ResultViewerSystemMain(const std::shared_ptr<const SApplicationSettingRecorder> setting_ptr)
{
}

void ResultViewerSystemMain::Main()
{
}

void ResultViewerSystemMain::Read()
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
