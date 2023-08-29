#include "graphic_data_broker.h"


void GraphicDataBroker::setMapState(const MapState& map)
{
	//書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

		//値をセットする．
		m_map_state = map;
	}
}


MapState GraphicDataBroker::getMapState() const
{
	//読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	return m_map_state;
}


void GraphicDataBroker::pushNode(const SNode& node)
{
	//書き込み用のロックをかける．
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

		//値をpushする．
		m_node.push_back(node);
	}
}


void GraphicDataBroker::copyAllNode(std::vector<SNode>* node_vec) const
{
	//読み取り用のロックをかける．
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	(*node_vec).clear();

	for (auto& i : m_node)
	{
		(*node_vec).push_back(i);
	}
}


void GraphicDataBroker::copyOnlyNewNode(std::vector<SNode>* node_vec) const
{
	//読み取り用のロックをかける．
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	if ((*node_vec).size() < m_node.size())
	{
		const size_t start_num = (*node_vec).size();

		for (size_t i = start_num; i < m_node.size(); ++i)
		{
			(*node_vec).push_back(m_node.at(i));
		}
	}
}


size_t GraphicDataBroker::getNodeNum() const
{
	//読み取り用のロックをかける．
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	return m_node.size();
}


void GraphicDataBroker::deleteAllNode()
{
	//書き込み用のロックをかける．
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

		//値をpushする．
		m_node.clear();
	}
}


void GraphicDataBroker::setSimuEnd()
{
	//書き込み用のロックをかける．
	boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

		m_simu_end.push_back(m_node.size() - 1);
	}
}

size_t GraphicDataBroker::getSimuEnd(const int simu_num)
{
	//読み取り用のロックをかける．
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	if (simu_num < m_simu_end.size())
	{
		return m_simu_end.at(simu_num);

	}
	else if (simu_num == m_simu_end.size())
	{
		return m_node.size() - 1;
	}

	return -1;
}


void GraphicDataBroker::copySimuEndIndex(std::vector<size_t>* simu_end_index) const
{
	//読み取り用のロックをかける．
	boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

	(*simu_end_index).clear();

	for (auto& i : m_simu_end)
	{
		(*simu_end_index).push_back(i);
	}
}

