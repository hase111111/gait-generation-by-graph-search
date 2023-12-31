
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "dead_lock_checker.h"


namespace designlab
{

void DeadLockChecker::AddNode(const RobotStateNode& node)
{
    // ノードを前から追加する
    node_vec_.push_front(node);

    // 最大保持数を超えているならばその分だけ，後ろから削除する．
    while (node_vec_.size() > kMaxDataNum)
    {
        node_vec_.pop_back();
    }
}


bool DeadLockChecker::IsDeadLock() const
{
    if (node_vec_.size() < 1)
    {
        // 比較するためのノードがないならば即終了．
        return false;
    }

    auto itr = node_vec_.begin();  // ノードの最初を指すイテレーターを取得して，
    itr++;  // 一つ進める．

    for (size_t i = 0; i < kMaxDataNum; i++)
    {
        if (itr != node_vec_.end())
        {
            itr++;
        }
    }

    // イテレーターが最後になるまでループする．
    for (itr; itr != node_vec_.end(); itr++)
    {
        // 同じノードがあれば，動作がループしているとみなし，trueを返す．
        if (node_vec_.front() == (*itr))
        {
            return true;
        }
    }

    return false;
}

}  // namespace designlab
