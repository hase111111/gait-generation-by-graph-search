#include "graphic_main_test.h"

#include "designlab_math_util.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "node_initializer.h"
#include "phantomx_mk2_const.h"
#include "simulation_map_creator.h"


namespace dl = ::designlab;
namespace dldu = ::designlab::dxlib_util;
namespace dlm = ::designlab::math_util;


GraphicMainTest::GraphicMainTest(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
) :
	node_display_gui_(setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 0, 10, calculator_ptr, checker_ptr),
	hexapod_renderer_(HexapodRendererBuilder::Build(converter_ptr, calculator_ptr, setting_ptr->gui_display_quality)),
	calculator_ptr_(calculator_ptr),
	converter_ptr_(converter_ptr)
{
	NodeInitializer node_initializer;
	node_ = node_initializer.InitNode();

	const MapCreateModeMessenger messanger;
	SimulationMapCreator map_creator(messanger);

	map_state_ = map_creator.InitMap();
	devide_map_state_.Init(map_state_, {});

	map_render_.SetMapState(map_state_);
	map_render_.SetHexapodPosition(node_.global_center_of_mass);
}


bool GraphicMainTest::Update()
{
	MoveLeg();

	if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LSHIFT) > 0 || Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RSHIFT) > 0)
	{
		MoveBody();
	}

	hexapod_renderer_->SetDrawNode(node_);

	node_display_gui_.SetDisplayNode(node_);

	node_display_gui_.Update();

	camera_gui_.SetHexapodPos(node_.global_center_of_mass);  //カメラの位置を更新する．

	camera_gui_.Update();      //カメラのGUIを更新する．

	return true;
}


void GraphicMainTest::Draw() const
{
	dldu::SetZBufferEnable();

	map_render_.Draw();

	hexapod_renderer_->Draw();

	camera_gui_.Draw();        //カメラのGUIを描画する．

	node_display_gui_.Draw();	 //ノードの情報を表示するGUIを描画する．

}

void GraphicMainTest::MoveBody()
{
	const float kComSpeed = 1.1f;

	if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0)
	{
		designlab::Vector3 com = 
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetUpVec() * kComSpeed,node_.quat);
		
		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0)
	{
		designlab::Vector3 com =
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetUpVec() * -kComSpeed, node_.quat);

		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0)
	{
		designlab::Vector3 com =
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetLeftVec() * kComSpeed, node_.quat);

		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0)
	{
		designlab::Vector3 com =
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetLeftVec() * -kComSpeed, node_.quat);

		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0)
	{
		designlab::Vector3 com =
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetFrontVec() * kComSpeed, node_.quat);

		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0)
	{
		designlab::Vector3 com =
			node_.global_center_of_mass + designlab::RotateVector3(designlab::Vector3::GetFrontVec() * -kComSpeed, node_.quat);

		node_.ChangeGlobalCenterOfMass(com, false);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_R) > 0)
	{
		float angle_speed = kComSpeed / 360.0f * 2.f * dlm::kFloatPi;

		if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0) 
		{
			angle_speed *= -1.f;
		}

		designlab::Quaternion rot = dl::Quaternion::MakeByAngleAxis(angle_speed, designlab::Vector3::GetFrontVec()) * node_.quat;

		node_.ChangeQuat(converter_ptr_, rot);
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_P) > 0)
	{
		float angle_speed = kComSpeed / 360.0f * 2.f * dlm::kFloatPi;

		if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0)
		{
			angle_speed *= -1.f;
		}

		designlab::Quaternion rot = dl::Quaternion::MakeByAngleAxis(angle_speed, designlab::Vector3::GetLeftVec()) * node_.quat;

		node_.ChangeQuat(converter_ptr_, rot);
	}
}

void GraphicMainTest::MoveLeg()
{
	const float kSpeed = 1;
	const float kAngleSpeed = 0.01f;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_1 + i) > 0)
		{
			if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0) { node_.leg_pos[i].z += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0) { node_.leg_pos[i].z -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0) { node_.leg_pos[i].y += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0) { node_.leg_pos[i].y -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0) { node_.leg_pos[i].x += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0) { node_.leg_pos[i].x -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_M) == 1)
			{
				designlab::Vector3 global = converter_ptr_->ConvertLegToGlobalCoordinate(
					node_.leg_reference_pos[i], i, node_.global_center_of_mass, node_.quat, true
				);

				int map_x = devide_map_state_.GetDevideMapIndexX(global.x);
				int map_y = devide_map_state_.GetDevideMapIndexY(global.y);

				if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_UP) > 0) { map_x++; }
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_DOWN) > 0) { map_x--; }
				if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LEFT) > 0) { map_y++; }
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RIGHT) > 0) { map_y--; }

				designlab::Vector3 map_pos = devide_map_state_.GetPointPos(map_x, map_y, map_index_ % devide_map_state_.GetPointNum(map_x, map_y));
				map_index_++;

				node_.leg_pos[i] = converter_ptr_->ConvertGlobalToLegCoordinate(
					map_pos, i, node_.global_center_of_mass, node_.quat, true
				);
			}
			
			if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_C) > 0 || Keyboard::GetIns()->GetPressingCount(KEY_INPUT_F) > 0 || Keyboard::GetIns()->GetPressingCount(KEY_INPUT_T) > 0)
			{
				std::array<HexapodJointState, HexapodConst::kLegNum> res = calculator_ptr_->CalculateAllJointState(node_);

				float coxa = res[i].joint_angle[0];
				float femur = res[i].joint_angle[1];
				float tibia = res[i].joint_angle[2];

				if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_C) > 0) 
				{
					const float speed = Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ? kAngleSpeed : kAngleSpeed * -1.f;
					
					coxa += speed;

					coxa = PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax <= coxa ?
						PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax : coxa;

					coxa = PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin >= coxa ?
						PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin : coxa;
				}
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_F) > 0) 
				{
					const float speed = Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ? kAngleSpeed : kAngleSpeed * -1.f;
					
					femur += speed;

					femur = (femur + tibia - dlm::kFloatPi) > 0 ? femur - speed : femur;
					femur = PhantomXMkIIConst::kFemurAngleMax <= femur ? PhantomXMkIIConst::kFemurAngleMax : femur;
					femur = PhantomXMkIIConst::kFemurAngleMin >= femur ? PhantomXMkIIConst::kFemurAngleMin : femur;
				}
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_T) > 0) 
				{
					float spped = Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ? kAngleSpeed : kAngleSpeed * -1.f;
					tibia += spped;
					tibia = (femur + tibia - dlm::kFloatPi) > 0 ? tibia - spped : tibia;
					tibia = PhantomXMkIIConst::kTibiaAngleMax <= tibia ? PhantomXMkIIConst::kTibiaAngleMax : tibia;
					tibia = PhantomXMkIIConst::kTibiaAngleMin >= tibia ? PhantomXMkIIConst::kTibiaAngleMin : tibia;
				}

				designlab::Vector3 leg_pos;
				leg_pos += designlab::Vector3{ PhantomXMkIIConst::kCoxaLength * cos(coxa),PhantomXMkIIConst::kCoxaLength * sin(coxa),0 };

				leg_pos += designlab::Vector3{
					PhantomXMkIIConst::kFemurLength * cos(coxa) * cos(femur),
					PhantomXMkIIConst::kFemurLength * sin(coxa) * cos(femur),
					PhantomXMkIIConst::kFemurLength * sin(femur) 
				};
				leg_pos += designlab::Vector3{
					PhantomXMkIIConst::kTibiaLength * cos(coxa) * cos(femur + tibia),
					PhantomXMkIIConst::kTibiaLength * sin(coxa) * cos(femur + tibia),
					PhantomXMkIIConst::kTibiaLength * sin(femur + tibia)
				};

				node_.leg_pos[i] = leg_pos;
			}
		}
	}
}