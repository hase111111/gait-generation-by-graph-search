#include "graph_search_result_recoder.h"

#include <sstream>


namespace dlm = designlab::math_util;
namespace dllf = designlab::leg_func;


std::string GraphSearchResultRecoder::ToCsvString() const
{ 
    // std::boolalphaを使うと，bool値を文字列に変換できる．

    std::stringstream stream;

    stream << "bit_" << result_node.leg_state.to_string() << ",";
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << std::boolalpha << dllf::IsGrounded(result_node.leg_state, i) << ","; }
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << magic_enum::enum_name(dllf::GetDiscreteLegPos(result_node.leg_state, i)) << ","; }
    stream << magic_enum::enum_name(dllf::GetDiscreteComPos(result_node.leg_state)) << ",";
    stream << result_node.global_center_of_mass << ",";
    stream << result_node.rot << ",";
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << result_node.leg_pos[i] << ","; }
    for (int i = 0; i < HexapodConst::kLegNum; i++) { stream << result_node.leg_reference_pos[i] << ","; }
    stream << magic_enum::enum_name(result_node.next_move) << ",";
    stream << dlm::ConvertDoubleToString(computation_time) << ",";
    stream << magic_enum::enum_name(graph_search_result);

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
    stream << "Rotate[rad], , ,";
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
    stream << "x-axis,y-axis,z-axis,";
    stream << "x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,";
    stream << "x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,x,y,z,";
    stream << ",";
    stream << ",";
    stream << ",";

    return stream.str();
}
