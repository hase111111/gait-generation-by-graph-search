#pragma once
#include "Node.h"
#include "HexapodStateCalculator.h"

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
	const float LEG_R = 10.0f;				//脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float JOINT_R = 20.0f;			//ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	HexapodStateCalclator m_HexaCalc;		//ロボットの姿勢や座標を計算する．

public:
	HexapodRenderer();
	~HexapodRenderer() = default;

	//ロボットの状態を更新する．
	void update(const SNode& _node);

	//ロボットを3D空間に描画する．
	void draw(const SNode& _node) const;

};
