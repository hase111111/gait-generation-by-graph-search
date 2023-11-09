//! @file interface_hexapod_coordinate_converter.h
//! @brief ロボットは重心位置・アクチュエータなど様々な点を基準とする座標系を持つ．これらを相互に変換するクラス．


#ifndef INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H
#define INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H


#include "designlab_vector3.h"
#include "designlab_euler.h"


//! @class IHexapodCoordinateConverter
//! @brief ロボットは重心位置・アクチュエータなど様々な点を基準とする座標系を持つ．これらを相互に変換するクラス．
//! @details プログラムには3つの座標系が存在している，
//! @n
//! @n [1] グローバル座標系 (Global Coordinate)
//! @n		マップの原点を原点とする座標系，座標軸はマップの座標軸と同じ．
//! @n
//! @n [2] ロボット座標系 (Robot Coordinate)
//! @n		ロボットの重心を原点とする座標系，座標軸はロボットの姿勢に合わせる．
//! @n 
//! @n [3] 脚座標系 (Leg Coordinate)
//! @n		脚の付け根を原点とする座標系，座標軸はロボットの姿勢に合わせる(ローカル座標系と同じ)．
//! @n		運動学の教科書とか読む感じ，この座標系の取り方とは違うのだが，先行研究で使われた手法であり，
//! @n		実用上問題ないのでこのまま．
class IHexapodCoordinateConverter
{
public:
	virtual ~IHexapodCoordinateConverter() = default;	//継承するクラスはデストラクタをvirtualにすること．

	//! @brief グローバル座標系で表現されている座標を，脚座標系に変換する．
	//! @param [in] converted_position 変換対象．グローバル座標系．
	//! @param [in] leg_index 脚番号．
	//! @param [in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 脚座標系の座標．脚先座標系とは脚の付け根を原点とし，軸はロボット座標系と同様な座標系．
	virtual designlab::Vector3 ConvertGlobalToLegCoordinate(const designlab::Vector3& converted_position, int leg_index,
		const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;

	//! @brief 脚座標系で表現されている座標を，グローバル座標系に変換する．
	//! @param [in] converted_position 変換対象．脚座標系．
	//! @param [in] leg_index 脚番号．
	//! @param [in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 グローバル座標系の座標．
	virtual designlab::Vector3 ConvertLegToGlobalCoordinate(const designlab::Vector3& converted_position, int leg_index,
		const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;

	//! @brief ロボット座標系で表現されている座標を，グローバル座標系に変換する．
	//! @param [in] converted_position 変換対象．ロボット座標系．
	//! @param [in] center_of_mass_global ロボットの重心の座標．グローバル座標系．
	//! @param [in] robot_rot ロボットの姿勢．角度はrad.
	//! @param [in] consider_rot ロボットの姿勢を考慮するかどうか．falseなら回転を考慮しない．
	//! @return designlab::Vector3 グローバル座標系の座標．
	virtual designlab::Vector3 ConvertRobotToGlobalCoordinate(const designlab::Vector3& converted_position,
		const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, bool consider_rot) const = 0;
};


#endif	// INTERFACE_HEXAPOD_COORDINATE_CONVERTER_H