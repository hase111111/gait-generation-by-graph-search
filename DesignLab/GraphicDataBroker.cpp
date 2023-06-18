#include "GraphicDataBroker.h"

void GraphicDataBroker::setMapState(const MapState& _map)
{
    //書き込み用のロックをかける．まずは，upgrade_lockを用意して，それをunique_lockに変更する．
    boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

    {
        boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

        //値をセットする．
        m_Map = _map;
    }
}

MapState GraphicDataBroker::getMapState() const
{
    //読み取り用のロックをかける．このスコープ { } を抜けるまでロックがかかる．(つまりこの関数が終わるまで)
    boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

    return m_Map;
}

void GraphicDataBroker::pushNode(const SNode &_node)
{
    //書き込み用のロックをかける．
    boost::upgrade_lock<boost::shared_mutex> upgrade_lock(m_mtx);

    {
        boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(upgrade_lock);

        //値をpushする．
        m_node.push_back(_node);
    }
}

void GraphicDataBroker::copyAllNode(std::vector<SNode>& _node_vec) const
{
    //読み取り用のロックをかける．
    boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

    _node_vec.clear();

    for (auto &i : m_node)
    {
        _node_vec.push_back(i);
    }
}

void GraphicDataBroker::copyOnlyNewNode(std::vector<SNode>& _node_vec) const
{
    //読み取り用のロックをかける．
    boost::shared_lock<boost::shared_mutex> read_lock(m_mtx);

    if (_node_vec.size() < m_node.size()) 
    {
        const size_t start_num = _node_vec.size();

        for (size_t i = start_num; i < m_node.size(); i++)
        {
            _node_vec.push_back(m_node.at(i));
        }
    }
}
