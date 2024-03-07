
//! @file      dxlib_gui_robot_control.h
//! @author    hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_ROBOT_CONTROL_H_
#define DESIGNLAB_DXLIB_GUI_ROBOT_CONTROL_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/thread.hpp>

#include "abstract_dxlib_gui.h"
#include "dxlib_camera.h"
#include "math_vector3.h"
#include "interface_dxlib_node_setter.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "serial_communication_thread.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiRobotControl
//! @brief カメラの操作・管理を行うGUIの処理・描画を行うクラス．
class DxlibGuiRobotControl final :
    public AbstractDxlibGui,
    public IDxlibNodeSetter
{
public:
    DxlibGuiRobotControl() = delete;  //!< デフォルトコンストラクタは生成できない．

    //! @brief コンストラクタでWindowのサイズと，カメラの管理を行うクラスを受け取る．
    //! @param[in] window_x ウィンドウの横幅．
    //! @param[in] window_y ウィンドウの縦幅．
    DxlibGuiRobotControl(
        int window_x, int window_y,
        const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
        const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
        const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

    ~DxlibGuiRobotControl();


    void Update() override;

    void Draw() const override;


    void SetNode(const RobotStateNode& node) override
    {
        node_ = node;
    }

private:
    static constexpr int kWidth{ 400 };  //!< GUIの横幅．
    static constexpr int kHeight{ 600 };  //!< GUIの縦幅．
    static constexpr int kTitleBarHeight{ 32 };  //!< タイトルバーの高さ．


    //! @brief GUIの文字を描画する．
    void DrawString() const;

    bool IsInWindow() const;

    std::string GetSerialData() const;

    const int window_x_;  //!< ウィンドウのX座標．
    const int window_y_;  //!< ウィンドウのY座標．

    //! 座標変換を行うクラスへのポインタ．
    std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

    //! 順運動学を計算するクラスへのポインタ．
    std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;

    //! 姿勢の妥当性を確認するクラスへのポインタ．
    std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;

    SerialCommunicationThread serial_communication_;  //!< シリアル通信を行うクラス．

    //!  シリアル通信を行うスレッドへのポインタ．
    std::unique_ptr<boost::thread> serial_communication_thread_ptr_;

    RobotStateNode node_;  //!< ロボットの状態．
    bool auto_update_flag_{ false };  //!< 自動更新のフラグ．
    int counter_{ 0 };  //!< カウンタ．
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_ROBOT_CONTROL_H_
