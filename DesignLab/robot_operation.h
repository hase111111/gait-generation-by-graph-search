
//! @file robot_operation.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_ROBOT_OPERATION_H_
#define DESIGNLAB_ROBOT_OPERATION_H_

#include <format>

#include "math_quaternion.h"
#include "string_util.h"
#include "toml_serialize_macro.h"
#include "vector3.h"

namespace designlab {

//! @enum RobotOperationType
//! @brief Robotをどのように動かすかを表す列挙体.
//! @details
//! @subsubsection [列挙体について]
//! 以下で定義されている RobotOperationType が列挙体に当たる.
//! イメージとしては以下に宣言された値しかとることのできない新しい
//! 変数を定義するような形.
//! @n こうすることで関数を特定のモードで動作させたいときなどに,
//! 入力されたくない数字が入力されなくなる.
//! @n また値に名前を付けることで何を想定してこの値が代入されたのかが
//! 分かりやすくなるという利点がある.
//! @n C++ には enum と enum class という2通りの列挙体があるが,
//! 安全のため enum class の使用をおすすめする.
//! @n よくわからない場合は, RobotOperationType の記述を真似してみること.
enum class RobotOperationType : int {
  kNone,                //!< 無効値.
  kStraightMoveVector,  //!< 直線移動をさせる（移動したい方向をベクトルで示す）
  kStraightMovePosition,  //!< 直線移動をさせる（移動したい座標を示す）
  kSpotTurnLastPosture,   //!< その場で旋回させる（最終的な姿勢 Posture を示す）
  kSpotTurnRotAxis,  //!< その場で旋回させる（回転軸を示し,その軸周りの右ねじの回転）
  kTurn,             //!< 旋回中心と,旋回半径と,旋回方向を与えて旋回させる.
};

//! @struct RobotOperation
//! @brief 探索において目標となる座標や角度,評価する値についてまとめた構造体.
//! @details
//! 先行研究では target という名前だった.
struct RobotOperation final {
  //!< 目標方向.正規化されたベクトル.
  Vector3 straight_move_vector{1.f, 0.f, 0.f};

  //!< 目標位置（グローバル座標）.
  Vector3 straight_move_position{10000.f, 0.f, 0.f};

  //!< 目標姿勢 ( posture )
  Quaternion spot_turn_last_posture{
      Quaternion::MakeByAngleAxis(0, Vector3::GetUpVec())};

  //!< 旋回時の回転軸.右ねじの回転.
  Vector3 spot_turn_rot_axis{Vector3::GetUpVec()};

  Vector2 turn_center{0.f, 0.f};  //!< 旋回中心.

  float turn_radius{1000.f};  //!< 旋回半径.

  bool turn_clockwise{true};  //!< 旋回方向.

  //!< どうやって目標を評価するか.
  RobotOperationType operation_type{kStraightMovePosition};

 private:
  using enum RobotOperationType;
};

DESIGNLAB_TOML11_DESCRIPTION_CLASS(RobotOperation) {
  DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION(
      "Note that this file is not referenced by some settings.");

  DESIGNLAB_TOML11_TABLE_NO_DESCRIPTION();

  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      straight_move_vector, "Param",
      "The value that is referenced when the setting is to go straight in the "
      "specified direction.");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      straight_move_position, "Param",
      "The value that is referred to when the setting is to go straight to the "
      "specified coordinates.");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      spot_turn_last_posture, "Param",
      "The value that is referenced when setting the camera to turn on the "
      "spot until it reaches the specified posture.");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      spot_turn_rot_axis, "Param",
      "The value referred to when setting up in-situ turning around the "
      "specified axis (in the direction of right-hand threads).");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      turn_center, "Param",
      "The value referred to when setting the rotation around the specified "
      "center.");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      turn_radius, "Param",
      "The value referenced in the case of a setup that turns at a specified "
      "radius.");
  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      turn_clockwise, "Param",
      "The value that is referenced when the setting is to turn in the "
      "specified direction.");

  DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
      operation_type, DESIGNLAB_TOML11_NO_TABLE,
      std::format("This is the setting of the robot's operation method. ({})",
                  string_util::EnumValuesToString<RobotOperationType>(" / ")));
};

}  // namespace designlab

DESIGNLAB_TOML11_SERIALIZE(designlab::RobotOperation, straight_move_vector,
                           straight_move_position, spot_turn_last_posture,
                           spot_turn_rot_axis, operation_type, turn_center,
                           turn_radius, turn_clockwise);

#endif  // DESIGNLAB_ROBOT_OPERATION_H_
