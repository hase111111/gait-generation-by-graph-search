//! @file graphic_main_graph_viewer.h
//! @brief GraphicMainGraphViewerクラス

#ifndef DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_
#define DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_

#include "interface_graphic_main.h"

#include <memory>

#include "camera_gui.h"
#include "graph_viewer_gui_controller.h"
#include "graphic_data_broker.h"
#include "hexapod_renderer.h"
#include "map_state.h"
#include "node_display_gui.h"


//! @class GraphicMainGraphViewer
//! @brief GraphViewerの画面を表示するクラス

class GraphicMainGraphViewer final : public IGraphicMain
{
public:
	GraphicMainGraphViewer(const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr, const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr);

	~GraphicMainGraphViewer() = default;

	bool Update() override;

	void Draw() const override;

private:

	std::shared_ptr<const GraphicDataBroker> broker_ptr_;

	std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;


	CameraGui camera_gui_;

	NodeDisplayGui node_display_gui_;

	std::unique_ptr<GraphViewerGUIController> gui_controller_ptr_;


	MapState map_state_;

	HexapodRenderer hexapod_renderer_;


	std::vector<SNode> graph_;

	size_t display_node_index_ = 0;


	int map_update_count_;

	int graph_update_count_;
};


#endif // !DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_