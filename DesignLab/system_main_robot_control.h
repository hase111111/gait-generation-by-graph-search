
//! @file system_main_robot_control.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
#define DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_

#include <memory>
#include <string>

#include "graphic_data_broker.h"
#include "interface_system_main.h"
#include "result_file_importer.h"

namespace designlab {

class SystemMainRobotControl final : public ISystemMain {
 public:
  SystemMainRobotControl(const std::shared_ptr<GraphicDataBroker>& broker_ptr);
  ~SystemMainRobotControl() = default;

  void Main() override;

 private:
  //! @brief ノードのうち,移動しないノードを削除する.
  void RemoveDoNotMoveNode(std::vector<RobotStateNode>* graph_ptr);

  //! @brief 胴体の平行移動を連続して行うノードをまとめる.
  void MergeContinuousMove(std::vector<RobotStateNode>* graph_ptr);

  //! @brief ノードのうち,脚の上下運動を行うノードを,遊脚と接地に分ける.
  void DivideSwingAndStance(std::vector<RobotStateNode>* graph_ptr);

  //! @brief ディレクトリを作成する.
  void InitializeDirectory();

  ResultFileImporter result_importer_;

  const std::shared_ptr<GraphicDataBroker> broker_ptr_;

  const std::string kResultFileDirectoryPath{
      "robot_control"};  //!< ファイルを読み込むディレクトリのパス.
};

}  // namespace designlab

#endif  // DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
