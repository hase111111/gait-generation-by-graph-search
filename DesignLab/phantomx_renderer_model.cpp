#include "phantomx_renderer_model.h"

#ifndef DESIGNLAB_DONOT_USE_DXLIB

#include <DxLib.h>

#include "designlab_math_util.h"
#include "designlab_rot_converter.h"
#include "designlab_rotation_matrix.h"
#include "dxlib_util.h"
#include "model_loader.h"
#include "phantomx_mk2_const.h"


namespace dlm = designlab::math_util;
namespace dldu = designlab::dxlib_util;


PhantomXRendererModel::PhantomXRendererModel(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr_,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr
) :
	converter_ptr_(converter_ptr_),
	calculator_ptr_(calculator_ptr),
	draw_node_{}
{
}

void PhantomXRendererModel::SetDrawNode(const RobotStateNode& node)
{
	draw_node_ = node;

	draw_joint_state_ = calculator_ptr_->CalculateAllJointState(node);
}

void PhantomXRendererModel::Draw() const
{
	dldu::SetZBufferEnable();	// Zバッファを有効にする

	DrawBody();		// ボディの描画

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawCoxaLink(i);	// 脚の描画

		DrawFemurLink(i);	// 脚の描画

		DrawTibiaLink(i);	// 脚の描画

		DrawJointAxis(i);	// 関節軸の描画
	}
}

void PhantomXRendererModel::DrawBody() const
{
	const int body_model_handle = ModelLoader::GetIns()->GetModelHandle("model/body.mv1");

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
	const int coxa_model_handle = ModelLoader::GetIns()->GetModelHandle("model/coxa_fixed.mv1");

	if (coxa_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(coxa_model_handle)"); }

	if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4) { return; }
	if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }

	//Coxa Jointは2つのConnect Linkで構成されているので，それぞれ描画する
	const VECTOR kScale = VGet(10.f, 10.f, 10.f);

	const VECTOR kCoxaJointPos = dldu::ConvertToDxlibVec(
		converter_ptr_->ConvertLegToGlobalCoordinate(
			draw_joint_state_[leg_index].joint_pos_leg_coordinate[0], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
		)
	);

	const float kCoxaAngle = draw_joint_state_[leg_index].joint_angle[0];

	const designlab::RotationMatrix3x3 kBodyRotMat = designlab::ToRotationMatrix(draw_node_.rot);

	const float kOffsetLength = 0;	//回転中心と原点がずれているので，その分を補正する

	{
		const designlab::RotationMatrix3x3 kDefRotMat =
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) *
			designlab::RotationMatrix3x3::CreateRotationMatrixY(dlm::ConvertDegToRad(-90.0f));

		const VECTOR kOffsetPos = dldu::ConvertToDxlibVec(
			designlab::RotateVector3
			(
				designlab::Vector3::GetFrontVec() * kOffsetLength,
				designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) * kBodyRotMat
			)
		);

		MV1SetScale(coxa_model_handle, kScale);

		// dxlibの座標系は左手座標系なので，右手座標系に変換するために逆転させる．
		designlab::EulerXYZ rot = (kBodyRotMat * kDefRotMat).ToEulerXYZ() * -1.f;
		MV1SetRotationXYZ(coxa_model_handle, VGet(rot.x_angle, rot.y_angle, rot.z_angle));

		MV1SetPosition(coxa_model_handle, kCoxaJointPos + kOffsetPos);

		MV1DrawModel(coxa_model_handle);
	}
}

void PhantomXRendererModel::DrawFemurLink(const int leg_index) const
{
	const int thign_model_handle = ModelLoader::GetIns()->GetModelHandle("model/thign_l.mv1");

	if (thign_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(thign_model_handle)"); }

	if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4) { return; }
	if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }

	//パラメータの計算
	const VECTOR kScale = VGet(10.f, 10.f, 10.f);

	const VECTOR kFemurJointPos = dldu::ConvertToDxlibVec(
		converter_ptr_->ConvertLegToGlobalCoordinate(
			draw_joint_state_[leg_index].joint_pos_leg_coordinate[1], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
		)
	);

	const float kCoxaAngle = draw_joint_state_[leg_index].joint_angle[0];

	const float kFemurAngle = draw_joint_state_[leg_index].joint_angle[1];

	const designlab::RotationMatrix3x3 kBodyRotMat = designlab::ToRotationMatrix(draw_node_.rot);

	// リンクが曲がっているため，簡単のために回転軸を結ぶように仮想的なリンクを使っている．
	// そのため，仮想的なリンクの角度を補正する必要がある．
	const float virtual_link_offset_angle = dlm::ConvertDegToRad(-13.5f);	
	
	const designlab::RotationMatrix3x3 kDefRotMat =
		designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(-kFemurAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixX(dlm::ConvertDegToRad(-90.0f)) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(dlm::ConvertDegToRad(-90.f)) *
		designlab::RotationMatrix3x3::CreateRotationMatrixX(virtual_link_offset_angle);

	const VECTOR kOffsetPos = dldu::ConvertToDxlibVec(
		designlab::RotateVector3
		(
			designlab::Vector3::GetFrontVec(),
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) * kBodyRotMat
		)
	);

	//描画する．
	MV1SetScale(thign_model_handle, kScale);

	// dxlibの座標系は左手座標系なので，右手座標系に変換するために逆転させる．
	designlab::EulerXYZ rot = (kBodyRotMat * kDefRotMat).ToEulerXYZ() * -1.f;
	MV1SetRotationXYZ(thign_model_handle, VGet(rot.x_angle, rot.y_angle, rot.z_angle));

	MV1SetPosition(thign_model_handle, kFemurJointPos + kOffsetPos);

	MV1DrawModel(thign_model_handle);
}

void PhantomXRendererModel::DrawTibiaLink(int leg_index) const
{
	// モデルの読み込みを行う．ここで呼び出すと毎フレーム読み込むことになりそうだが，実際は既に読込済みならばそのハンドルが返ってくるだけなので問題ない．
	// こんなところでこの処理を書いているのは，コンストラクタで呼び出すと，Dxlibの初期化が終わっていないので，エラーが出るからである．
	int tibia_model_handle = ModelLoader::GetIns()->GetModelHandle("model/tibia_l_fixed.mv1");

	if (tibia_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(tibia_model_handle)"); }

	if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4) { return; }
	if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }

	//パラメータの計算
	const VECTOR kScale = VGet(0.01f, 0.01f, 0.01f);

	const VECTOR kTibiaJointPos = dldu::ConvertToDxlibVec(
		converter_ptr_->ConvertLegToGlobalCoordinate(
			draw_joint_state_[leg_index].joint_pos_leg_coordinate[2], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
		)
	);

	const float kCoxaAngle = draw_joint_state_[leg_index].joint_angle[0];

	const float kFemurAngle = draw_joint_state_[leg_index].joint_angle[1];

	const float kTibiaAngle = draw_joint_state_[leg_index].joint_angle[2];

	const designlab::RotationMatrix3x3 kBodyRotMat = designlab::ToRotationMatrix(draw_node_.rot);

	const designlab::RotationMatrix3x3 kDefRotMat =
		designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(-kFemurAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(-kTibiaAngle) *
		designlab::RotationMatrix3x3::CreateRotationMatrixX(dlm::ConvertDegToRad(-90.0f)) *
		designlab::RotationMatrix3x3::CreateRotationMatrixY(dlm::ConvertDegToRad(90.f));

	const VECTOR kOffsetPos = dldu::ConvertToDxlibVec(
	designlab::RotateVector3
		(
			designlab::Vector3::GetFrontVec(),
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle) * kBodyRotMat
		)
	);

	//描画する．
	MV1SetScale(tibia_model_handle, kScale);

	// dxlibの座標系は左手座標系なので，右手座標系に変換するために逆転させる．
	designlab::EulerXYZ rot = (kBodyRotMat * kDefRotMat).ToEulerXYZ() * -1.f;
	MV1SetRotationXYZ(tibia_model_handle, VGet(rot.x_angle, rot.y_angle, rot.z_angle));

	MV1SetPosition(tibia_model_handle, kTibiaJointPos + kOffsetPos);

	MV1DrawModel(tibia_model_handle);
}

void PhantomXRendererModel::DrawJointAxis(int leg_index) const
{
	if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4) { return; }
	if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }


	const float kAxisLength = 100.f;
	const float kAxisRadius = 2.f;
	const int kAxisDivNum = 16;

	const unsigned int kCoxaAxisColor = GetColor(0, 0, 255);
	const unsigned int kFemurAxisColor = GetColor(0, 255, 0);
	const unsigned int kTibiaAxisColor = kFemurAxisColor;
	const unsigned int kSpecColor = GetColor(255, 255, 255);
	[[maybe_unused]]const unsigned int kJointColor = GetColor(64, 64, 64);

	const float kCoxaAngle = draw_joint_state_[leg_index].joint_angle[0];

	const designlab::RotationMatrix3x3 kBodyRotMat = designlab::ToRotationMatrix(draw_node_.rot);

	//Coxaの回転軸
	{
		const VECTOR kCoxaJointPos = dldu::ConvertToDxlibVec(
			converter_ptr_->ConvertLegToGlobalCoordinate(
				draw_joint_state_[leg_index].joint_pos_leg_coordinate[0], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
			)
		);

		const VECTOR kAxisVec = dldu::ConvertToDxlibVec(
			designlab::RotateVector3(designlab::Vector3::GetUpVec() * kAxisLength / 2, kBodyRotMat)
		);

		DrawCapsule3D(kCoxaJointPos - kAxisVec, kCoxaJointPos + kAxisVec, kAxisRadius, kAxisDivNum, kCoxaAxisColor, kSpecColor, TRUE);

		//間接に点を描画する
		//DrawSphere3D(kCoxaJointPos, kAxisRadius * 2, kAxisDivNum, kJointColor, kSpecColor, TRUE);
	}

	//Femurの回転軸
	{
		const VECTOR kFemurJointPos = dldu::ConvertToDxlibVec(
			converter_ptr_->ConvertLegToGlobalCoordinate(
				draw_joint_state_[leg_index].joint_pos_leg_coordinate[1], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
			)
		);

		const designlab::RotationMatrix3x3 kDefRotMat =
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle);

		const VECTOR kAxisVec = dldu::ConvertToDxlibVec(
			designlab::RotateVector3
			(
				designlab::Vector3::GetLeftVec() * kAxisLength / 2,
				kDefRotMat * kBodyRotMat
			)
		);

		DrawCapsule3D(kFemurJointPos - kAxisVec, kFemurJointPos + kAxisVec, kAxisRadius, kAxisDivNum, kFemurAxisColor, kSpecColor, TRUE);

		//間接に点を描画する
		//DrawSphere3D(kFemurJointPos, kAxisRadius * 2, kAxisDivNum, kJointColor, kSpecColor, TRUE);
	}

	//Tibiaの回転軸
	{
		const VECTOR kTibiaJointPos = dldu::ConvertToDxlibVec(
			converter_ptr_->ConvertLegToGlobalCoordinate(
				draw_joint_state_[leg_index].joint_pos_leg_coordinate[2], leg_index, draw_node_.global_center_of_mass, draw_node_.rot, true
			)
		);

		const designlab::RotationMatrix3x3 kDefRotMat =
			designlab::RotationMatrix3x3::CreateRotationMatrixZ(kCoxaAngle);

		const VECTOR kAxisVec = dldu::ConvertToDxlibVec(
			designlab::RotateVector3
			(
				designlab::Vector3::GetLeftVec() * kAxisLength / 2,
				kDefRotMat * kBodyRotMat
			)
		);

		DrawCapsule3D(kTibiaJointPos - kAxisVec, kTibiaJointPos + kAxisVec, kAxisRadius, kAxisDivNum, kTibiaAxisColor, kSpecColor, TRUE);

		//間接に点を描画する
		//DrawSphere3D(kTibiaJointPos, kAxisRadius * 2, kAxisDivNum, kJointColor, kSpecColor, TRUE);
	}
}


#endif	// #ifndef DESIGNLAB_DONOT_USE_DXLIB