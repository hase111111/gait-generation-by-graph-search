#include "phantomx_renderer_model.h"

#ifndef DESIGNLAB_DONOT_USE_DXLIB

#include <DxLib.h>

#include "designlab_math_util.h"
#include "designlab_rotation_matrix.h"
#include "dxlib_util.h"
#include "model_loader.h"
#include "phantomx_const.h"


namespace dlm = designlab::math_util;
namespace dldu = designlab::dxlib_util;


PhantomXRendererModel::PhantomXRendererModel(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr) :
	calculator_ptr_(calculator_ptr)
{
}

void PhantomXRendererModel::SetDrawNode(const RobotStateNode& node)
{
	draw_node_ = node;

	// 逆運動学で求めた脚の位置を計算する
	calculator_ptr_->CalculateAllJointState(draw_node_ , &draw_joint_state_);
}

void PhantomXRendererModel::Draw() const
{
	dldu::SetZBufferEnable();	// Zバッファを有効にする

	DrawBody();		// ボディの描画

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawCoxaLink(i);	// 脚の描画
	}


	{
		unsigned int color = GetColor(0, 0, 0);

		DrawSphere3D(dldu::ConvertToDxlibVec(draw_node_.global_center_of_mass), 10, 16, color, color, TRUE);
	
		for (int i = 0; i < 6; i++)
		{
			designlab::Vector3 leg_joint = calculator_ptr_->GetGlobalLegPosition(i, draw_node_.leg_pos[i], draw_node_.global_center_of_mass, draw_node_.rot, true);

			DrawSphere3D(dldu::ConvertToDxlibVec(leg_joint), 5, 16, color, color, TRUE);

			designlab::Vector3 coxa_joint = calculator_ptr_->GetGlobalLegBasePosition(i, draw_node_.global_center_of_mass, draw_node_.rot, true);

			DrawSphere3D(dldu::ConvertToDxlibVec(coxa_joint), 5, 16, color, color, TRUE);
		}

	}
}

bool PhantomXRendererModel::IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const
{
	if (dlm::IsEqual((coxa_joint - femur_joint).GetLength(), PhantomXConst::kCoxaLength)) { return true; }
	return false;
}


bool PhantomXRendererModel::IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const
{
	if (dlm::IsEqual((femur_joint - tibia_joint).GetLength(), PhantomXConst::kFemurLength)) { return true; }
	return false;
}


bool PhantomXRendererModel::IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const
{
	if (dlm::IsEqual((tibia_joint - leg_joint).GetLength(), PhantomXConst::kTibiaLength) ) { return true; }
	return false;
}

void PhantomXRendererModel::DrawBody() const
{
	// モデルの読み込みを行う．ここで呼び出すと毎フレーム読み込むことになりそうだが，実際は既に読込済みならばそのハンドルが返ってくるだけなので問題ない．
	// こんなところでこの処理を書いているのは，コンストラクタで呼び出すと，Dxlibの初期化が終わっていないので，エラーが出るからである．
	int body_model_handle = ModelLoader::GetIns()->LoadModel("model/body.mv1");

	// モデルの読み込みがされていなければ描画しない(というかできない)
	if (body_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(body_model_handle)"); }

	const VECTOR kScale = VGet(10.f, 10.f, 10.f);


	MV1SetScale(body_model_handle, kScale);

	// dxlibの座標系は左手座標系なので，右手座標系に変換するために逆転させる．
	MV1SetRotationXYZ(body_model_handle, VGet(-draw_node_.rot.x_angle, -draw_node_.rot.y_angle, -draw_node_.rot.z_angle ));

	MV1SetPosition(body_model_handle, dldu::ConvertToDxlibVec(draw_node_.global_center_of_mass));

	MV1DrawModel(body_model_handle);
}

void PhantomXRendererModel::DrawCoxaLink(const int leg_index) const
{
	// モデルの読み込みを行う．ここで呼び出すと毎フレーム読み込むことになりそうだが，実際は既に読込済みならばそのハンドルが返ってくるだけなので問題ない．
	// こんなところでこの処理を書いているのは，コンストラクタで呼び出すと，Dxlibの初期化が終わっていないので，エラーが出るからである．
	int connect_model_handle = ModelLoader::GetIns()->LoadModel("model/connect.mv1");

	if (connect_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(connect_model_handle)"); }

	if (draw_joint_state_[leg_index].global_joint_position.size() != 4) { return; }
	if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }


	const VECTOR kScale = VGet(10.f, 10.f, 10.f);

	const VECTOR kCoxaJointPos = dldu::ConvertToDxlibVec(draw_joint_state_[leg_index].global_joint_position[0]);

	const float kCoxaAngle = draw_joint_state_[leg_index].joint_angle[0];

	const designlab::RotationMatrix3x3 kDefRotMat = 
		designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(designlab::math_util::ConvertDegToRad(90.0f));

	const designlab::RotationMatrix3x3 kBodyRotMat(draw_node_.rot);

	const float kOffsetLength = 26.33f;

	const VECTOR kOffsetPos = dldu::ConvertToDxlibVec(
		designlab::rotVector
		(
			designlab::Vector3::GetFrontVec() * kOffsetLength, 
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) * kBodyRotMat
		)
	);

	


	MV1SetScale(connect_model_handle, kScale);

	// dxlibの座標系は左手座標系なので，右手座標系に変換するために逆転させる．
	designlab::EulerXYZ rot = (kBodyRotMat * kDefRotMat).ToEulerXYZ() * -1;
	MV1SetRotationXYZ(connect_model_handle, VGet(rot.x_angle, rot.y_angle, rot.z_angle));

	MV1SetPosition(connect_model_handle, VAdd(kCoxaJointPos, kOffsetPos));

	MV1DrawModel(connect_model_handle);
}



#endif