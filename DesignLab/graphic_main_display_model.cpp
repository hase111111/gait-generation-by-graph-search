
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "graphic_main_display_model.h"

#include <numbers>

#include "approximated_motion_range_render.h"
#include "camera_dragger.h"
#include "math_util.h"
#include "dxlib_camera.h"
#include "dxlib_gui_camera.h"
#include "dxlib_gui_camera_parameter_displayer.h"
#include "dxlib_gui_node_displayer.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_creator_factory.h"
#include "map_renderer.h"
#include "node_initializer.h"
#include "simulation_setting_record.h"
#include "toml_file_importer.h"
#include "phantomx_mk2_const.h"
#include "world_grid_renderer.h"


namespace designlab
{

GraphicMainDisplayModel::GraphicMainDisplayModel(
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr) :
    mouse_ptr_(std::make_shared<Mouse>()),
    calculator_ptr_(calculator_ptr),
    converter_ptr_(converter_ptr)
{
    NodeInitializer node_initializer{ Vector3{0.f, 0.f, 30.f}, EulerXYZ(), enums::HexapodMove::kNone };
    robot_ = node_initializer.InitNode();

    TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
    const SimulationSettingRecord simulation_setting_record = simulation_setting_importer.ImportOrUseDefault("./simulation_condition/simulation_setting.toml");
    auto map_creator = MapCreatorFactory::Create(simulation_setting_record);

    map_state_ = map_creator->InitMap();
    divided_map_state_.Init(map_state_, {});

    const auto camera = std::make_shared<DxlibCamera>();
    const auto camera_gui = std::make_shared<DxlibGuiCamera>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);

    const auto camera_dragger = std::make_shared<CameraDragger>(camera);

    const auto camera_parameter_gui = std::make_shared<DxlibGuiCameraParameterDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_parameter_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);
    camera_parameter_gui->SetVisible(false);

    const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr, calculator_ptr, checker_ptr);
    node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10, kDxlibGuiAnchorRightTop, true);

    const auto [hexapod_renderer, hexapod_node_setter] =
        HexapodRendererBuilder::Build(converter_ptr_, calculator_ptr_, setting_ptr->gui_display_quality);

    const auto map_renderer = std::make_shared<MapRenderer>();
    map_renderer->SetMapState(map_state_);
    map_renderer->SetNode(robot_);

    const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();

    const auto approximated_motion_range_render =
        std::make_shared<ApproximatedMotionRangeRender>(checker_ptr, converter_ptr);

    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_parameter_gui), 1);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibDraggable>>(camera_dragger), 0);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(node_display_gui), 1);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_gui), 1);

    gui_updater_.OpenTerminal();

    node_setter_group_.Register(approximated_motion_range_render);
    node_setter_group_.Register(camera_gui);
    node_setter_group_.Register(node_display_gui);
    node_setter_group_.Register(hexapod_node_setter);
    node_setter_group_.Register(map_renderer);

    // render_group_.Register(approximated_motion_range_render);
    render_group_.Register(hexapod_renderer);
    render_group_.Register(map_renderer);
    render_group_.Register(world_grid_renderer);
}


bool GraphicMainDisplayModel::Update()
{
    assert(mouse_ptr_ != nullptr);

    mouse_ptr_->Update();

    if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LSHIFT) > 0 ||
        Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RSHIFT) > 0)
    {
        MoveBody();
    }
    else
    {
        MoveLeg();
    }

    gui_updater_.Activate(mouse_ptr_);

    node_setter_group_.SetNode(robot_);

    return true;
}

void GraphicMainDisplayModel::Draw() const
{
    render_group_.Draw();

    gui_updater_.Draw();
}


void GraphicMainDisplayModel::MoveBody()
{
    const float kComSpeed = 1.1f;

    if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetUpVec() * kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetUpVec() * -kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetLeftVec() * kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetLeftVec() * -kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetFrontVec() * kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0)
    {
        Vector3 com =
            robot_.center_of_mass_global_coord +
            RotateVector3(Vector3::GetFrontVec() * -kComSpeed, robot_.posture);

        robot_.ChangeGlobalCenterOfMass(com, false);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_R) > 0)
    {
        float angle_speed = kComSpeed / 360.0f * 2.f * std::numbers::pi_v<float>;

        if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0)
        {
            angle_speed *= -1.f;
        }

        Quaternion rot = Quaternion::MakeByAngleAxis(angle_speed,
                                                     Vector3::GetFrontVec()) * robot_.posture;

        robot_.ChangePosture(converter_ptr_, rot);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_P) > 0)
    {
        float angle_speed = kComSpeed / 360.0f * 2.f * std::numbers::pi_v<float>;

        if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0)
        {
            angle_speed *= -1.f;
        }

        Quaternion rot = Quaternion::MakeByAngleAxis(angle_speed, Vector3::GetLeftVec()) *
            robot_.posture;

        robot_.ChangePosture(converter_ptr_, rot);
    }
    else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Y) > 0)
    {
        float angle_speed = kComSpeed / 360.0f * 2.f * std::numbers::pi_v<float>;

        if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0)
        {
            angle_speed *= -1.f;
        }

        Quaternion rot = Quaternion::MakeByAngleAxis(angle_speed, Vector3::GetUpVec()) * robot_.posture;

        robot_.ChangePosture(converter_ptr_, rot);
    }
}

void GraphicMainDisplayModel::MoveLeg()
{
    const float kSpeed = 1;
    const float kAngleSpeed = 0.01f;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_1 + i) > 0)
        {
            if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0)
            {
                robot_.leg_pos[i].z += kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0)
            {
                robot_.leg_pos[i].z -= kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0)
            {
                robot_.leg_pos[i].y += kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0)
            {
                robot_.leg_pos[i].y -= kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0)
            {
                robot_.leg_pos[i].x += kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0)
            {
                robot_.leg_pos[i].x -= kSpeed;
            }
            else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_M) == 1)
            {
                Vector3 global =
                    converter_ptr_->ConvertLegToGlobalCoordinate(
                    robot_.leg_reference_pos[i],
                    i,
                    robot_.center_of_mass_global_coord,
                    robot_.posture,
                    true);

                int map_x = divided_map_state_.GetDividedMapIndexX(global.x);
                int map_y = divided_map_state_.GetDividedMapIndexY(global.y);

                if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_UP) > 0)
                {
                    map_x++;
                }
                else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_DOWN) > 0)
                {
                    map_x--;
                }
                if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LEFT) > 0)
                {
                    map_y++;
                }
                else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RIGHT) > 0)
                {
                    map_y--;
                }

                Vector3 map_pos = divided_map_state_.GetPointPos(
                    map_x, map_y,
                    divided_map_tile_index_ % divided_map_state_.GetPointNum(map_x, map_y));

                ++divided_map_tile_index_;

                robot_.leg_pos[i] = converter_ptr_->ConvertGlobalToLegCoordinate(
                  map_pos, i, robot_.center_of_mass_global_coord, robot_.posture, true);
            }

            if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_C) > 0 ||
                Keyboard::GetIns()->GetPressingCount(KEY_INPUT_F) > 0 ||
                Keyboard::GetIns()->GetPressingCount(KEY_INPUT_T) > 0)
            {
                std::array<HexapodJointState, HexapodConst::kLegNum> res =
                    calculator_ptr_->CalculateAllJointState(robot_);

                float coxa = res[i].joint_angle[0];
                float femur = res[i].joint_angle[1];
                float tibia = res[i].joint_angle[2];

                if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_C) > 0)
                {
                    const float speed =
                        Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ?
                        kAngleSpeed : kAngleSpeed * -1.f;

                    coxa += speed;

                    coxa = PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax <= coxa ?
                        PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax : coxa;

                    coxa = PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin >= coxa ?
                        PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin : coxa;
                }
                else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_F) > 0)
                {
                    const float speed = Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ?
                        kAngleSpeed : kAngleSpeed * -1.f;

                    femur += speed;

                    femur = (femur + tibia - std::numbers::pi_v<float>) > 0 ? femur - speed : femur;
                    femur = PhantomXMkIIConst::kFemurAngleMax <= femur ? PhantomXMkIIConst::kFemurAngleMax : femur;
                    femur = PhantomXMkIIConst::kFemurAngleMin >= femur ? PhantomXMkIIConst::kFemurAngleMin : femur;
                }
                else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_T) > 0)
                {
                    float speed = Keyboard::GetIns()->GetPressingCount(KEY_INPUT_I) > 0 ? kAngleSpeed : kAngleSpeed * -1.f;
                    tibia += speed;
                    tibia = (femur + tibia - std::numbers::pi_v<float>) > 0 ? tibia - speed : tibia;
                    tibia = PhantomXMkIIConst::kTibiaAngleMax <= tibia ? PhantomXMkIIConst::kTibiaAngleMax : tibia;
                    tibia = PhantomXMkIIConst::kTibiaAngleMin >= tibia ? PhantomXMkIIConst::kTibiaAngleMin : tibia;
                }

                Vector3 leg_pos;

                leg_pos += Vector3{
                    PhantomXMkIIConst::kCoxaLength * cos(coxa),
                        PhantomXMkIIConst::kCoxaLength * sin(coxa),
                        0 };

                leg_pos += Vector3{
                    PhantomXMkIIConst::kFemurLength * cos(coxa) * cos(femur),
                        PhantomXMkIIConst::kFemurLength * sin(coxa) * cos(femur),
                        PhantomXMkIIConst::kFemurLength * sin(femur)
                };

                leg_pos += Vector3{
                    PhantomXMkIIConst::kTibiaLength * cos(coxa) * cos(femur + tibia),
                        PhantomXMkIIConst::kTibiaLength * sin(coxa) * cos(femur + tibia),
                        PhantomXMkIIConst::kTibiaLength * sin(femur + tibia)
                };

                robot_.leg_pos[i] = leg_pos;
            }
        }
    }
}

}  // namespace designlab
