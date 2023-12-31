
//! @file      math_rot_converter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MATH_ROT_CONVERTER_H_
#define DESIGNLAB_MATH_ROT_CONVERTER_H_

#include "math_euler.h"
#include "math_quaternion.h"
#include "math_rotation_matrix.h"


namespace designlab
{

//! @brief 回転角行列からクォータニオンへの変換．
//! @param[in] rot 回転角行列．
//! @return クォータニオン．
[[nodiscard]] Quaternion ToQuaternion(const RotationMatrix3x3& rot);

//! @brief オイラー角からクォータニオンへの変換．
//! @param[in] e オイラー角．
//! @return クォータニオン．
[[nodiscard]] Quaternion ToQuaternion(const EulerXYZ& e);

//! @brief クォータニオンから回転角行列への変換．
//! @param[in] q クォータニオン．
//! @return 回転角行列．
[[nodiscard]] RotationMatrix3x3 ToRotationMatrix(const Quaternion& q);

//! @brief XYZオイラー角から回転角行列への変換．
//! @param[in] e XYZオイラー角．
//! @return 回転角行列．
[[nodiscard]] RotationMatrix3x3 ToRotationMatrix(const EulerXYZ& e);

//! @brief 回転角行列からXYZオイラー角への変換．
//! @param[in] rot 回転角行列．
//! @return XYZオイラー角．
[[nodiscard]] EulerXYZ ToEulerXYZ(const RotationMatrix3x3& rot);

//! @brief クォータニオンからXYZオイラー角への変換．
//! @param[in] q クォータニオン．
//! @return オイラー角．
[[nodiscard]] EulerXYZ ToEulerXYZ(const Quaternion& q);


}  // namespace designlab


#endif  // DESIGNLAB_MATH_ROT_CONVERTER_H_
