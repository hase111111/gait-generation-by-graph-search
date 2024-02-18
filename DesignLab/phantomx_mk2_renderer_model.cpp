
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "phantomx_mk2_renderer_model.h"

#include <DxLib.h>

#include "math_util.h"
#include "math_rot_converter.h"
#include "math_rotation_matrix.h"
#include "dxlib_util.h"
#include "model_loader.h"
#include "phantomx_mk2_const.h"


namespace designlab
{

PhantomXMkIIRendererModel::PhantomXMkIIRendererModel(
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr) :
    converter_ptr_(converter_ptr),
    calculator_ptr_(calculator_ptr),
    draw_node_{}
{
}

void PhantomXMkIIRendererModel::SetNode(const RobotStateNode& node)
{
    draw_node_ = node;

    if (calculator_ptr_ == nullptr) { return; }

    draw_joint_state_ = calculator_ptr_->CalculateAllJointState(node);
}

void PhantomXMkIIRendererModel::Draw() const
{
    dxlib_util::SetZBufferEnable();  // Zバッファを有効にする．

    DrawBody();  // ボディの描画．

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawCoxaLink(i);  // 脚の描画．

        DrawFemurLink(i);  // 脚の描画．

        DrawTibiaLink(i);  // 脚の描画．

        DrawJointAxis(i);  // 関節軸の描画．
    }
}

void PhantomXMkIIRendererModel::DrawBody() const
{
    const int body_model_handle = ModelLoader::GetIns()->GetModelHandle("model/body.mv1");

    // モデルの読み込みがされていなければ描画しない(というかできない)
    if (body_model_handle == -1)
    {
        printfDx("モデルの読み込みに失敗しました．(body_model_handle)");
    }

    const VECTOR scale = VGet(10.f, 10.f, 10.f);  // モデルの寸法を調整するためのスケール．


    MV1SetScale(body_model_handle, scale);

    // dxlib の座標系は左手座標系なので，右手座標系に変換するために逆転させる．
    MV1SetRotationMatrix(body_model_handle,
               dxlib_util::ConvertToDxlibMat(ToRotationMatrix(draw_node_.posture.ToLeftHandCoordinate())));

    MV1SetPosition(body_model_handle, dxlib_util::ConvertToDxlibVec(draw_node_.center_of_mass_global_coord));

    MV1DrawModel(body_model_handle);

    // 胴体の正面に目印のために球を描画する．
    const VECTOR sphere_pos = dxlib_util::ConvertToDxlibVec(RotateVector3({ PhantomXMkIIConst::kCoxaBaseOffsetX - 20.f, 0.f, 0.f }, draw_node_.posture) + draw_node_.center_of_mass_global_coord);
    const float sphere_radius = 30.f;
    const int sphere_div_num = 16;
    const unsigned int sphere_color = GetColor(255, 0, 0);
    const unsigned int spec_color = GetColor(255, 255, 255);

    DrawSphere3D(sphere_pos, sphere_radius, sphere_div_num, sphere_color, spec_color, TRUE);
}

void PhantomXMkIIRendererModel::DrawCoxaLink(const int leg_index) const
{
    const int coxa_model_handle = ModelLoader::GetIns()->GetModelHandle("model/coxa_fixed.mv1");

    if (coxa_model_handle == -1)
    {
        printfDx("モデルの読み込みに失敗しました．(coxa_model_handle)");
    }

    if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4)
    {
        return;
    }
    if (draw_joint_state_[leg_index].joint_angle.size() != 3)
    {
        return;
    }

    // Coxa Joint は2つの Connect Link で構成されているので，それぞれ描画する．
    const VECTOR scale = VGet(10.f, 10.f, 10.f);

    const VECTOR coxa_joint_pos_global = dxlib_util::ConvertToDxlibVec(
      converter_ptr_->ConvertLegToGlobalCoordinate(
        draw_joint_state_[leg_index].joint_pos_leg_coordinate[0], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

    const Quaternion coxa_quat = Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(draw_joint_state_[leg_index].joint_angle[0], Vector3::GetUpVec()) *
        draw_node_.posture.ToLeftHandCoordinate();

    MV1SetScale(coxa_model_handle, scale);

    // dxlib の座標系は左手座標系なので，右手座標系に変換するために逆転させる．
    MV1SetRotationMatrix(coxa_model_handle, dxlib_util::ConvertToDxlibMat(ToRotationMatrix(coxa_quat)));

    MV1SetPosition(coxa_model_handle, coxa_joint_pos_global);

    MV1DrawModel(coxa_model_handle);
}

void PhantomXMkIIRendererModel::DrawFemurLink(const int leg_index) const
{
    const int thign_model_handle = ModelLoader::GetIns()->GetModelHandle("model/thign_l.mv1");

    if (thign_model_handle == -1) { printfDx("モデルの読み込みに失敗しました．(thign_model_handle)"); }

    if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4) { return; }
    if (draw_joint_state_[leg_index].joint_angle.size() != 3) { return; }

    // パラメータの計算．
    const VECTOR scale = VGet(10.f, 10.f, 10.f);

    const VECTOR femur_joint_pos_global_coord = dxlib_util::ConvertToDxlibVec(
      converter_ptr_->ConvertLegToGlobalCoordinate(
        draw_joint_state_[leg_index].joint_pos_leg_coordinate[1], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

    const float coxa_angle = draw_joint_state_[leg_index].joint_angle[0];
    const float femur_angle = draw_joint_state_[leg_index].joint_angle[1];

    // リンクが曲がっているため，簡単のために回転軸を結ぶように仮想的なリンクを使っている．
    // そのため，仮想的なリンクの角度を補正する必要がある．
    constexpr float virtual_link_offset_angle = math_util::ConvertDegToRad(12.5f);

    const Quaternion thign_def_quat =
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetFrontVec()) *
        Quaternion::MakeByAngleAxis(femur_angle, Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(virtual_link_offset_angle, Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec());

    // 原点の位置が少しズレているので，補正する．
    const VECTOR offset_pos = dxlib_util::ConvertToDxlibVec(RotateVector3(Vector3::GetFrontVec(),
                                                            Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec()) * draw_node_.posture.ToLeftHandCoordinate()));

    // 描画する．
    MV1SetScale(thign_model_handle, scale);

    // dxlib の座標系は左手座標系なので，右手座標系に変換するために逆転させる．
    Quaternion thign_quat = thign_def_quat * draw_node_.posture.ToLeftHandCoordinate();
    MV1SetRotationMatrix(thign_model_handle, dxlib_util::ConvertToDxlibMat(ToRotationMatrix(thign_quat)));

    MV1SetPosition(thign_model_handle, femur_joint_pos_global_coord + offset_pos);

    MV1DrawModel(thign_model_handle);
}

void PhantomXMkIIRendererModel::DrawTibiaLink(const int leg_index) const
{
    // モデルの読み込みを行う．
    // ここで呼び出すと毎フレーム読み込むことになりそうだが，
    // 実際は既に読込済みならばそのハンドルが返ってくるだけなので問題ない．
    // こんなところでこの処理を書いているのは，コンストラクタで呼び出すと，
    // Dxlib の初期化が終わっていないので，エラーが出るからである．
    int tibia_model_handle = ModelLoader::GetIns()->GetModelHandle("model/tibia_l_fixed.mv1");

    if (tibia_model_handle == -1)
    {
        printfDx("モデルの読み込みに失敗しました．(tibia_model_handle)");
    }

    if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4)
    {
        return;
    }
    if (draw_joint_state_[leg_index].joint_angle.size() != 3)
    {
        return;
    }

    // パラメータの計算．
    const VECTOR scale = VGet(0.01f, 0.01f, 0.01f);

    const VECTOR tibia_joint_pos_global_coord = dxlib_util::ConvertToDxlibVec(
      converter_ptr_->ConvertLegToGlobalCoordinate(
        draw_joint_state_[leg_index].joint_pos_leg_coordinate[2], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

    const float coxa_angle = draw_joint_state_[leg_index].joint_angle[0];

    const float femur_angle = draw_joint_state_[leg_index].joint_angle[1];

    const float tibia_angle = draw_joint_state_[leg_index].joint_angle[2];

    const Quaternion default_quat =
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetFrontVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(-femur_angle - tibia_angle, Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(-90.0f), Vector3::GetLeftVec()) *
        Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec()) *
        Quaternion::MakeByAngleAxis(math_util::ConvertDegToRad(180.0f), Vector3::GetUpVec());

    const VECTOR offset_pos = dxlib_util::ConvertToDxlibVec(RotateVector3(Vector3::GetFrontVec(),
                                                            Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec()) * draw_node_.posture.ToLeftHandCoordinate()));

    // 描画する．
    MV1SetScale(tibia_model_handle, scale);

    // dxlib の座標系は左手座標系なので，右手座標系に変換するために逆転させる．
    const Quaternion tibia_quat = default_quat * draw_node_.posture.ToLeftHandCoordinate();
    MV1SetRotationMatrix(tibia_model_handle, dxlib_util::ConvertToDxlibMat(ToRotationMatrix(tibia_quat)));

    MV1SetPosition(tibia_model_handle, tibia_joint_pos_global_coord + offset_pos);

    MV1DrawModel(tibia_model_handle);
}

void PhantomXMkIIRendererModel::DrawJointAxis(const int leg_index) const
{
    if (draw_joint_state_[leg_index].joint_pos_leg_coordinate.size() != 4)
    {
        return;
    }
    if (draw_joint_state_[leg_index].joint_angle.size() != 3)
    {
        return;
    }

    const float axis_length = 100.f;
    const float axis_radius = 2.f;
    const int axis_div_num = 16;

    const unsigned int coxa_axis_color = GetColor(0, 0, 255);
    const unsigned int femur_axis_color = GetColor(0, 255, 0);
    const unsigned int tibia_axis_color = femur_axis_color;
    const unsigned int spec_color = GetColor(255, 255, 255);

    const float coxa_angle = draw_joint_state_[leg_index].joint_angle[0];

    // Coxa の回転軸．
    {
        const VECTOR coxa_joint_pos_global_coord = dxlib_util::ConvertToDxlibVec(
          converter_ptr_->ConvertLegToGlobalCoordinate(
            draw_joint_state_[leg_index].joint_pos_leg_coordinate[0], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

        const VECTOR axis_vec = dxlib_util::ConvertToDxlibVec(
          RotateVector3(Vector3::GetUpVec() * axis_length / 2, draw_node_.posture));

        DrawCapsule3D(coxa_joint_pos_global_coord - axis_vec, coxa_joint_pos_global_coord + axis_vec, axis_radius, axis_div_num, coxa_axis_color, spec_color, TRUE);
    }

    // Femurの回転軸．
    {
        const VECTOR femur_joint_pos_global_coord = dxlib_util::ConvertToDxlibVec(
          converter_ptr_->ConvertLegToGlobalCoordinate(
            draw_joint_state_[leg_index].joint_pos_leg_coordinate[1], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

        const Quaternion default_quat = Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec());

        const VECTOR axis_vec = dxlib_util::ConvertToDxlibVec(
          RotateVector3(Vector3::GetLeftVec() * axis_length / 2, draw_node_.posture * default_quat));

        DrawCapsule3D(femur_joint_pos_global_coord - axis_vec, femur_joint_pos_global_coord + axis_vec, axis_radius, axis_div_num, femur_axis_color, spec_color, TRUE);
    }

    // Tibiaの回転軸．
    {
        const VECTOR tibia_joint_pos_global_coord = dxlib_util::ConvertToDxlibVec(
          converter_ptr_->ConvertLegToGlobalCoordinate(
            draw_joint_state_[leg_index].joint_pos_leg_coordinate[2], leg_index, draw_node_.center_of_mass_global_coord, draw_node_.posture, true));

        const Quaternion default_quat = Quaternion::MakeByAngleAxis(coxa_angle, Vector3::GetUpVec());

        const VECTOR axis_vec = dxlib_util::ConvertToDxlibVec(
          RotateVector3(Vector3::GetLeftVec() * axis_length / 2, draw_node_.posture * default_quat));

        DrawCapsule3D(tibia_joint_pos_global_coord - axis_vec, tibia_joint_pos_global_coord + axis_vec, axis_radius, axis_div_num, tibia_axis_color, spec_color, TRUE);
    }
}

}  // namespace designlab
