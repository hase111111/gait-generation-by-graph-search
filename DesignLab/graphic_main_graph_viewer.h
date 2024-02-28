
//! @file      graphic_main_graph_viewer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_
#define DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_

#include <memory>
#include <vector>

#include "dxlib_3d_renderer_group.h"
#include "dxlib_gui_updater.h"
#include "dxlib_node_setter_group.h"
#include "graph_viewer_gui_controller.h"
#include "graphic_data_broker.h"
#include "interface_graphic_main.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "mouse.h"


namespace designlab
{

//! @class GraphicMainGraphViewer
//! @brief GraphViewerの画面を表示するクラス．
class GraphicMainGraphViewer final : public IGraphicMain
{
public:
    GraphicMainGraphViewer(
      const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

    ~GraphicMainGraphViewer() = default;

    bool Update() override;

    void Draw() const override;

private:
    const std::shared_ptr<const GraphicDataBroker> broker_ptr_;
    const std::shared_ptr<Mouse> mouse_ptr_;

    const std::unique_ptr<GraphViewerGUIController> gui_controller_ptr_;

    Dxlib3dRendererGroup renderer_group_;
    DxlibGuiUpdater gui_updater_;
    DxlibNodeSetterGroup node_setter_group_;

    std::vector<RobotStateNode> graph_;
    size_t display_node_index_{ 0 };

    int graph_update_count_{ 0 };
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPHIC_MAIN_GRAPH_VIEWER_H_
