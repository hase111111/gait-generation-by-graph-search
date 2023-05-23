#pragma once
#include "listFunc.h"
#include "hexapod.h"

// ロボットの描画を行うクラス．
class HexapodRenderer
{
private:
	const unsigned int COLOR_BODY;			//胴体の色
	const unsigned int COLOR_LEG;			//脚の色
	const unsigned int COLOR_LIFTED_LEG;	//遊脚している脚の色
	const unsigned int COLOR_JOINT;			//ジョイントの色
	const unsigned int COLOR_LIFTED_JOINT;	//遊脚しているジョイントの色

	const int CAPSULE_DIV_NUM;				//ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int SPHERE_DIV_NUM;				//ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float LEG_R = 10.0f;				//脚の半径．このクラスでは脚を円柱に近似して描画している．
	const float JOINT_R = 20.0f;			//ジョイントの半径．

public:
	HexapodRenderer();
	~HexapodRenderer() = default;

	//ロボットを3D空間に描画する．
	void draw(const Hexapod& _hexapod, const int _leg_state) const;

};
