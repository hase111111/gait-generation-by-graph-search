//! @file designlab_rot_converter.h
//! @brief 回転角行列，オイラー角，クォータニオンの変換を行う関数

#ifndef DESIGNLAB_ROT_CONVERTER_H_
#define DESIGNLAB_ROT_CONVERTER_H_


#include "designlab_euler.h"
#include "designlab_quaternion.h"
#include "designlab_rotation_matrix.h"

namespace designlab 
{
	//! @brief 回転角行列からクォータニオンへの変換
	//! @param[in] rot 回転角行列
	//! @return クォータニオン
	Quaternion ToQuaternion(const RotationMatrix3x3& rot);

	//! @brief クォータニオンからXYZオイラー角への変換
	//! @param[in] q クォータニオン
	//! @return XYZオイラー角
	Quaternion ToQuaternion(const EulerXYZ& e);

	//! @brief クォータニオンから回転角行列への変換
	//! @param[in] q クォータニオン
	//! @return 回転角行列
	RotationMatrix3x3 ToRotationMatrix(const Quaternion& q);

	//! @brief XYZオイラー角から回転角行列への変換
	//! @param[in] e XYZオイラー角
	//! @return 回転角行列
	RotationMatrix3x3 ToRotationMatrix(const EulerXYZ& e);

	//! @brief 回転角行列からXYZオイラー角への変換
	//! @param[in] rot 回転角行列
	//! @return XYZオイラー角
	EulerXYZ ToEulerXYZ(const RotationMatrix3x3& rot);

	//! @brief 回転角行列からクォータニオンへの変換
	//! @param[in] e XYZオイラー角
	//! @return クォータニオン
	EulerXYZ ToEulerXYZ(const Quaternion& q);
}

#endif