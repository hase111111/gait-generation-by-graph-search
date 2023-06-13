#pragma once

// Hexapod，phantomXのパラメータを定数で表現したもの．

class HexapodConst final
{
public:

	//Hexapodの脚の本数を表す．これを変更しても脚の本数が変更できるわけではない．マジックナンバーをなくすことが目的．
	constexpr static int LEG_NUM = 6;

	//ロボットの寸法
	const static float COXA_LENGTH;			//第1関節部の長さ．詳しくはreferenceをフォルダ参照．
	const static float FEMUR_LENGTH;		//第2関節部の長さ．詳しくはreferenceをフォルダ参照．
	const static float TIBIA_LENGTH;		//第3関節部の長さ．詳しくはreferenceをフォルダ参照．
	const static float BODY_FRONT_WIDTH;	//前方の幅[mm]．phantomXの横幅.
	const static float BODY_CENTER_WIDTH;	//中心の幅[mm]．phantomXの横幅.
	const static float BODY_REAR_WIDTH;		//後方の幅[mm]．phantomXの横幅.
	const static float BODY_FRONT_LENGTH;	//中央から前方までの距離[mm]．phantomXの縦幅.
	const static float BODY_REAR_LENGTH;	//中央から前方までの距離[mm]．phantomXの縦幅.
	const static float BODY_HEIGHT;			//胴体の高さ[mm]．

	//ロボットの可動範囲
	const static float VERTICAL_MAX_RANGE;			//地面の最高点と胴体下方の隙間の最大値を示す．脚を伸ばし切らない程度に設定する．旧名 MAX_DELTAZ
	const static float VERTICAL_MIN_RANGE;			//地面の最高点と胴体下方の隙間の最小値を示す．旧名 MIN_DELTAZ
	const static float MOVABLE_LEG_RANGE;			//coxa関節(第1関節)の稼働可能な角度 [rad]
	const static float DEFAULT_LEG_ANGLE[LEG_NUM];	//脚の第1関節の初期角度を示す[rad]．ロボットの正面を 0[rad]として，右ねじを正にとる．

private:
	//コンストラクタとコピーコンストラクタを削除し，実体を生成できないようにする．
	HexapodConst() = delete;
	HexapodConst(const HexapodConst& _other) = delete;
};

