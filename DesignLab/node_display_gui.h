//! @file node_display_gui.h
//! @brief ノードの情報を表示するGUI


#ifndef DESIGNLAB_NODE_DISPLAY_GUI_H_
#define DESIGNLAB_NODE_DISPLAY_GUI_H_

#include <array>
#include <map>
#include <memory>

#include "button_controller.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_vaild_checker.h"
#include "robot_state_node.h"


//! @class NodeDisplayGui
//! @brief ノードの情報を表示するGUI
class NodeDisplayGui final
{
public:

	//! @param [in] x_pos GUIの左上のx座標
	//! @param [in] y_pos GUIの左上のy座標
	//! @param [in] calculator_ptr 六脚歩行ロボットの状態を計算するクラス
	NodeDisplayGui(
		int x_pos, 
		int y_pos, 
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	);


	//! @brief 表示するノードを設定する，その後関節の角度を計算し，セットする
	//! @param [in] node 表示するノード
	void SetDisplayNode(const RobotStateNode& node);


	//! @brief GUIのボタンの更新を行う
	void Update();

	//! @brief GUIの表示を行う
	void Draw() const;


	const static int kWidth;			//!< GUIの幅
	const static int kHeight;			//!< GUIの高さ
	const static int kClosedHeight;		//!< GUIが閉じているときの高さ

private:

	enum class ButtonType
	{
		kOpenClose,
		kModeSwitching
	};

	enum class DisplayMode
	{
		kDefualt,		//デフォルト
		kJointState		//角度
	};


	void DrawBackground() const;

	void DrawNodeInfo() const;

	void DrawJointInfo() const;


	const int kGuiLeftPosX;

	const int kGuiTopPosY;


	std::map<ButtonType, std::unique_ptr<ButtomController>> buttons_;				//!< ボタン

	const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;	//!< 六脚歩行ロボットの状態を計算するクラス
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr_;


	RobotStateNode display_node_;										//!< 表示するノード

	std::array<HexapodJointState, HexapodConst::kLegNum> joint_state_;	//!< 関節の角度

	bool is_closed_;			//!< GUIが閉じているか(最小化しているか)どうか

	DisplayMode display_type_;	//!< 表示する情報の種類
};


#endif // !DESIGNLAB_NODE_DISPLAY_GUI_H_