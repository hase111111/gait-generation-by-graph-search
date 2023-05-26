#pragma once
#include "IGraphTreeCreator.h"
#include "MapState.h"

//�g������̃O���t���쐬����v���O�������ڐA�������̂ł��D
//���Ƃ��Ƃ̃v���O�����ōs��ꂽ�����̒��ŃR�����g�A�E�g���ꂽ���̂͂��ׂč폜�����̂ŁC�m�肽����Ήߋ��̃v���O�������Q�Ƃ��Ă��������D
class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato() = default;
	~GraphTreeCreatorHato() = default;

	bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) override;

private:

	void pushNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	const MapState * mp_Map;
};