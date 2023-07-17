#pragma once
#include "MapState.h"
#include "Node.h"
#include <vector>
#include "MyPolygon.h"
#include "ComType.h"

class ComMoveNodeCreator final
{
public:

	//! @brief �d�S�𕽍s�ړ������m�[�h�𐶐�����
	//! @param[in] _current_node �d�S�𕽍s�ړ�����m�[�h
	//! @param[in] _current_num �d�S�𕽍s�ړ�����m�[�h�̔ԍ�
	//! @param[out] _output_graph �d�S�𕽍s�ړ������m�[�h���i�[����R���e�i
	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

	const MapState* mp_Map;

	std::vector<my_vec::SPolygon2> m_Polygons;

	EHexapodMove m_next_move = EHexapodMove::LEG_HIERARCHY_CHANGE;

	const bool DO_DEBUG_PRINT = false;
};