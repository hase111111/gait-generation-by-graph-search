#include "graph_search_result_recoder.h"

#include <sstream>

#include "designlab_string_util.h"


namespace dlm = designlab::math_util;
namespace dllf = designlab::leg_func;
namespace dlsu = ::designlab::string_util;


std::string GraphSearchResultRecoder::ToCsvString() const
{ 
    // std::boolalphaを使うと，bool値を文字列に変換できる．
    // excelにおいて，数値を文字列として扱うためには，数値の前にシングルクォーテーション ' をつける必要がある．

    std::stringstream stream;

    stream << "'" << result_node.leg_state.to_string() << ",";
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << std::boolalpha << dllf::IsGrounded(result_node.leg_state, i) << ","; }
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << dlsu::MyEnumToString(dllf::GetDiscreteLegPos(result_node.leg_state, i)) << ","; }
    stream << dlsu::MyEnumToString(dllf::GetDiscreteComPos(result_node.leg_state)) << ",";
    stream << result_node.global_center_of_mass << ",";
    stream << result_node.quat << ",";
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << result_node.leg_pos[i] << ","; }
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << result_node.leg_reference_pos[i] << ","; }
    stream << dlsu::MyEnumToString(result_node.next_move) << ",";
    stream << dlm::ConvertDoubleToString(computation_time) << ",";
    stream << dlsu::MyEnumToString(graph_search_result);

    return stream.str();
}

std::string GraphSearchResultRecoder::GetCsvHeader()
{
    std::stringstream stream;

    stream << ",";
    stream << "Leg State Bit,";
    stream << "Is Grounded, , , , , , ";
    stream << "Discretized Leg Pos, , , , , , ";
    stream << "Discretized Com Pos,";
    stream << "Center of Mass[mm], , ,";
    stream << "Quaternion, , , ,";
    stream << "Leg Pos 0[mm],,,Leg Pos 1[mm], , ,Leg Pos 2[mm], , ,Leg Pos 3[mm], , ,Leg Pos 4[mm], , ,Leg Pos 5[mm], , , ";
    stream << "Reference Pos 0[mm],,,Reference Pos 1[mm],,,Reference Pos 2[mm],,,Reference Pos 3[mm],,,Reference Pos 4[mm],,,Reference Pos 5[mm],,,";
    stream << "Next Move,";
    stream << "Time[msec],";
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
