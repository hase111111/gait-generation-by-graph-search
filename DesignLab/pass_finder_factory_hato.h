#pragma once

#include "abstract_pass_finder_factory.h"


//! @class PassFinderFactoryHato
//! @date 2023/08/14
//! @author ���J��
//! @brief �p�X�T���N���X�̃t�@�N�g���[�N���X
class PassFinderFactoryHato final : public AbstractPassFinderFactory
{
public:
	PassFinderFactoryHato() = default;
	~PassFinderFactoryHato() = default;

	void createGraphTreeCreator(const MapState* const map, std::unique_ptr<IGraphTreeCreator>& tree) override;

	void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) override;
};


//! @file pass_finder_factory_hato.h
//! @date 2023/08/14
//! @author ���J��
//! @brief �p�X�T���N���X�̃t�@�N�g���[�N���X
//! @n �s�� : @lineinfo