//! @file robot_operator_fixed.h
//! @brief ターゲットの更新をせず，初期化時のみに更新するクラス．

#ifndef DESIGNLAB_TARGET_UPDATER_ONCE_H_
#define DESIGNLAB_TARGET_UPDATER_ONCE_H_

#include "interface_robot_operator.h"


namespace designlab
{

//! @class RobotOperatorFixed
//! @brief ターゲットの更新をせず，初期化時のみに更新するクラス．
class RobotOperatorFixed : public IRobotOperator
{
public:

	RobotOperatorFixed() = delete;

	//! @brief コンストラクタで指定したターゲットを常に返す．
	//! @param[in] target ターゲット.
	RobotOperatorFixed(const RobotOperation& target);

	RobotOperation Init() const override;

	RobotOperation Update(const RobotStateNode& state) override;

private:

	const RobotOperation operation_;
};

} // namespace designlab


#endif // DESIGNLAB_TARGET_UPDATER_ONCE_H_