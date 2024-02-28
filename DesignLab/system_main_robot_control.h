
//! @file      system_main_robot_control.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
#define DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_

#include <string>
#include <memory>

#include "graphic_data_broker.h"
#include "interface_system_main.h"
#include "result_file_importer.h"


namespace designlab
{

class SystemMainRobotControl final : public ISystemMain
{
public:
    SystemMainRobotControl(const std::shared_ptr<GraphicDataBroker>& broker_ptr);
    ~SystemMainRobotControl() = default;

    void Main() override;

private:

    //! @brief ノードのうち，移動しないノードを削除する．
    void RemoveDoNotMoveNode(std::vector<RobotStateNode>* graph_ptr);

    //! @brief 胴体の平行移動を連続して行うノードをまとめる．
    void MergeContinuousMove(std::vector<RobotStateNode>* graph_ptr);

    //! @brief ノードのうち，脚の上下運動を行うノードを，遊脚と接地に分ける．
    void DivideSwingAndStance(std::vector<RobotStateNode>* graph_ptr);

    ResultFileImporter result_importer_;

    const std::shared_ptr<GraphicDataBroker> broker_ptr_;

    const std::string kResultFileDirectoryPath{ "robot_control" };  //!< ファイルを読み込むディレクトリのパス．
};

}  // namespace designlab

#endif  // DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
