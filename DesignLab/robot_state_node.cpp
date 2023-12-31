
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "robot_state_node.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "math_util.h"
#include "string_util.h"
#include "leg_state.h"


namespace designlab
{

void RobotStateNode::ChangeGlobalCenterOfMass(const Vector3& new_com, const bool base)
{
    const Vector3 dif = new_com - center_of_mass_global_coord;
    const Vector3 com_dif = RotateVector3(dif, posture.GetConjugate());

    center_of_mass_global_coord = new_com;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(leg_state, i))
        {
            leg_pos[i] -= com_dif;
            leg_reference_pos[i] = leg_pos[i];
        }
        else
        {
            if (base)leg_reference_pos[i] -= com_dif;
        }
    }
}

void RobotStateNode::ChangePosture(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const Quaternion& new_posture)
{
    const Quaternion dif = new_posture * posture.GetConjugate();

    posture = new_posture;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(leg_state, i))
        {
            Vector3 leg_pos_robot_coord =
                converter_ptr->ConvertLegToRobotCoordinate(leg_pos[i], i);
            leg_pos_robot_coord = RotateVector3(leg_pos_robot_coord, dif.GetConjugate());
            leg_pos[i] = converter_ptr->ConvertRobotToLegCoordinate(leg_pos_robot_coord, i);

            Vector3 leg_reference_pos_robot_coord =
                converter_ptr->ConvertLegToRobotCoordinate(leg_reference_pos[i], i);
            leg_reference_pos_robot_coord =
                RotateVector3(leg_reference_pos_robot_coord, dif.GetConjugate());
            leg_reference_pos[i] =
                converter_ptr->ConvertRobotToLegCoordinate(leg_reference_pos_robot_coord, i);
        }
    }
}

std::string RobotStateNode::ToString() const
{
    // \t はタブを表す文字．また，スペースのみを追加したい場合は std::string(" ") とする．

    std::string res;  // 結果として返す文字列．

    // 脚状態のビット列．
    res += "Leg State Bit : " + leg_state.to_string() + "\n";
    res += "\n";

    // 重心位置の出力．
    enums::DiscreteComPos com = leg_func::GetDiscreteComPos(leg_state);
    res += "Com Pattern : " + string_util::EnumToStringRemoveTopK(com) +
        "(" + std::to_string(static_cast<int>(com)) + ")\n";

    // 脚の接地状態．
    res += "Ground : ";

    for (int i = 0; i < HexapodConst::kLegNum; ++i)
    {
        res += (leg_func::IsGrounded(leg_state, i) ? "ground" : "lifted") + std::string(" ");
    }

    res += "\n";

    // 脚の階層．
    res += "Hierarchy : ";

    for (int i = 0; i < HexapodConst::kLegNum; ++i)
    {
        enums::DiscreteLegPos dis_leg_pos = leg_func::GetDiscreteLegPos(leg_state, i);
        res += string_util::EnumToStringRemoveTopK(dis_leg_pos) +
            "(" + std::to_string(static_cast<int>(dis_leg_pos)) + ") ";
    }

    res += "\n\n";

    // 脚位置．
    res += "Leg Position : \n";

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        res += "  " + std::to_string(i) + ":" + leg_pos[i].ToString() + "\n";
    }

    res += "Leg Base Position : \n";

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        res += "  " + std::to_string(i) + ":" + leg_reference_pos[i].ToString() + "\n";
    }

    // 重心位置．
    res += "\nGlobal Center of Mass : " + center_of_mass_global_coord.ToString() + "\n";

    // 回転姿勢．
    res += "Quaternion  : " + posture.ToString() + "\n";

    // 次動作．
    res += "\n";
    res += "(Next Move : " + string_util::EnumToStringRemoveTopK(next_move) +
        std::string(")\n");
    res += "(Depth : " + std::to_string(depth) + std::string(")\n");
    res += "(parent number : " + std::to_string(parent_index) + std::string(")\n");

    return res;
}

std::string RobotStateNode::ToCsvString() const
{
    std::stringstream ss;

    ss << *this;

    return ss.str();
}

RobotStateNode RobotStateNode::FromString(const std::string& str)
{
    std::vector<std::string> data = string_util::Split(str, ",");

    RobotStateNode res;
    int cnt = 0;

    try
    {
        // 脚状態のビット列．
        res.leg_state = std::bitset<leg_func::kLegStateBitNum>(data[cnt++]);

        // 脚の位置．
        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            res.leg_pos[i] =
                Vector3{ std::stof(data[cnt++]), std::stof(data[cnt++]), std::stof(data[cnt++]) };
        }

        // 脚の基準位置．
        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            res.leg_reference_pos[i] =
                Vector3{ std::stof(data[cnt++]), std::stof(data[cnt++]), std::stof(data[cnt++]) };
        }

        // 重心位置．
        res.center_of_mass_global_coord =
            Vector3{ std::stof(data[cnt++]), std::stof(data[cnt++]), std::stof(data[cnt++]) };

        // 回転姿勢．
        res.posture =
            Quaternion{ std::stof(data[cnt++]),
            std::stof(data[cnt++]), std::stof(data[cnt++]), std::stof(data[cnt++]) };

        // 次動作．
        res.next_move = magic_enum::enum_cast<enums::HexapodMove>(data[cnt++]).value();

        // 親ノードの番号．
        res.parent_index = std::stoi(data[cnt++]);

        // 深さ．
        res.depth = std::stoi(data[cnt++]);
    }
    catch (...)
    {
        //! @todo うまく読み込めなかった時の処理を書く．
    }

    return res;
}


}  // namespace designlab
