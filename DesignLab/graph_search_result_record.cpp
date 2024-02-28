
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "graph_search_result_record.h"

#include <sstream>

#include "string_util.h"


namespace designlab
{

std::string GraphSearchResult::ToString() const
{
    return string_util::EnumToStringRemoveTopK(result) + "," + message;
}

std::string GraphSearchResultRecord::ToCsvString() const
{
    // std::boolalphaを使うと，bool値を文字列に変換できる．
    // excelにおいて，数値を文字列として扱うためには，
    // 数値の前にシングルクォーテーション ' をつける必要がある．

    std::stringstream stream;

    stream << "'" << result_node.leg_state.to_string() << ",";

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        stream << std::boolalpha << leg_func::IsGrounded(result_node.leg_state, i) << ",";
    }

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        stream << string_util::EnumToStringRemoveTopK(
            leg_func::GetDiscreteLegPos(result_node.leg_state, i)) << ",";
    }

    stream << string_util::EnumToStringRemoveTopK(
        leg_func::GetDiscreteComPos(result_node.leg_state)) << ",";

    stream << result_node.center_of_mass_global_coord << ",";
    stream << result_node.posture << ",";

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        stream << result_node.leg_pos[i] << ",";
    }

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        stream << result_node.leg_reference_pos[i] << ",";
    }

    stream << string_util::EnumToStringRemoveTopK(result_node.next_move) << ",";
    stream << math_util::FloatingPointNumToString(computation_time) << ",";
    stream << graph_search_result.ToString();

    return stream.str();
}

std::string GraphSearchResultRecord::GetCsvHeader()
{
    std::stringstream stream;

    stream << ",";
    stream << "Leg State Bit,";
    stream << "Is Grounded, , , , , , ";
    stream << "Discretized Leg Pos, , , , , , ";
    stream << "Discretized Com Pos,";
    stream << "Center of Mass[mm], , ,";
    stream << "Quaternion, , , ,";
    stream << "Leg Pos 0[mm], , ,Leg Pos 1[mm], , ,Leg Pos 2[mm], , ,";
    stream << "Leg Pos 3[mm], , ,Leg Pos 4[mm], , ,Leg Pos 5[mm], , , ";
    stream << "Reference Pos 0[mm],,,Reference Pos 1[mm],,,Reference Pos 2[mm],,,";
    stream << "Reference Pos 3[mm],,,Reference Pos 4[mm],,,Reference Pos 5[mm],,,";
    stream << "Next Move,";
    stream << "Time[milli sec],";
    stream << "Graph Search Result" << std::endl;

    stream << ",";
    stream << ",";
    stream << "leg0,leg1,leg2,leg3,leg4,leg5,";
    stream << "leg0,leg1,leg2,leg3,leg4,leg5,";
    stream << ",";
    stream << "x,y,z,";
    stream << "w,v-x,v-y,v-z,";
    stream << "x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,";
    stream << "x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,";
    stream << ",";
    stream << ",";
    stream << ",";

    return stream.str();
}

}  // namespace designlab
