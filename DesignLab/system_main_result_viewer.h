
//! @file      system_main_result_viewer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_
#define DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_

#include <memory>
#include <string>

#include "application_setting_record.h"
#include "graphic_data_broker.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_system_main.h"
#include "result_file_importer.h"


namespace designlab
{

//! @class SystemMainResultViewer
//! @brief 結果を表示するシステムのクラス．
class SystemMainResultViewer final : public ISystemMain
{
public:
    SystemMainResultViewer(
      const std::shared_ptr<GraphicDataBroker>& broker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord> setting_ptr,
      const std::shared_ptr<const IHexapodJointCalculator> joint_calculator,
      const std::shared_ptr<const IHexapodCoordinateConverter> converter);


    void Main() override;

private:
    ResultFileImporter result_importer_;

    const std::shared_ptr<GraphicDataBroker> broker_ptr_;

    const std::shared_ptr<const IHexapodJointCalculator> joint_calculator_;

    const std::shared_ptr<const IHexapodCoordinateConverter> converter_;

    void OutputErrorLegPos(const std::string& file, const std::vector<RobotStateNode>& nodes);
};

}  // namespace designlab


#endif  // DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_
