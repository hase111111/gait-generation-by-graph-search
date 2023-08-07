#pragma once
#include "my_math.h"

//! @brief  Hexapod，phantomXのパラメータを定数で表現したもの．
class HexapodConst final
{
public:

	//Hexapodの脚の本数を表す．これを変更しても脚の本数が変更できるわけではない．マジックナンバーをなくすことが目的．
	constexpr static int LEG_NUM = 6;

	//ロボットの寸法
	constexpr static float COXA_LENGTH = 52.0f;			//!< 第1関節部の長さ．詳しくはreferenceをフォルダ参照．
	constexpr static float FEMUR_LENGTH = 66.0f;		//第2関節部の長さ．詳しくはreferenceをフォルダ参照．
	constexpr static float TIBIA_LENGTH = 130.0f;		//第3関節部の長さ．詳しくはreferenceをフォルダ参照．
	constexpr static float BODY_FRONT_WIDTH = 60.0f;	//前方の幅[mm]．phantomXの横幅.
	constexpr static float BODY_CENTER_WIDTH = 100.0f;	//中心の幅[mm]．phantomXの横幅.
	constexpr static float BODY_REAR_WIDTH = 60.0f;		//後方の幅[mm]．phantomXの横幅.
	constexpr static float BODY_FRONT_LENGTH = 120.0f;	//中央から前方までの距離[mm]．phantomXの縦幅.
	constexpr static float BODY_REAR_LENGTH = 120.0f;	//中央から前方までの距離[mm]．phantomXの縦幅.
	constexpr static float BODY_HEIGHT = 40.0f;			//胴体の高さ[mm]．

	//ロボットの可動範囲
	const static float VERTICAL_MAX_RANGE;			//地面の最高点と胴体下方の隙間の最大値を示す．脚を伸ばし切らない程度に設定する．旧名 MAX_DELTAZ
	const static float VERTICAL_MIN_RANGE;			//地面の最高点と胴体下方の隙間の最小値を示す．旧名 MIN_DELTAZ

	constexpr static float MOVABLE_LEG_RANGE = my_math::convertDegToRad(40.0f);			//coxa関節(第1関節)の稼働可能な角度 [rad]

	//MOVABLE_LEG_RANGEのcos値 -85°，-130°，-175°，95°，50°，5°
	constexpr static float MOVABLE_LEG_RANGE_COS_MAX[LEG_NUM] = { 0.08715574f , -0.6427876f, -0.9961947f, -0.08715574f, 0.6427876f, 0.9961947f };

	//MOVABLE_LEG_RANGEのcos値 -5°，-50°，-95°，175°，130°，85°
	constexpr static float MOVABLE_LEG_RANGE_COS_MIN[LEG_NUM] = { 0.9961947f,  0.6427876f, -0.08715574f, -0.9961947f, -0.6427876f, 0.08715574f };

	//MOVABLE_LEG_RANGEのsin値 -5°，-50°，-95°，175°，130°，85°
	constexpr static float MOVABLE_LEG_RANGE_SIN_MAX[LEG_NUM] = { -0.08715574f, -0.76604444f, -0.9961947f, 0.0871557f, 0.76604444f, 0.9961946f };

	//MOVABLE_LEG_RANGEのsin値 -85°，-130°，-175°，95°，50°，5°
	constexpr static float MOVABLE_LEG_RANGE_SIN_MIN[LEG_NUM] = { -0.9961947f, -0.76604444f, -0.08715574f, 0.9961947f, 0.76604444f,0.08715574f };


	//! 脚の第1関節の初期角度を示す[rad]．ロボットの正面を 0[rad]として，右ねじを正にとる．
	constexpr static float DEFAULT_LEG_ANGLE[LEG_NUM] = { my_math::convertDegToRad(-45.0f),	my_math::convertDegToRad(-90.0f),	my_math::convertDegToRad(-135.0f),
													my_math::convertDegToRad(135.0f),	my_math::convertDegToRad(90.0f),	my_math::convertDegToRad(45.0f) };

	//! DEFAULT_LEG_ANGLEの値を元にsin cos を計算しておく．
	constexpr static float DEFAULT_LEG_ANGLE_SIN[LEG_NUM] = { -0.70710678118654f, -1.0f, -0.70710678118654f,
													 0.70710678118654f, 1.0f, 0.70710678118654f };

	constexpr static float DEFAULT_LEG_ANGLE_COS[LEG_NUM] = { 0.70710678118654f, 0.0f, -0.70710678118654f,
														 -0.70710678118654f, 0.0f, 0.70710678118654f };

private:
	//コンストラクタとコピーコンストラクタを削除し，実体を生成できないようにする．
	HexapodConst() = delete;
	HexapodConst(const HexapodConst& _other) = delete;
};

