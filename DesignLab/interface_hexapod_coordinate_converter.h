
//! @file      interface_hexapod_coordinate_converter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H_
#define DESIGNLAB_INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H_

#include "math_quaternion.h"
#include "math_vector3.h"


namespace designlab
{

//! @class IHexapodCoordinateConverter
//! @brief ロボットは重心位置・アクチュエータなど様々な点を基準とする座標系を持つ．
//! これらを相互に変換する処理のインターフェース．
//! @details
//! プログラムには3つの座標系が存在している，
//! @li グローバル座標系 (Global Coordinate) マップの原点を原点とする座標系，
//! 座標軸はマップの座標軸と同じ．
//! @li ロボット座標系 (Robot Coordinate) ロボットの重心を原点とする座標系，
//! 座標軸はロボットの姿勢に合わせる．
//! @li 脚座標系 (Leg Coordinate) 脚の付け根を原点とする座標系，
//! 座標軸はロボットの姿勢に合わせる(ローカル座標系と同じ)．
//!
//! 運動学の教科書とか読む感じ，この座標系の取り方とは違うのだが，先行研究で使われた手法であり，
//! 実用上問題ないのでこのまま．
class IHexapodCoordinateConverter
{
public:
    //! 継承するクラスはデストラクタを virtualにすること．
    virtual ~IHexapodCoordinateConverter() = default;

    //! @brief グローバル座標系で表現されている座標を，脚座標系に変換する．
    //! @param[in] converted_position 変換対象．グローバル座標系．
    //! @param[in] leg_index 脚番号．
    //! @param[in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
    //! @param[in] robot_rot ロボットの姿勢．角度はrad.
    //! @param[in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
    //! @return 脚座標系の座標．脚先座標系とは脚の付け根を原点とし，
    //! 軸はロボット座標系と同様な座標系．
    virtual Vector3 ConvertGlobalToLegCoordinate(const Vector3& converted_position,
                                                 int leg_index,
                                                 const Vector3& center_of_mass_global,
                                                 const Quaternion& robot_quat,
                                                 bool consider_rot) const = 0;

    //! @brief 脚座標系で表現されている座標を，グローバル座標系に変換する．
    //! @param[in] converted_position 変換対象．脚座標系．
    //! @param[in] leg_index 脚番号．
    //! @param[in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
    //! @param[in] robot_rot ロボットの姿勢．角度は[rad].
    //! @param[in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
    //! @return グローバル座標系の座標．
    virtual Vector3 ConvertLegToGlobalCoordinate(const Vector3& converted_position,
                                                 int leg_index,
                                                 const Vector3& center_of_mass_global,
                                                 const Quaternion& robot_quat,
                                                 bool consider_rot) const = 0;

    //! @brief ロボット座標系で表現されている座標を，グローバル座標系に変換する．
    //! @param[in] converted_position 変換対象．ロボット座標系．
    //! @param[in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
    //! @param[in] robot_rot ロボットの姿勢．角度は[rad].
    //! @param[in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
    //! @return グローバル座標系の座標．
    virtual Vector3 ConvertRobotToGlobalCoordinate(const Vector3& converted_position,
                                                   const Vector3& center_of_mass_global,
                                                   const Quaternion& robot_quat,
                                                   bool consider_rot) const = 0;

    //! @brief ロボット座標系で表現されている座標を，脚座標系に変換する．
    virtual Vector3 ConvertRobotToLegCoordinate(const Vector3& converted_position,
                                                int leg_index) const = 0;

    //! @brief 脚座標系で表現されている座標を，ロボット座標系に変換する．
    virtual Vector3 ConvertLegToRobotCoordinate(const Vector3& converted_position,
                                                int leg_index) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H_
