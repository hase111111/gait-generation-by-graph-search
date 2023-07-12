#include "ComMoveNodeCreator.h"
#include "ComCandidatePolygonMaker.h"

void ComMoveNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//重心移動先の候補地点の範囲を示す多角形を作成する
	ComCandidatePolygonMaker _maker;

	std::vector<my_vec::SPolygon2> _candidate_polygons;
	_maker.makeCandidatePolygon(_current_node, _candidate_polygons);

	//候補範囲から実際に移動する先の座標を決定する
}

SNode ComMoveNodeCreator::makeNextNode(const SNode& _current_node, const int _current_num, const my_vec::SVector _next_com_pos, const ComType::EComPattern _com_pattern)
{
	return SNode();
}
