#include "phantomx_renderer_model.h"

#ifndef DESIGNLAB_DONOT_USE_DXLIB

#include <DxLib.h>

#include "designlab_math_util.h"
#include "dxlib_util.h"
#include "model_loader.h"


namespace dlm = designlab::math_util;
namespace dldu = designlab::dxlib_util;


PhantomXRendererModel::PhantomXRendererModel(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr) :
	calculator_ptr_(calculator_ptr),
	body_model_handle_(-1)
{
}

void PhantomXRendererModel::SetDrawNode(const RobotStateNode& node)
{
	draw_node_ = node;
}

void PhantomXRendererModel::Draw() const
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý‚ª‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î•`‰æ‚µ‚È‚¢(‚Æ‚¢‚¤‚©‚Å‚«‚È‚¢)
	if(ModelLoader::GetIns()->LoadModel("model/body.mv1") == -1)
	{
		printfDx("ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½");
	}

	MV1SetScale(ModelLoader::GetIns()->LoadModel("model/body.mv1"), dldu::ConvertToDxlibVec(designlab::Vector3(10, 10, 10)));

	MV1SetRotationXYZ(ModelLoader::GetIns()->LoadModel("model/body.mv1"), { -1.57f, 0, 0 });

	MV1SetPosition(ModelLoader::GetIns()->LoadModel("model/body.mv1"), dldu::ConvertToDxlibVec(draw_node_.global_center_of_mass));

	MV1DrawModel(ModelLoader::GetIns()->LoadModel("model/body.mv1"));

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
	if (dlm::IsEqual((coxa_joint - femur_joint).GetLength(), HexapodConst::PHANTOMX_COXA_LENGTH)) { return true; }
	return false;
}


bool PhantomXRendererModel::IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const
{
	if (dlm::IsEqual((femur_joint - tibia_joint).GetLength(), HexapodConst::PHANTOMX_FEMUR_LENGTH)) { return true; }
	return false;
}


bool PhantomXRendererModel::IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const
{
	if (dlm::IsEqual((tibia_joint - leg_joint).GetLength(), HexapodConst::PHANTOMX_TIBIA_LENGTH) ) { return true; }
	return false;
}



#endif